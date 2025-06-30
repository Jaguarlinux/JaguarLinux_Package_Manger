-include ../config.mk

PCFILE = libps4.pc

.PHONY: all
all:
	sed -e "s|@@VERSION@@|${VERSION}|g" \
		-e "s|@@PREFIX@@|${PREFIX}|g" \
		-e "s|@@EPREFIX@@|${EPREFIX}|g" \
		${PCFILE}.in > ${PCFILE}

.PHONY: install
install:
	install -d $(DESTDIR)$(MANDIR)/man5
	install -m644 ps4.d.5 $(DESTDIR)$(MANDIR)/man5
	install -d $(DESTDIR)$(MANDIR)/man7
	install -m644 ps4.7 $(DESTDIR)$(MANDIR)/man7
	install -d $(DESTDIR)$(PKGCONFIGDIR)
	install -m644 $(PCFILE) $(DESTDIR)$(PKGCONFIGDIR)
	install -d $(DESTDIR)/$(DBDIR)/keys
	install -m644 *.plist $(DESTDIR)/$(DBDIR)/keys
	install -d $(DESTDIR)/$(SHAREDIR)/ps4.d
	install -m644 repod-main.conf $(DESTDIR)/$(SHAREDIR)/ps4.d/00-repository-main.conf
	install -m644 ps4.conf $(DESTDIR)/$(SHAREDIR)/ps4.d/ps4.conf
	install -d $(DESTDIR)/$(SHAREDIR)/zsh/site-functions
	install -m644 _ps4* $(DESTDIR)/$(SHAREDIR)/zsh/site-functions
	install -d $(DESTDIR)/$(SHAREDIR)/bash-completion/completions
	install -m644 ps4.bash $(DESTDIR)/$(SHAREDIR)/bash-completion/completions/ps4
	for file in ps4-checkvers ps4-create ps4-dgraph ps4-install \
		ps4-pkgdb ps4-query ps4-reconfigure ps4-remove ps4-rindex; do \
		ln -sf ps4 $(DESTDIR)/$(SHAREDIR)/bash-completion/completions/$$file; \
	done

.PHONY: uninstall
uninstall:
	-rm -f $(DESTDIR)$(PKGCONFIGDIR)/$(PCFILE)
	-rm -f $(DESTDIR)$(SHAREDIR)/ps4.d/00-repository-main.conf
	-rm -f $(DESTDIR)$(SHAREDIR)/ps4.d/ps4.conf


.PHONY: clean
clean:
	-rm -f $(PCFILE)
