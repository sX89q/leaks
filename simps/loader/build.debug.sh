#!/bin/bash
gcc -lefence -g -DDEBUG -static -lpthread -pthread -O3 src/*.c -w -o loader.dbg
