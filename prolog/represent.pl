father(pavlos, giannis).
father(pavlos, georgia).

mother(eleni, maria).
mother(eleni, petros).

parent(X, Y) :- father(X, Y).
parent(X, Y) :- mother(X, Y).

/* X \= Y χρειάζεται για να μην ισχύει sigling(giannis, giannis) */
sibling(X, Y) :- parent(Z,X), parent(Z,Y), X \= Y.


?- sibling(giannis, S)