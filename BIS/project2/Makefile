PROG = antispam
ZIP_NAME = xcoufa09.zip
REQUIRE = requirements.txt

all:
	pip3 install --user -r $(REQUIRE)
	chmod +x $(PROG)

.PHONY: zip run

run: $(PROG)
	./$^

zip:
	zip $(ZIP_NAME) $(PROG) $(REQUIRE) *.py stop_word_*.txt *.gz *.pdf Makefile
