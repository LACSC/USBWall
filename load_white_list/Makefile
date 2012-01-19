#
# Makefile for Project usbwall in .
#
# Made by David Fernandes
# Login   <dfernand@ece.fr>
#
# Started on  Tue Jan  10 14:49:06 2012 David FERNANDES
# 

#############################################################################
## basic macros
#############################################################################
CC           = gcc
AR           = ar
CFLAGS       = -Wall -W -Werror
LD           = ld
LDFLAGS      = 
ARFLAGS      = cr
RM           = rm
RMFLAGS      = -rf
TAR          = tar
TARFLAGS     = -cjvf
MAKE        ?= make
INSTALL      = install
CTAGS        = ctags -x > tags
DEPEND       = makedepend $(CFLAGS)
MAKE         = make

#############################################################################
## lint -- static code mistakes detector
#############################################################################
LINT         = splint
LARCH_PATH   = /usr/local/lib
LCLIMPORTDIR = /usr/local/share/splint/imports

#############################################################################
## dist target file
#############################################################################
DISTTARGET   = .tar.bz2

#############################################################################
## project's source and generated files
#############################################################################
TARGET       = load_white_list
SOURCE       =  load_white_list.c

OBJS         = $(SOURCE:.c=.o)
HEADERS      = $(SOURCE:.c=.h)
TODEL        = tags *~ .*.swp

#############################################################################
## rules
#############################################################################

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.c.o :
	$(CC) $(CFLAGS) -c $<

doc :
	$(MAKE) -C doc all

lint :
	$(LINT) $(CFLAGS) $(SOURCE)

tags : $(SOURCE)
	$(CTAGS) $(SOURCE)

depend : $(SOURCE)
	$(DEPEND) $(SOURCE)

.PHONY : clean doc

clean : 
	$(RM) $(RMFLAGS) $(OBJS) $(TODEL)

distclean : clean
	$(RM) $(RMFLAGS) $(TARGET)

dist : distclean
	$(TAR) $(TARFLAGS) $(DISTTARGET) .

check : $(TARGET)
	cd check; $(MAKE) all

install : $(TARGET)
	@echo you must be root to install
