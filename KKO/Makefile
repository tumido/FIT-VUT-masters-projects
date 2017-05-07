CPP = g++
CPPFLAGS = -std=c++11 -Wall
ZIP_NAME = kko.proj3.xcoufa09.zip
PROG = gif2bmp

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