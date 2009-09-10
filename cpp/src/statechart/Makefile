VERSION=0.9.4

SRC=\
	concurrentstate.cpp \
	context.cpp \
	eventqueue.cpp \
	hierarchicalstate.cpp \
	metadata.cpp \
	pseudostate.cpp \
	statechart.cpp \
	state.cpp \
	timeoutevent.cpp \
	timeouteventqueue.cpp \
	transition.cpp \
	../util/condition.cpp \
	../util/thread.cpp

SRC_HEADERS=\
	action.h \
	concurrentstate.h \
	context.h \
	event.h \
	eventqueue.h \
	finalstate.h \
	guard.h \
	hierarchicalstate.h \
	metadata.h \
	parameter.h \
	pseudostate.h \
	statechartexception.h \
	statechart.h \
	state.h \
	timeoutevent.h \
	timeouteventqueue.h \
	transition.h

UTIL_HEADERS=\
	../util/condition.h \
	../util/mutex.h \
	../util/scopemutex.h \
	../util/smartpointer.h \
	../util/thread.h \
	../util/time.h

HEADERS=$(SRC_HEADERS) $(UTIL_HEADERS)

OBJ = $(SRC:.cpp=.o)

LIB = libstatechart.a

ARFLAGS=crv

CXXFLAGS=-g -I..

all: util $(LIB)
	@echo done

$(LIB): util $(OBJ)
	$(AR) $(ARFLAGS) $(LIB) $(OBJ)

util:
	mkdir -p util
	cd ../util; p=`pwd`; cd ../statechart/util ; for f in $(UTIL_HEADERS) ; do f=$$p/`basename $$f` ; ln -s $$f ; done

clean:
	rm -f $(OBJ) *~ $(LIB) *.o $(BIN1)
	rm -rf util


install: $(LIB)
	mkdir -p $$prefix/lib ; chmod 755 $$prefix/lib
	cp $(LIB) $$prefix/lib
	chmod 444 $$prefix/lib/$(LIB)
	mkdir -p $$prefix/include/statechart ; chmod 755 $$prefix/include/statechart
	for f in $(SRC_HEADERS) ; do cp $$f $$prefix/include/statechart ; f=`basename $$f`; chmod 444 $$prefix/include/statechart/$$f ; done
	mkdir -p $$prefix/include/statechart/util ; chmod 755 $$prefix/include/statechart/util
	for f in $(UTIL_HEADERS) ; do cp $$f $$prefix/include/statechart/util ; f=`basename $$f`; chmod 444 $$prefix/include/statechart/util/$$f ; done
	mkdir -p $$prefix/lib/pkgconfig ; chmod 755 $$prefix/lib/pkgconfig
	sed -e "s;@@PREFIX@@;$$prefix;" -e "s;@@VERSION@@;$(VERSION);" statechart.pc.in >$$prefix/lib/pkgconfig/statechart.pc
	chmod 644 $$prefix/lib/pkgconfig/statechart.pc
