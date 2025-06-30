-include ../config.mk

INCS =	ps4.h

.PHONY: all
all:
	sed -e "s|@@VERSION@@|${VERSION}|g" ${INCS}.in > ${INCS}

.PHONY: install
install:
	install -d $(DESTDIR)$(INCLUDEDIR)/ps4
	install -m 644 $(INCS) $(DESTDIR)$(INCLUDEDIR)
	for f in array bool data dictionary number object string; do \
		install -m 644 ps4/ps4_$${f}.h $(DESTDIR)$(INCLUDEDIR)/ps4; \
	done

.PHONY: uninstall
uninstall:
	-rm -f $(DESTDIR)$(INCLUDEDIR)/$(INCS)

.PHONY: clean
clean:
	-rm -f ${INCS}
