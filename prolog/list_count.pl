size([], 0).
size([H|T], L) :- size(T, PREV_L), L is PREV_L+1.

?- length_1([a,a,b], X).