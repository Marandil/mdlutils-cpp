#!/bin/bash

URL=https://github.com/google/googletest/archive/master.zip

if [ -z `ls googletest-master` ]; then
    wget $URL
    unzip master
fi