CPP = g++
CPPFLAGS = -std=c++11
ZIP_NAME = kko.proj2.xcoufa09.zip
PROG = bwted

all: $(PROG)

$(PROG): $(PROG).o btree.o main.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

.PHONY: encode decode zip clean
.SILENT: encode decode

encode: $(PROG)
	./$^ -c -l /tmp/bwted.log -o /tmp/bwted.encoded
	echo -e "\n\nLOG display"
	cat /tmp/bwted.log

decode: $(PROG)
	./$^ -x -l /tmp/bwted.log -i /tmp/bwted.encoded
	echo -e "\n\nLOG display"
	cat /tmp/bwted.log

clean:
	rm -vf *.o *.zip $(PROG)

zip:
	zip $(ZIP_NAME) *.cpp *.h *.pdf Makefile
