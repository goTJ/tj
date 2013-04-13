LEXFILE=hwk4.l
YACCFILE=hwk4.y
PROG=hwk4

.PHONY: all clean test

all: $(PROG)

$(PROG): lex.yy.c y.tab.cpp
	g++ -o $(PROG) y.tab.cpp -ll -ly

lex.yy.c: $(LEXFILE)
	lex $(LEXFILE)

y.tab.cpp: $(YACCFILE)
	yacc -v -o y.tab.cpp $(YACCFILE)

clean:
	rm -f $(PROG) lex.yy.c y.tab.cpp y.tab.cpp.output

test: all
	./hwk4
