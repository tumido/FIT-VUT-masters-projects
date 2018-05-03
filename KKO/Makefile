CPP = g++
CPPFLAGS = -std=c++11
ZIP_NAME = kko.proj2.xcoufa09.zip
PROG = bwted
BUILD_DIR = build
SOURCE_DIR = blocks
OBJS = $(BUILD_DIR)/btree.o $(BUILD_DIR)/mtf.o $(BUILD_DIR)/bwt.o $(BUILD_DIR)/rle.o $(BUILD_DIR)/shc.o bwted.o

all: $(BUILD_DIR) $(PROG)

$(PROG): main.cpp $(OBJS)
	$(CPP) $(CPPFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

.PHONY: encode decode zip clean $(BUILD_DIR)
.SILENT: encode decode

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

encode: $(PROG)
	./$^ -c -l /tmp/bwted.log -o /tmp/bwted.encoded
	echo -e "\n\nLOG display"
	cat /tmp/bwted.log

decode: $(PROG)
	./$^ -x -l /tmp/bwted.log -i /tmp/bwted.encoded
	echo -e "\n\nLOG display"
	cat /tmp/bwted.log

clean:
	rm -rvf $(BUILD_DIR) *.o *.zip $(PROG)

zip:
	zip $(ZIP_NAME) *.cpp *.hpp blocks/*.cpp blocks/*.hpp *.pdf Makefile
