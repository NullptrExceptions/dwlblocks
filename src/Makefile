PREFIX  := /usr/local
CC      := cc
CFLAGS  := -pedantic -Wall -Wno-deprecated-declarations -Os

all: options dwlblocks

options:
	@echo dwlblocks build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "CC      = ${CC}"

dwlblocks: dwlblocks.c blocks.def.h blocks.h
	${CC} -o dwlblocks dwlblocks.c ${CFLAGS}

blocks.h:
	cp blocks.def.h $@

clean:
	rm -f *.o *.gch dwlblocks

install: dwlblocks
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwlblocks ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwlblocks

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwlblocks

.PHONY: all options clean install uninstall
