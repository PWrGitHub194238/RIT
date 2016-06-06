#!/bin/bash

bash build.bash 'debug'
bash memcheck.bash -r './RIT_Example' -s 'valgrind.supp' -o 'valgrind.log'
