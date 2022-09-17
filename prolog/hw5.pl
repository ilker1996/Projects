:- module(hw5, [filled_area/2, filling_list/4]).
:- [tiles].

% DO NOT MODIFY THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

tile(t1,3,5,blue).
tile(t2,1,2,blue).
tile(t3,4,3,blue).
tile(t4,2,4,red).
tile(t5,4,1,red).
tile(t6,2,3,green).
tile(t7,1,5,green).
tile(t8,3,3,green).
tile(t9,1,1,purple).
tile(t10,2,5,purple).


area(T,Area):-tile(T,X,Y,_), /* Area of a tile */
			Area is X*Y.


filled_area([],0). 			/* FILLED AREA */
filled_area([H|T],AREA):-filled_area(T,Y),area(H,X),not(member(H,T)),
						AREA is X + Y.

notexceed(0,_,[]).
notexceed(Weight,Height,[Head|Tail]):-tile(Head,X,Y,_),  										 /* Tile that does not exceed given height and weight */ 
									X =< Weight,Y =< Height,Weightrem is Weight - X,
									notexceed(Weightrem,Height,Tail),
									not(member(Head,Tail)).
									
									
filling_list(Weight,Height,Percentage,Tiles):-	notexceed(Weight,Height,Tiles),
												filled_area(Tiles,X),
												 P2 is X / (Weight * Height),P2 >= Percentage.
												
										
