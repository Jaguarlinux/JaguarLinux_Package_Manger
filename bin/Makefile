-include ../config.mk

SUBDIRS +=	ps4-alternatives
SUBDIRS +=	ps4-create
SUBDIRS +=	ps4-dgraph
SUBDIRS +=	ps4-install
SUBDIRS +=	ps4-pkgdb
SUBDIRS +=	ps4-query
SUBDIRS +=	ps4-reconfigure
SUBDIRS +=	ps4-remove
SUBDIRS +=	ps4-rindex
SUBDIRS +=	ps4-uhelper
SUBDIRS +=	ps4-checkvers
SUBDIRS +=	ps4-fbulk
SUBDIRS +=	ps4-digest
SUBDIRS +=	ps4-fetch

ifeq (${PS4_OS},linux)
SUBDIRS +=	ps4-uchroot
SUBDIRS +=	ps4-uunshare
endif

include ../mk/subdir.mk
