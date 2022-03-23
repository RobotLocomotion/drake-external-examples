#!/bin/bash

ROOT_DIR=$(git rev-parse --show-toplevel)

HEADER_FILE=$ROOT_DIR/scripts/setup/utils/header.txt
FILE_NAME=$1

first_line=$(head -n 1 $FILE_NAME)

if [[ $first_line == \#!* ]]
then
    echo $first_line
    cat $HEADER_FILE
    tail -n +2 $FILE_NAME
else
    cat $HEADER_FILE $FILE_NAME
fi
