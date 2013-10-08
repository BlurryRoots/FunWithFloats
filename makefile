SOURCE = src
BINARIES = bin

ifeq ($(OS),Windows_NT)
	MINGW = %MINGW%
	COMPILER = $(MINGW)/bin/g++
	INCLUDES = -I$(MINGW)/include
	OUTFILE = main.exe
	CHMOD =
	RUN = $(BINARIES)/$(OUTFILE)
else
	COMPILER = g++
	INCLUDES = -I$(SOURCE)
	OUTFILE = main
	CHMOD = chmod +x $(BINARIES)/$(OUTFILE)
	RUN = $(BINARIES)/./$(OUTFILE)
endif

CFLAGS = -g -Wall -std=c++11

clean:
	rm -rf $(BINARIES)
	mkdir $(BINARIES)

build:
	$(COMPILER) $(CFLAGS) $(INCLUDES) -c $(SOURCE)/main.cpp -o $(BINARIES)/main.o
	$(COMPILER) -I$(INCLUDES) $(BINARIES)/main.o -o $(BINARIES)/$(OUTFILE)
	$(CHMOD)

run:
	$(RUN)
