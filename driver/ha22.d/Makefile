#
# Makefile for iplc
#
#

NOM	= ha22

KDIR := /lib/modules/$(shell uname -r)/build 
myPWD := $(shell pwd )

# Use this to see all the commands
export KBUILD_VERBOSE=1

obj-m := ha22.o

# ----------------------------------------------------------------------------
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(myPWD) modules
	rm -rf *.mod.c *.o *~ .*cmd .tmp_versions

clean:
	rm -rf *.mod.c *.o *.ko *~ .*cmd .tmp_versions

install: prereq_check gen_exports default stack_check
	echo $(INST_DIR)
	mkdir -p $(INST_DIR)
	install -m 0644 $(MODULE) $(INST_DIR)
	-@/bin/rm -rf $(SRC_DIR)/.tmp_versions
ifndef DIST_DESTDIR
	-/sbin/depmod -a $(KVERS) -b $(DESTDIR)
endif

# modules: $(vmlinux-dirs) $(if $(KBUILD_BUILTIN),vmlinux)
#        @echo '  Building modules, stage 2.';
#        $(MAKE) -rR -f $(srctree)/scripts/Makefile.modpost

