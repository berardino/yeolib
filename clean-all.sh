#!/bin/bash
to_delete=( CMakeCache.txt CMakeFiles cmake_install.cmake Makefile)
for WORD in "${to_delete[@]}"
do
    find . -name ${WORD} -print0 | xargs -0 rm -rf
done
