/*Alex Stokes and Jake Diamond */

vocalist(james_hetfield, metallica).
vocalist(james_mercer, shins).
vocalist(thom_yorke, radiohead).
vocalist(roger_waters, pink_floyd).
vocalist(don_henley, eagles).

genre(metal, metallica).
genre(alternative, shins).
genre(classic_rock, eagles).
genre(psychedelic, pink_floyd).
genre(alternative, radiohead).

manager(irving_azoff, eagles).
manager(chris_hufford, radiohead).
manager(david_burton, shins).
manager(steve_orourke, pink_floyd).
manager(peter_mensch, metallica).

label(universal, metallica).
label(subpop, shins).
label(emi, eagles).
label(beggars, radiohead).
label(emi, pink_floyd).

vocalistWorth(150000000, james_hetfield).
vocalistWorth(200000000, don_henley).
vocalistWorth(270000000, roger_waters).
vocalistWorth(35000000, thom_yorke).
vocalistWorth(6000000, james_mercer).


song(fuel, metallica).
song(fade_to_black, metallica).
song(and_justice_for_all, metallica).
song(battery, metallica).
song(for_whom_the_bell_tolls, metallica).
song(seek_and_destroy, metallica).
song(one, metallica).
song(enter_sandman, metallica).
song(creeping_death, metallica).
song(master_of_puppets, metallica).

song(turn_on_me, shins).
song(phantom_limb, shins).
song(sea_legs, shins).
song(girl_sailor, shins).
song(australia ,shins).
song(caring_is_creepy, shins).
song(new_slang, shins).

song(everything_in_its_right_place, radiohead).
song(knives_out, radiohead).
song(fake_plastic_trees, radiohead).
song(i_might_be_wrong, radiohead).
song(creep, radiohead).
song(black_star, radiohead).
song(high_and_dry, radiohead).
song(there_there, radiohead).
song(bodysnatchers, radiohead).
song(nude, radiohead).
song(all_i_need, radiohead).
song(bangers_and_mash, radiohead).

song(wish_you_were_here, pink_floyd).
song(shine_on_you_crazy_diamond, pink_floyd).
song(time, pink_floyd).
song(mother, pink_floyd).
song(comfortably_numb, pink_floyd).
song(money, pink_floyd).
song(breathe, pink_floyd).
song(dogs, pink_floyd).
song(pigs, pink_floyd).
song(cows, pink_floyd).

song(hotel_california, eagles).
song(take_it_easy, eagles).
song(desperado, eagles).
song(witchy_woman, eagles).
song(new_kid_in_town, eagles).
song(livin_eyes, eagles).
song(the_last_resort, eagles).
song(dirty_laundry, eagles).
song(take_it_to_the_limit, eagles).




songVocalist(X, Y):- song(X, Z), vocalist(Y, Z).
songGenre(X, Y) :- song(X, Z), genre(Y, Z).
songLabel(X, Y) :- song(X, Z), label(Y, Z).
songManager(X, Y) :- song(X, Z), manager(Y, Z).

vocalistSong(X,Y) :- vocalist(X, Z), song(Y, Z).
vocalistGenre(X, Y) :- vocalist(X, Z), genre(Y, Z).
vocalistManager(X, Y) :- vocalist(X, Z), manager(Y, Z).
vocalistLabel(X, Y) :- vocalist(X, Z), label(Y, Z).

managerSong(X, Y) :- manager(X, Z), song(Y, Z).
managerGenre(X, Y) :- manager(X, Z), genre(Y, Z).
managerLabel(X, Y) :- manager(X, Z), label(Y, Z).
managerVocalist(X, Y) :- manager(X, Z), vocalist(Y, Z).

genreSong(X, Y) :- genre(X, Z), song(Y, Z).
genreManager(X, Y) :- genre(X, Z), manager(Y, Z).
genreVocalist(X, Y) :- genre(X, Z), vocalist(Y, Z).
genreLabel(X, Y) :- genre(X, Z), label(Y, Z).


isLoaded(X) :- vocalistWorth(Loaded, X), Loaded > 50000000.
bandSingerIsLoaded(Z) :- vocalist(V, Z), isLoaded(V).

