/* Build tree with min variance such that internal nodes has less priority than extern node */
#include "heap.h"
#include "huffman.h"


void min_heapify(priority_queue *q, int i);

void init_priority_queue(priority_queue *q){

	/* Initially queue is empty 				*/
	q->queue_size = 0;

	return;
}

#define TRUE (1)

void min_heapify(priority_queue *q, int i){
	int l, r, smallest;

	while(TRUE){

		smallest = i;

		l = LEFTCHILD(i);
		r = RIGHTCHILD(i);

		/* Find the smallest of parent, left child, right child 		*/

		if(l < q->queue_size && lessthan(q->queue[l], q->queue[smallest]))
			smallest = l;

		if(r < q->queue_size && lessthan(q->queue[r], q->queue[smallest]))
			smallest = r;

		/* 
		 * If parent is the smallest then simply exchange parent with smallest
		 * and continue with smallest as new parent.
		 */
		if(smallest != i){
			exchange(&(q->queue[i]), &(q->queue[smallest]));
			i = smallest;
		}
		else{
			break;
		}
	}
	return;
}

void queue_insert(priority_queue *q,  node *nd){

	/* Get the empty index 							*/
	int i = q->queue_size;

	if(i >= CHAR_RANGE)
		return;

	/* Insert input node to that index 				*/
	q->queue[i] = nd;

	int p = PARENT(i);

	/* Loop until min heap property is satisfied 	*/
	while(i > 0 && lessthan(q->queue[i], q->queue[p])){
		exchange(&(q->queue[i]), &(q->queue[p]));
		i = p;
		p = PARENT(i);
	}

	q->queue_size++;
}

node *queue_extract_min(priority_queue *q){
	/* check if queue is empty 						*/
	if(queue_is_empty(q))
		return NULL;

	/* Get the top of the heap which is min			*/
	node *min_node = q->queue[0];

	int i = --q->queue_size;

	/* replace min by last of the queue 			*/
	q->queue[0] = q->queue[i];
	
	min_heapify(q, 0);

	return min_node;
}


