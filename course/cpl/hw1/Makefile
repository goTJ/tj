LEXFILE=hwk1.l
PROG=hwk1

.PHONY: all clean test

all: $(PROG)

$(PROG): $(LEXFILE)
	lex $(LEXFILE)
	gcc -Wall -o $(PROG) lex.yy.c -ll
	rm -f lex.yy.c

clean:
	rm -f $(PROG) lex.yy.c

test: all
	bash test.sh ./$(PROG)
