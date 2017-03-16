#!/bin/bash
echo "/media/strange/F6926FD0926F93C1/Documents and Settings/Administrator/Desktop/SDK-gcc/input/$1"
echo "/media/strange/F6926FD0926F93C1/Documents and Settings/Administrator/Desktop/SDK-gcc/output/$2"

./build.sh

cd bin
./cdn "/media/strange/F6926FD0926F93C1/Documents and Settings/Administrator/Desktop/SDK-gcc/input/$1" "/media/strange/F6926FD0926F93C1/Documents and Settings/Administrator/Desktop/SDK-gcc/output/$2"
cat "/media/strange/F6926FD0926F93C1/Documents and Settings/Administrator/Desktop/SDK-gcc/output/$2"

