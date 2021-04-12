# Time Complexity Analysis of both the Algorithm

NOTE:   
* n = file size in bytes
* ch = maximum unsigned characters (256) / size of symbol table

## Huffman

### [Huffman encoder](./huffman/encoder.c)  

1. Encoder first builds the symbol table which contains the characters 
with their frequency using first pass on the file which is done in 
O(n). 

2. then encoder builds the priority queue with the given symbol table
Now the time complexity of priority queue is O(ch log(ch)), since ch is
constant (256) for algorithm hence we building priority queue can be O(1) 
considered as O(1).


3. Huffman tree is build using priority queue with O(ch log(ch))
     * log(ch): for extracting from queue and again for inserting
     * ch : since above operation is done till queue is empty

    But This can also be considered as O(1) since ch is constant.

4. Traversing the huffman tree is simply preorder traversal of tree 
with 256 max nodes, here also time complexity is O(ch) i.e. O(1) as nodes 
remains constant.

5. Generation of canonical huffman tree is done in O(ch) time. hence here 
time complexity is O(1).

6. While writing huffman codes input file is traversed again and
corresponding code is outputted to the output file hence time complexity is
O(n).

7. Hence total time complexity of encoder is 
     = O(n) + 4 * O(1) + O(n)
     = O(n)

### [Huffman decoder](huffman/decoder.c)
1. All steps in encoder are just in different order for decoder 
Hence time complexity for decoder is also O(n)


## LZW

### [LZW encoder](LZW/lzw_encoder.c)
1. In LZW encoder all lines till while loop take constant time, hence time
complexity is O(1).

2. In while loop modified Trie data structure is used whose working 
is explained in [DATASTRUCTURE.md](./DATASTRUCTURE.md). which takes 
O(1) time to insert and simultaneouly returns the match status.
sinde this operations are done ch time hence time complexity of while loop is 
O(n)

3. Hence Total time complexity is O(n) = O(1) + O(n).

### [LZW decoder](LZW/lzw_decoder.c)
1. In LZW decoder all lines till while loop takes constant time.

2. In decoder LZW uses hash table for storing strings by key
which takes average time of O(m / _&alpha;_) for insert or lookup. where 
m is total entries in hashtable and _&alpha;_ is number of slots in hashtable.
see [DATASTRUCTURE.md](./DATASTRUCTURE.md) for more details. since for worst 
case lookup and insert both might be done in every iteration hence time 
complexity is O(n * (m / _&alpha;_)).

3. Hence total time complexity is 
O(n * (m / _&alpha;_)) = O(1) + O(n * (m / _&alpha;_))












