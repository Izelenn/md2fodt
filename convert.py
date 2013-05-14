#!/usr/bin/python

from sys import argv, exit
from os import system

if (len(argv) != 2):
	print("Usage : convert.py <filename>")
	exit(1)

mdfile = argv[1]
if (mdfile.count(".md") == 0):
	mdfile += ".md"
fodtfile = mdfile.replace(".md", ".fodt")

system("./md2fodt " + mdfile + " " + fodtfile)
system("libreoffice4.0 --headless --convert-to pdf " + fodtfile)

