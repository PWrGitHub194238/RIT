#!/bin/bash

bash toggleLogs.bash
bash build.bash 'debug'
bash memcheck.bash -r './RIT_Library' -s 'valgrind.supp' -o 'valgrind.log'
bash toggleLogs.bash
