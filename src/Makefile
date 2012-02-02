TODEL	     = tags *~ .*.swp *.o *~ core .depend .*.cmd *.ko *.mod.c modules.order Module.symvers
RM	     = rm
INSTALL	     = install
DEPMOD       = depmod
RMFLAGS      = -rf
SOURCES      = usbwall_mod.c \
	       procfs_iface.c \
	       usbwall_chrdev.c \
	       keylist.c \
	       trace.c

OBJS         = $(SOURCES:.c=.o)

ifeq ($(KERNELRELEASE),)
     SYSVERSION := $(shell uname -r)
     KERNELDIR ?= /lib/modules/$(SYSVERSION)/build
     PWD := $(shell pwd)

all : modules

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=x86 modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	$(INSTALL) -m 0644 usbwall.h /usr/include/linux/usb
	$(DEPMOD) -a

clean:
	$(RM) $(RMFLAGS) $(OBJS) $(TODEL)

.PHONY: modules modules_install clean

else

EXTRA_CFLAGS += -ggdb

obj-m := usbwall.o
usbwall-objs := $(OBJS)

endif
