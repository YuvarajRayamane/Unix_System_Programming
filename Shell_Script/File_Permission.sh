#!/bin/bash

echo -n "Enter a directory name: "
read -r dir_name

echo -n "Enter the permissions in rwxrwxrwx format: "
read -r permissions

var=$(ls -lR "$dir_name")
var2=${var:2}
echo "The files with permissions $permissions in $dir_name are: "
echo "$var2" | grep "$permissions"
