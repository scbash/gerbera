#!/bin/bash
if ! [ "$(id -u)" = 0 ]; then
    echo "Please run this script with superuser access!"
    exit 1
fi
set -ex
wget https://github.com/scbash/taglib/tarball/topic/multi-tag-separator -O taglib-1.11.1-multi.tgz
tar -xzvf taglib-1.11.1-multi.tgz
mkdir taglib-build
cd taglib-build
if [[ $CXX == clang++* ]]; then
	cmake -DCMAKE_CXX_FLAGS="-stdlib=libc++" ../scbash-taglib-fc5ac34
else
	cmake ../scbash-taglib-fc5ac34
fi
make && make install && ldconfig
