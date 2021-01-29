#!/bin/bash
direc=`pwd`
rm -rf ./FileList
mkdir -p FileList
cd FileList
mkdir -p RootList
find ${direc}/*.root > ./AllRoot.txt
#
while read line
do
filename_tmp=${line%.root}
filename=${filename_tmp#${direc}}
echo "${line}" > ./RootList/"${filename}".txt
done < AllRoot.txt
#
find ${direc}/FileList/RootList/*.txt > ./AllTxt.txt
cd ..
