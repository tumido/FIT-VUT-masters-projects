FILE=homework

all:
	latex $(FILE).tex
	dvips -t a4 $(FILE).dvi
	ps2pdf $(FILE).ps

clean:
	rm -f *.ps *.log *.dvi *.aux *.out *.zip $(FILE).pdf
