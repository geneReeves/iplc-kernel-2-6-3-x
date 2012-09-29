#
# Makefile for iplc
#
#
KDIR := /lib/modules/$(shell uname -r)/build/
myPWD := $(shell pwd )

#obj-m := iplc.o

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(myPWD)/iplc.d modules

# HA22 requires a completely different driver.
#ALL: iplc cm15a cm19a lj ha22
all: iplc cm15a cm19a lj

iplc:
	$(MAKE) -C $(KDIR) SUBDIRS=$(myPWD)/iplc.d modules

cm15a:
	$(MAKE) -C $(KDIR) SUBDIRS=$(myPWD)/cm15a.d modules

cm19a:
	$(MAKE) -C $(KDIR) SUBDIRS=$(myPWD)/cm19a.d modules

lj:
	$(MAKE) -C $(KDIR) SUBDIRS=$(myPWD)/lj.d modules

ha22:
	$(MAKE) -C $(KDIR) SUBDIRS=$(myPWD)/ha22.d modules

clean:
	rm -rf *.mod.c *.o *.ko *~ .*cmd .tmp_versions
	(cd iplc.d  ; rm -rf *.mod.c *.o *.ko *~ .*cmd .tmp_versions)
	(cd cm15a.d ; rm -rf *.mod.c *.o *.ko *~ .*cmd .tmp_versions)
	(cd cm19a.d ; rm -rf *.mod.c *.o *.ko *~ .*cmd .tmp_versions)
	(cd lj.d    ; rm -rf *.mod.c *.o *.ko *~ .*cmd .tmp_versions)
	(cd ha22.d  ; rm -rf *.mod.c *.o *.ko *~ .*cmd .tmp_versions)
