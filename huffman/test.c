#include "huffman.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>


void traverse(priority_queue *q, int i){
	for(int i = 0; i < q->queue_size; i++){
		printf("%d\n", q->queue[i]->sym->frequency);
	}

}
int main(){

	priority_queue pq;
	init_priority_queue(&pq);
	node *huf_node;

	for(unsigned char i = 'a'; i < 'z'; i++){
	
		huf_node = (node *) malloc(sizeof(node));
		huf_node->sym = (symbol *) malloc(sizeof(node));
		huf_node->sym->ch = i;
		huf_node->sym->frequency = i % 26;
		
		queue_insert(&pq, huf_node);
	}


	while(!queue_is_empty(&pq)){
		huf_node = queue_extract_min(&pq);
		printf("%d ", huf_node->sym->frequency);
	}

	return 0;
}
