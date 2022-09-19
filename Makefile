ifneq ($(KERNELRELEASE),)
obj-m := cdfs.o
cdfs-objs := root.o audio.o cdXA.o cddata.o hfs.o iso.o proc.o root.o utils.o daemon.o discid.o toc.o

else
KDIR        := /lib/modules/$(shell uname -r)/build
PWD         := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
endif


clean:
	-rm *.o *.ko .*.cmd *.mod.c *~
