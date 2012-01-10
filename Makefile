#
# Makefile for Project usbwall in .
#
# Made by Philippe Thierry
# Login phil  <phil@reseau-libre.net>
#
# Started on  Thu Jul 10 10:27:41 2011 Philippe Thierry
#

SRCDIR	= src
DOCDIR	= doc
TESTDIR	= test
DATE = $(shell date "+%Y-%m-%d")
MAKE = make

all :
	$(MAKE) -C src all

modules: all

modules_install: modules
	$(MAKE) -C src modules_install

doc :
	(cd doc; make all)

check :
	(cd test; make all)

clean : 
	(cd src && make clean)

distclean :
	(cd src && make distclean)

export: distclean
	(git export . /tmp/module-$(DATE) && cd /tmp && tar -cjvf module-$(DATE).tar.bz2 module-$(DATE) && cd - && rm -rf /tmp/module-$(DATE))
