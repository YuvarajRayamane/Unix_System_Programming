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


#space sensitive
#chmod u+x 1.sh
#./1.sh
#./1.sh 1.sh   (Regular file)
#./1.sh /dev  ==(directory file)
#./1.sh /dev/null  ==(Character device file)
#./1.sh /dev/stderr  ==(Character device file)
#./1.sh /dev/loop1  ==(Block device file)
# ln -s file.txt file_name.txt  (creates symbolic link)
#./1.sh file_name.txt  ==(Symbolic link file)
