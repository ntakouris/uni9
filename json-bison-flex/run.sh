rm parser
rm lex.yy.c y.tab.c y.tab.h lex.yy.o y.tab.o
bison -t -y -d calc.y 
flex -d calc.l
gcc -c y.tab.c lex.yy.c
gcc y.tab.o lex.yy.o -o parser
rm output
./parser tweets.json
cat output
