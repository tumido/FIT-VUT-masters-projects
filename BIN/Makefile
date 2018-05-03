CPP = g++
CPPFLAGS = -O3 -std=c++14 -Itiny-dnn -DCNN_USE_GEMMLOWP -DDNN_USE_IMAGE_API -pthread -w
NAME = xcoufa09
PROG = lenet

all: $(PROG)

lenet: lenet.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@

.PHONY: clean pack run
run: $(PROG)
	./$^ mnist

clean:
	rm -vf *.o *.tar.gz $(PROG)

pack: clean
	tar -zcvf $(NAME).tar.gz *.cpp *.c *.h *.pdf Makefile
