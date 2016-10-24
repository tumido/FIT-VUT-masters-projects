FILE=homework

all:
	dot -Tps img/exercise_3.gv -o exercise_3.ps
	dot -Tps img/exercise_2_tree.gv -o exercise_2_tree.ps
	dot -Tps img/exercise_2_rka.gv -o exercise_2_rka.ps
	dot -Tps img/exercise_2_dka.gv -o exercise_2_dka.ps
	dot -Tps img/exercise_2_rdka.gv -o exercise_2_rdka.ps
	latex $(FILE).tex
	dvips -t a4 $(FILE).dvi
	ps2pdf $(FILE).ps

clean:
	rm -f *.ps *.log *.dvi *.aux *.out *.zip $(FILE).pdf
