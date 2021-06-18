#!/bin/bash

while true
do
    gcc main.c util.c && ./a.out 61 2 200 200 | grep "die"
done