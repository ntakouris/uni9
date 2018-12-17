length([], 0).
length([H|T], L) :- length(T, PREV_L), L is PREV_L+1.

?- length_1([a,a,b], X), LEN is X.