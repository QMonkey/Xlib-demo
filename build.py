#!/usr/bin/env python3

import sys;
import os;

binary = sys.argv[1];
source = binary + ".c";
os.system("gcc -g -o build/" + binary + " src/" + source + " -lX11");