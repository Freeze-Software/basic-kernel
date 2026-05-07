CC := gcc
AS := gcc
LD := ld

CFLAGS := -m32 -ffreestanding -fno-pie -fno-stack-protector -Wall -Wextra -O2
ASFLAGS := -m32 -c
LDFLAGS := -m elf_i386 -T linker.ld -nostdlib

BUILD_DIR := build
ISO_DIR := $(BUILD_DIR)/iso
KERNEL := $(BUILD_DIR)/kernel.bin
ISO := $(BUILD_DIR)/kernel.iso

OBJS := \
	$(BUILD_DIR)/boot.o \
	$(BUILD_DIR)/kernel.o

.PHONY: all clean run run-graphic run-nographic hardware-image

all: $(ISO)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: src/boot.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/kernel.o: src/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(ISO): $(KERNEL) grub/grub.cfg
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.bin
	cp grub/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

run: run-graphic

run-graphic: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

run-nographic: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -display none -serial stdio

hardware-image: $(ISO)
	cp $(ISO) $(BUILD_DIR)/hardware-boot.iso
	@echo "Created $(BUILD_DIR)/hardware-boot.iso"

clean:
	rm -rf $(BUILD_DIR)
