#!/bin/bash

if [ ! -e "$1" ]; then  #input of file given as command. Ex: ./2a.sh file_name
  echo "File $1 does not exist"
  exit
fi

line_count=$(grep "" -c "$1")
echo "There are $line_count lines in $1"
