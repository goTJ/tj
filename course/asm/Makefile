%: %.o
	gcc $? -o $@
	rm -f $?

%.o: %.asm
	nasm -f elf $?

%.o: %.c
	gcc -c j.c

%.pdf: %.cjk
	pdflatex $?

%.cjk: %.tex
	bg5conv < $? > $@

clean:
	rm -f *.cjk *.aux *.log
