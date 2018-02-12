CC = g++
FILES = utils.cpp ROPHelper.cpp main.cpp BaseGadgets.cpp GadgetHelper.cpp ROPGen.cpp  ROPStringHelper.cpp payloads.cpp FctCallHelper.cpp
OUT_EXE = rop_generator
CFLAGS = -Wall -O2 # -g

build: $(FILES)
		$(CC) $(CFLAGS) -o $(OUT_EXE) $(FILES)


clean:
		rm -f *.o core

re: clean build

tar:
		tar -cjvf $(OUT_EXE).tar.bz2 *.cpp *.h Makefile gadgets/*.txt

all: build ; cp $(OUT_EXE) ./gadgets/gadgets_PLATFORM1.txt gadgets/gadgets_PLATFORM2.txt ../
