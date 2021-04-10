1. Why Huffman fails for random data ?
Huffman algorithm uses entropy encoding assigning shorter code for frequent 
characters and longer codes for infrequent character. 
Since in random data files there is equal probability of each character
appearing in the file for which huffman algorithm creates balance [huffman 
tree] (../Datastructure.md). Additionally there is 262 bytes of overhead for 
huffman algorithm hence filesize increases.

2. Why LZW fails for random data ?
LZW is dictionary based compression algorithm. Hence it compresses the file
based on pattern previously appeared in the file. Since Random data file has
minimal pattern hence it outputs the two bytes codes more frequently.

3. Why Huffman beats LZW in terms of compressed file size for random data ?
Huffman assigns nearly equal length codes for each char in the random data 
file which creates nearly equal size compressed file. But in LZW the output
index is fixed which is 2 byte so for every new pattern there is 2 byte output
hence as randomness increases file size increases much faster than huffman


4. Why Huffman works better for pattern data ?
As The probablity of bytes occuring in the file is distributed
over the large range, meaning some bytes occurs more frequently than the 
others, Huffman algorithm assigns shorter codes to more frequent bytes 
resulting in compression.


5. Why LZW works better for pattern data ?
In LZW once the new pattern is seen in the file it is logged into dictionary
with unique index. This index is outputted to file each time it encounter the
pattern. Hence greater the pattern in the file smaller the file will be.


6. Why LZW beats Huffman in terms of compressed file size for pattern data ?
In huffman codes are fixed and determined using statistic of source 
being encoded. Whereas LZW adapts to the data as it reads the file and
increases the dictionary size. As LZW reads more data it encodes the longer 
pattern in the 2 bytes.

7. Why Huffman compression ratio decreases with increase in file size ?
As the file size increases the probability that each byte occurs with same 
frequency increases which result in less compression of file

8. Why LZW compression ratio decreases with increase in file size ?
As the file size increases dictionary of pattern fills up which has maximum of
65536 entries for 2 byte code, After these many amount of entries 
we are not able to add new pattern into dictionary and compression is 
decreases, However This problem can easily be solved by using code size longer
than 2 bytes, but this will result in decrease in compression ratio for smaller
files




