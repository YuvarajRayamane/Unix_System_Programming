#!/bin/bash

if [ -c $1 ]      #give file name in command line. Ex:./Prints_type_of_file.sh file_name
then
echo -e "\n$1 Character device file. \c"

elif [ -d $1 ]
then
echo -e "\n$1 Directory file. \c"

elif [ -b $1 ]
then
echo -e "\n$1 Block device file. \c"

elif [ -L $1 ]
then
echo -e "\n$1 Symbolic link file. \c"

elif [ -f $1 ]
then
echo -e "\n$1 Regular File. \n\c"

else
echo -e "\n$1 Invalid file."

fi
