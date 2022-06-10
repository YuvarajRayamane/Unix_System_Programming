#!/bin/bash
echo "Enter the string"
read string
echo "Enter start position of the substring"
read POS
echo "Enter the length of the substring"
read LEN
echo "The required substring is : ${string:$POS:$LEN}"
