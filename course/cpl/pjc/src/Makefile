prefix = ../bin

CXX=g++
CXXFLAG=-Wall -g
LEXFILE=pscaner.l
YACCFILE=pparser.y
PROG=$(prefix)/core_compiler

.PHONY: all clean test

all: $(PROG)

$(PROG): lex.yy.c y.tab.cpp OneSymbolTableByAVLTree.o emit.o library.o inc.h
	g++ -o $(PROG) y.tab.cpp OneSymbolTableByAVLTree.o emit.o library.o -ll -ly

library.o: library.cpp library.h
	$(CXX) -c library.cpp

OneSymbolTableByAVLTree.o: OneSymbolTableByAVLTree.cpp OneSymbolTableByAVLTree.h
	$(CXX) -c OneSymbolTableByAVLTree.cpp

emit.o: emit.cpp emit.h
	$(CXX) -c emit.cpp

lex.yy.c: $(LEXFILE)
	lex $(LEXFILE)

y.tab.cpp: $(YACCFILE)
	yacc -v -o y.tab.cpp $(YACCFILE)

clean:
	rm -f lex.yy.c y.tab.cpp y.tab.cpp.output *.o $(PROG)
