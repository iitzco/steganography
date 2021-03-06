#!/bin/bash

set -e

green=$(tput setaf 2)
normal=$(tput sgr0)

ALGS=(aes128 aes192 aes256 des)
MODES=(ecb cfb ofb cbc)

echo "Running tests..."

echo "file without encryption LSB1"
./stegowav -p samples/fun.wav -i samples/avatar.png -o test/steg.wav -s LSB1 -e
./stegowav -p test/steg.wav -o test/out -s LSB1 -x
cmp samples/avatar.png test/out.png

echo "file without encryption LSB4"
./stegowav -p samples/fun.wav -i samples/avatar.png -o test/steg.wav -s LSB4 -e
./stegowav -p test/steg.wav -o test/out -s LSB4 -x
cmp samples/avatar.png test/out.png

echo "file without encryption LSBE"
./stegowav -p samples/fun.wav -i samples/icon.png -o test/steg.wav -s LSBE -e
./stegowav -p test/steg.wav -o test/out -s LSBE -x
cmp samples/icon.png test/out.png

for ALG in ${ALGS[*]}; do
  for MODE in ${MODES[*]}; do
    echo "file with $ALG encryption in $MODE mode"
    ./stegowav -p samples/fun.wav -i samples/avatar.png -o test/steg.wav -s LSB1 -a $ALG -m $MODE -e --pass world
    ./stegowav -p test/steg.wav -o test/out -s LSB1 -a $ALG -m $MODE -x --pass world
    cmp samples/avatar.png test/out.png
  done
done

echo "${green}All tests passed.${normal}"
