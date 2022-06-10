#!/bib/bash
echo "Enter the file name."
read file_name
echo "The file contains" $(cat $file_name | wc -l)" lines."
