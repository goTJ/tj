.PHONY: clean

%.pdf: %.cjk
	pdflatex $?

%.cjk: %.tex
	bg5conv < $? > $@

clean:
	rm -f *.cjk *.aux *.log *.toc
