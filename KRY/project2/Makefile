CPP = g++
CPPFLAGS = -std=c++11 -pedantic -Wall -Werror -lcrypto -lssl
NAME = xcoufa09
PROG = validate_certs

all: validate_certs

validate_certs: validate_certs.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@

.PHONY: clean pack run

run: validate_certs
	./$^

clean:
	rm -vf *.o *.tar.gz $(PROG)

pack:
	tar -zcvf $(NAME)-source-code.tar.gz *.cpp Makefile
	tar -zcvf $(NAME).tar.gz $(NAME)-source-code.tar.gz $(NAME)-domains.csv $(NAME)-report.pdf
