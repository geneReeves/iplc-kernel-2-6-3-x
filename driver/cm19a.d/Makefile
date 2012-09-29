#
# Makefile for cm19a
#
#

KDIR := /lib/modules/$(shell uname -r)/build 

obj-m := cm19a.o

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	rm -rf *.mod.c *.o *~ .*cmd .tmp_versions

clean:
	rm -rf *.mod.c *.o *.ko *~ .*cmd .tmp_versions

