f(X,Y,_):- assert(g(X,Y)), fail.
f(_,_,Y):- retract(g(X,Z)), Y is X+Z.

?- f(11,8,X).

/* Απάντηση = 19 */