father(giannis, pavlos)
father(giannis, georgia)

mother(eleni, maria)
mother(eleni, petros)

/* X \= Y χρειάζεται για να μην ισχύει sigling(giannis, giannis) */
sibling(X, Y) :- father(Somebody,X), father(Somebody,Y), X \= Y.
sibling(X, Y) :- father(Somebody,X), mother(Somebody,Y), X \= Y.
sibling(X, Y) :- mother(Somebody,X), father(Somebody,Y), X \= Y.
sibling(X, Y) :- mother(Somebody,X), mother(Somebody,Y), X \= Y.

?- sibling(giannis, S)