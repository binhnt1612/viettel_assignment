obj-m += i2c-light-sensor.o

KERNEL_DIR ?= $(HOME)/Workplace/viettel_assignment/linux-fslc-4.1

all:
	make -C $(KERNEL_DIR) \
		ARCH=arm CROSS_COMPILE=arm-poky-linux-gnueabi- \
		SUBDIRS=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) \
		ARCH=arm CROSS_COMPILE=arm-poky-linux-gnueabi- \
		SUBDIRS=$(PWD) clean