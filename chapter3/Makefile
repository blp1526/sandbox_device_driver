CFILES = main.c
obj-m := mydevice.o
mydevice-objs := $(CFILES:.c=.o)

ccflags-y += -std=gnu99 -Wall -Wno-declaration-after-statement

.PHONY: all
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

.PHONY: clean
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
