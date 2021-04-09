#!/usr/bin/python3

import pandas as pd
import matplotlib.pyplot as plt
import sys

n = len(sys.argv)
if( n != 3 ):
    print("Usage: ./plot.py [Huffman Result] [LZW result]")
    sys.exit(1)

titles = ["Filename", "Time", "Input_size", "Output_size", "percentage"]
units = {"Time":"second", "Input_size":"byte", "Output_size":"byte",
        "percentage":""}

huffman_result = pd.read_csv(sys.argv[1], names=titles)
lzw_result = pd.read_csv(sys.argv[2], names=titles)

xquantity = titles[2]

for col in titles :

    if(col == titles[0] or col == xquantity):
        continue

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)

    unit = units[xquantity]

    xlabel = f"{xquantity} {units[xquantity]}"
    ylabel = f"{col} {units[col]}"
    title = f'Huffman and LZW {col} comparison'

    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)

    huffman_result.plot(ax=ax, x=xquantity, y=col, kind="line", 
            figsize=(10,10), label="Huffman")

    lzw_result.plot(ax=ax, x=xquantity, y=col, kind="line", 
            figsize=(10,10), label="LZW")

    plt.savefig(f'{xquantity}_{col}.jpg')




