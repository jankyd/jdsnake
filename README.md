# JD Snake
A simple snake game to practice using ncurses as well as c++.

## 'Features'
* A pre-determined gridsize centered in the terminal window.
* A snake!
* Food!
* The snake grows when it eats the food!

## Known bugs

### NOTE: I may or may not address these. This is just a little practice project, so I am not trying to make it perfect.
* Any keypress is considered an input, so game turns will be updated even if control keys are not pressed
* Will probably hang while trying to place food with an extremely long snake as there is no handling other than index randomization for placing food.
* ~~Has memory leaks (pending valgrind tests)~~ Fixed :)