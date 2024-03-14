#!/bin/sh
clear

if (test -d ./build); then
    rm -r ./build
fi

mkdir ./build

gcc brainfuckInterpreter.c -o brainfuck

mv  brainfuck ./build/brainfuck

./build/brainfuck test.bf -d