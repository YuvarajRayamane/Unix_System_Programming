#!/bin/bash

echo -n "Enter a string: ";
read -r string;

echo -n "Enter the starting index: "
read -r start

echo -n "Enter the ending index: "
read -r end

substring=$(echo "$string" | cut -c "$start"-"$end")
echo "The substring is \"$substring\""
