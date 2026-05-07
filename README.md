# basic-kernel

build
```bash
make
```

run qemu
```bash
make run
```

or
```bash
make run-graphic
```
or
```bash
make hardware-image
```

`build/hardware-boot.iso`. Write this file to USB:

```bash
sudo dd if=build/hardware-boot.iso of=/dev/sdX bs=4M status=progress oflag=sync
```

Replace `/dev/sdX` with your USB device.

clean:
```bash
make clean
```
