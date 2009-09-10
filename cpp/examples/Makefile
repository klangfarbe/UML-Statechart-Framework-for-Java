CXXFLAGS=-g `pkg-config --cflags statechart`
LDFLAGS=-g  `pkg-config --libs statechart`
LIBS=

BIN1=telephone
OBJS1=telephone.o

all: $(BIN1)
	@echo done

$(BIN1):	$(OBJS1)
	$(CXX) -o $(BIN1) $(OBJS1) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN1)
