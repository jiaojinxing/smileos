start qemu-system-arm.exe -M mini2440 -kernel SmileOS.elf -show-cursor -serial telnet:127.0.0.1:1200,server -net nic -net tap,ifname=virtual -mtdblock nand.bin -sd SDCARD.ima -serial file:virtualkbd
start putty.exe telnet://127.0.0.1:1200/
pause
start putty.exe telnet://192.168.7.30:23/