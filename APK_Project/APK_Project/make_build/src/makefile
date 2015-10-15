CC=g++
IDIR = ../include
CFLAGS=-I$(IDIR)

ODIR = obj
LDIR = ../lib

LIBS  = 

_DEPS = Log.h SCUtils.h VehicleDetector.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o IntersectionStateMachine.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

intersection: $(OBJ)
	$(CC) -Wall -g -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
