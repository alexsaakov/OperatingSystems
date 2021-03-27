#!/bin/sh
counter=0
flag=0
handler(){
	echo
	flag=1
	counter=`expr $counter + 1`
	if [ $counter -ge 6 ]; then
		ls *.c
	fi
}

trap 'handler' 2

while :
do
	echo Enter c source files or press q to leave
	read input
	if [ $flag = 1 ]; then
		flag=0
		continue
	fi
	if [ $input = q ]; then
		exit 0
	fi
	gcc $input -o out
	echo Program output
	./out
done
