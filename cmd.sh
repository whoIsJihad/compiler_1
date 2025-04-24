#! /usr/bin/bash

g++ -fsanitize=address -g offline.cpp -o offline
 ./offline input.txt output.txt SDMBHash