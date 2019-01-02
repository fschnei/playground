# linux_modules

I followed this [tutorial] (https://wiki.ntb.ch/infoportal/_media/software/linux/treiber_entwicklung/linuxtreiberentwicklung_v1.3.pdf)

My machine: Ubuntu 18.10 in virtual box 6.0

setup: 

sudo apt-get install build-essential libelf-dev



build module:
make modules

load own module:
sudo insmod hello_mod.ko

check if load was successfully:
lsmod

check kernel log:
dmesg

unload own module:
sudo rmmod hello_mod

check kernel log:
`<dmesg>`
