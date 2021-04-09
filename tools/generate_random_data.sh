#!/usr/bin/zsh

if [ $# -lt 1 ];
then
    echo "Usage: $0 [number of files]"
    exit 1
fi

n=$1
infile=/dev/random
bscount=1

for multiplier in {1..$n};
do
    outfile=${bscount}K.rand
    echo "$outfile, $bscount"
    dd if=$infile of=$outfile bs=1024 count=$bscount
    bscount=$(($bscount * 2))
done
