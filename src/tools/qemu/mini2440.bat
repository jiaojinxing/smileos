start putty.exe
start qemu-system-arm.exe -M mini2440 -kernel 2440.elf -show-cursor -sd SDCARD -serial telnet:127.0.0.1:1200,server