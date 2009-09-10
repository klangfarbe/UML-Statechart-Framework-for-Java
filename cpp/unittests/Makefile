VERSION=0.9.4

SRC=\
	runner.cpp

OBJ = $(SRC:.cpp=.o)

CXXFLAGS=-g -I../src -I.
LDFLAGS=-g -L../src/statechart/ -lstatechart -lpthread
BIN=./runner

all: test
	@echo done

test:	$(BIN)
	$(BIN)

$(BIN): $(OBJ)
	$(CXX) -o $(BIN) $(OBJ) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)

clean:
	rm -f $(OBJ) *~ $(BIN)

install:
	@echo done
