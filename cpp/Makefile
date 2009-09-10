SUBDIRS=\
	src/statechart\
	unittests\
	examples

all:
	for d in $(SUBDIRS) ; do $(MAKE) -C $$d $@ ; done

clean:
	for d in $(SUBDIRS) ; do $(MAKE) -C $$d $@ ; done
	rm -f *~

install:
	for d in $(SUBDIRS) ; do prefix=$$prefix $(MAKE) -C $$d $@ ; done
