#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <math.h>

#define NUM_OF_THREADS 4
#define REC_SIZE 63
#define KEY_SIZE 8

#define BUF_SIZE 16
#define LINE_LENGTH 62

pthread_mutex_t toggleMutex;
char* fileName = "invalid";
int num_threads = NUM_OF_THREADS;
int num_threads_merge = -1;
char *pbuf;
ssize_t numRead;
char *addr;
size_t len = 0;
FILE *fileIn;
int fileNum;
char *thread_offset;
long num_records_per_thread;
int increment_offset;
struct stat sb;
// for the threads
long t;
int rc;
pthread_t thread[512];
pthread_attr_t attr;

	char* calc_offset(long t) {
		if(t == 0) {
            return addr;
		}
		else {
            char *offset = addr + (t * increment_offset);
            return offset;
		}
	}

	// sort the array
    //custom compare
    int compare(const void *a, const void *b) {
		char keya[8];
		strncpy(keya, (char *)a, 8);
		return strncmp((char *)a, (char *)b, 8);
    }


    void merge_threads(long current_t, int iter, long rec_per_threads) {
        pthread_mutex_lock(&toggleMutex);
        long target_thread = current_t + pow(2, iter);
        iter++;
        pthread_mutex_unlock(&toggleMutex);
        printf("In thread %ld, iteration number %d\n", current_t, iter-1);

        if(current_t % 2 == 0) {
            printf("TARGET thread: %ld.\n", target_thread);

            if(current_t % target_thread == 0) {
                pthread_join(thread[target_thread], NULL);
                printf("pthread %ld has been merged with pthread %ld.\n", current_t, target_thread);
                qsort(calc_offset((long)current_t), rec_per_threads, REC_SIZE, compare);
                num_threads_merge = num_threads_merge - 1;
                printf("Number of threads after merge: %d\n", num_threads_merge);
                if(num_threads_merge == 1) {
                    pthread_exit(NULL);
                    return;
                }
                merge_threads(current_t, iter, rec_per_threads*2);
            }
            printf("pthread %ld has been merged with pthread %ld.\n", current_t, target_thread);
            num_threads_merge = num_threads_merge - 1;
            printf("Number of threads after merge: %d\n", num_threads_merge);

            qsort(calc_offset((long)current_t), rec_per_threads*2, REC_SIZE, compare);
        }
        // sort the odds
        qsort(calc_offset((long)current_t), rec_per_threads, REC_SIZE, compare);
    }


    void *thread_sort(void *t) {
        printf("Thread %ld is working\n", (long)t);
        merge_threads((long)t, 0, num_records_per_thread);
        //qsort(calc_offset((long)t), num_records_per_thread, REC_SIZE, compare);
		pthread_exit(NULL);
    }



int main(int argc, char *argv[])
{

  printf ("Default threads = %d\n", num_threads);
  int n;
    if(argc > 1) {
      for(n = 1; n < argc; n++) {
	if(strcmp(argv[n], "-t") == 0) {
	    num_threads = atoi(argv[++n]);
	    num_threads_merge = num_threads;
	    printf("Set num_threads = %d\n", num_threads);
            continue;
	}
	else if(strcmp(argv[n], "-file") == 0) {
	    fileName = argv[++n];
	    printf("Set input file\n");
            continue;
	}
        else {
           printf("Input file : %s.\n", fileName);
           printf("ThreadCount : %d.\n", num_threads);
        }
      }
    }
    else {
      printf("Usage: \n\t-t <num_threads>\n\t-file <input_file>\n");
    }


   // Allocate buffer for reading & writing
   pbuf = (char *)malloc(BUF_SIZE);
   if (pbuf == 0) {
     printf("malloc failed.\n");
     exit(EXIT_FAILURE);
   }


    fileNum = open(fileName, O_RDONLY);
    if(fileNum == -1) {
	printf("Error opening file.");
        exit(-1);
    }
    printf("File Opened Successfully\n");

    if((fstat(fileNum, &sb)) == -1) {
         printf("fstat fail");
         exit(-1);
    }

	int sb_size = sb.st_size;
    int num_records = sb_size/REC_SIZE;

	printf("SB SIZE: %d\n", sb_size);
	printf("num_records: %d\n", num_records);

    addr = (char *)mmap(NULL, sb_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fileNum, 0);

 	if (addr == MAP_FAILED) {
		close(fileNum);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
    }
    printf("Unsorted Addr: \n%s\n", addr);

	num_records_per_thread = num_records/num_threads;
	printf("Number of Records per Thread = %ld\n", num_records_per_thread);
	increment_offset = num_records_per_thread * REC_SIZE;
	// set thread offset variable to addr
	thread_offset = addr - increment_offset;
	//qsort(addr, num_records, REC_SIZE, compare);


    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(t = 0; t < num_threads; t++) {
      printf("creating thread %ld\n", t);
      rc = pthread_create(&thread[t], &attr, thread_sort, (void *)t);
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
    }

    // wait for thread 0 to finish.
    // this means all have finished.
    rc = pthread_join(thread[0], NULL);
    if (rc) {
        printf("ERROR; return code from pthread_create() for thread %ld is %d\n", t, rc);
        //exit(-1);
    }

    //last sort with 1 thread left
    // comment this out to see the last merge
    qsort(calc_offset((long)0), num_records, REC_SIZE, compare);

    printf("Sort complete.\n\n");

    printf("Sorted Records: \n%s\n", addr);

    return 0;
}
