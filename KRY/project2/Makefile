CPP = g++
CPPFLAGS = -std=c++11 -pedantic -Wall -Werror -lcrypto -lssl
NAME = xcoufa09
PROG = validate_certs

all: run

validate_certs: validate_certs.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@

.PHONY: clean pack run csv
run: validate_certs
	./$^

csv: validate_certs
	./$^ > $(NAME)-domains.csv

clean:
	rm -vf *.o *.tar.gz $(PROG)

pack: clean csv
	tar -zcvf $(NAME)-source-code.tar.gz *.cpp Makefile
	tar -zcvf $(NAME).tar.gz $(NAME)-source-code.tar.gz $(NAME)-domains.csv $(NAME)-report.pdf
