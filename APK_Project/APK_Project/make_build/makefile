#Compile flags and substitution variables
CC=g++
IDIR = include
CFLAGS=-I$(IDIR) -std=c++11 -Wall
MKDIR_P = mkdir -p

#Required directories
OUT_DIR = obj

#Directories and search paths
ODIR = obj
LDIR = lib
VPATH = src

#Other files to be deleted on clean
OTHERFILES = output.txt

LIBS  = -lpthread

#Dependencies and objects setup
_DEPS = Log.h SCUtils.h VehicleDetection.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o IntersectionStateMachine.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#Compile all .cpp
$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#Link main target object files
intersection: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean directories

#Run all targets in order
all: directories intersection clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(OTHERFILES)

directories: $(REQ_DIR)
	$(MKDIR_P) $(OUT_DIR)
