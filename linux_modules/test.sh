#!/bin/bash

make clean

make

sudo insmod hello_mod.ko

sudo rmmod hello_mod.ko

