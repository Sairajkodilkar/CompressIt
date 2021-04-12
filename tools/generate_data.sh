#!/usr/bin/bash


if [ $# -lt 1 ] ; then
	echo "Usage $0: [FILES].. "
	exit 1
fi
shift 1

ECHO=echo

result=$1

OPTION_HUFF=-ych
OPTION_LZW=-ycl

cmd="./compressit" 


truncate -s 0 .result_lzw.temp
truncate -s 0 .result_huff.temp

for infile in "$@"; do
	if ! [[ $infile =~ lzw$ ]] && ! [[ $infile =~ huff$ ]] && [ -f $infile ]; 
    then
		echo "compressing $infile" > /dev/tty
		$cmd "$OPTION_HUFF" -f "$infile" >> .result_huff.temp 
		$cmd "$OPTION_LZW" -f "$infile" >> .result_lzw.temp 
	fi
done

sort -t" " -k3 -n .result_lzw.temp > result_lzw.txt
sort -t" " -k3 -n .result_huff.temp > result_huff.txt


