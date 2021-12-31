#!/usr/bin/python
# -*- coding: utf-8

import sys, sys, os, re, binascii

binarys = [
	"dlr.x86_64:x86_64.hex",
	"dlr.i586:i586.hex",
	"dlr.mips:mips.hex",
	"dlr.mipsel:mipsel.hex",
	"dlr.armv4l:armv4l.hex",
	"dlr.armv5l:armv5l.hex",
	"dlr.armv6l:armv6l.hex",
	"dlr.armv7l:armv7l.hex",
	"dlr.powerpc:powerpc.hex",
	"dlr.powerpc-440fp:powerpc-440fp.hex",
	"dlr.sparc:sparc.hex",
	"dlr.m68k:m68k.hex",
	"dlr.i686:i686.hex",
	"dlr.sh4:sh4.hex"
	]

def insert_slashx(string, every=2):
    return '\\x'.join(string[i:i+every] for i in xrange(0, len(string), every))

def splitCount(s, count):
     return [''.join(x) for x in zip(*[list(s[z::count]) for z in range(count)])]

for bin in binarys:
	try:
		Input = bin.split(":")[0]
		Output = bin.split(":")[1]
		hexify = '\\x'+insert_slashx(binascii.hexlify(open(Input).read()))
		echoify = "echo -en \'"+'\' >>lno\necho -en \''.join(splitCount(hexify,(64*2))) + '\' >>lno'
		f = open(Output, "w")
		f.write(echoify)
		f.close()
	except:
		pass