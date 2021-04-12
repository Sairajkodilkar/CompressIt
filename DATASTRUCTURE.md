# Datastructure used in CompressIt

## Huffman
Huffman encoder and decoder both uses binary tree implimentation.


## LZW 

### LZW encoder
* LZW encoder uses trie data structure. 

* Here trie data structure has linked list of childs, where each child can 
cantain one byte from 0-255. 

* When inserting a string:
    * desends down the tree for each matching character in child nodes 
    * once first unmatched character is found it allocates new nodes 
    * inserts remaing character of string. 

* When matching string: 
    * Desends down the tree for each matching character in child nodes 
    * If character is unmatched it returns -1 
    * If all charater matched it returns its index.

* Modification:
    * We know that LZW grows the dictionary character by character.

    * Using this to the advantange trie data structure is modified using a
    static pointer.

    * Once the character is read from the file it is searched in the linked
    list of child which is O(256) for worst case if any character in child 
    matched then static pointer points to that child and its index is 
    returned

    * If character is Unmatched then it allocates that character and returns
     -1 to indicate caller that new character is allocated.

    * Although for general purpose this is makes trie very hard to use but 
     for LZW it makes algorithm much more easy and improves time complexity 
     significatly as after unmatched character is found LZW outputs previous 
     index storing unmatched string in the dictionary

    * Because of this modification trie does not match new string from the
      start unless specified. which makes each character search and insert as
      O(1) and Making LZW encoder as O(n) algorithm

### LZW decoder   

* LZW decoder uses Hash table instead of trie.
* Implimentation of Hash table is done by chaining with _&alpha;_ hash 
table slots.
* The average time for such hashtable is O(m / _&alpha;_) by where m is
total number entries in hashtable.



