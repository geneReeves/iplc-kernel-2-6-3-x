#
# Makefile for lj
#
#

KDIR := /lib/modules/$(shell uname -r)/build 

obj-m := lj.o

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	rm -rf *.mod.c *.o *~ .*cmd .tmp_versions

clean:
	rm -rf *.mod.c *.o *.ko *~ .*cmd .tmp_versions

