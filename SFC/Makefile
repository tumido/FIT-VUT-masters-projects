CPP = g++
CPPFLAGS = -std=c++11 -Wall -Werror -pedantic
ZIP_NAME = xcoufa09.zip
PROG = rbm

all: $(PROG)

$(PROG): $(PROG).o main.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

.PHONY: clean zip run

run: $(PROG)
	./$^

clean:
	rm -vf *.o *.zip $(PROG)

zip:
	zip $(ZIP_NAME) *.cpp *.h *.pdf Makefile
