#!/bin/bash
gcc -static -O3 -lpthread -pthread src/*.c -g -w -o loader
