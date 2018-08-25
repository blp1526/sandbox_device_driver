CFILES = main.c
obj-m := device.o
device-objs := $(CFILES:.c=.o)

.PHONY: all
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

.PHONY: clean
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
