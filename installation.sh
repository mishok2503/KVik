#!/bin/bash

mkdir libs && cd libs

sudo git clone git@github.com:facebook/zstd.git
sudo git clone git@github.com:lz4/lz4.git

cd zstd/lib

make && sudo make install

cd ../..

cd lz4

make && sudo make install
