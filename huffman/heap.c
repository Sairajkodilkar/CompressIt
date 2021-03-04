/* Build tree with min variance such that internal nodes has less priority than extern node */
#include "heap.h"


void min_heapify(priority_queue *q, int i);

void init_priority_queue(priority_queue *q){

	if(!q)
		return;

	/* Initially queue is empty 				*/
	q->queue_size = 0;

	return;
}

#define TRUE (1)

void heapify(priority_queue *q, int i, int (*comp) (void *, void *)){

	if(!q || !comp)
		return ;

	int l, r, sg;

	while(TRUE){

		sg = i;

		l = LEFTCHILD(i);
		r = RIGHTCHILD(i);

		/* Find the smallest/greatest of parent, left child, right child 		*/

		if(l < q->queue_size && (*comp)(q->queue[l], q->queue[sg]))
			sg = l;

		if(r < q->queue_size && (*comp)(q->queue[r], q->queue[sg]))
			sg = r;

		/* 
		 * If parent is the smallest/greatest then simply exchange parent with smallest
		 * and continue with smallest/greatest as new parent.
		 */
		if(sg != i){
			exchange(&(q->queue[i]), &(q->queue[sg]));
			i = sg;
		}
		else{
			break;
		}
	}
	return;
}

void queue_insert(priority_queue *q,  node *nd, int (*comp)(void *, void *)){

	if(!q || !nd || !comp)
		return;

	/* Get the empty index 							*/
	int i = q->queue_size;

	if(i >= CHAR_RANGE)
		return;

	/* Insert input node to that index 				*/
	q->queue[i] = nd;

	int p = PARENT(i);

	/* Loop until min heap property is satisfied 	*/
	while(i > 0 && (*comp)(q->queue[i], q->queue[p]) ){
		exchange(&(q->queue[i]), &(q->queue[p]));
		i = p;
		p = PARENT(i);
	}

	q->queue_size++;
}

void *queue_extract_min(priority_queue *q, int (*comp) (void *, void *)){
	/* check if queue is empty 						*/
	if(queue_is_empty(q) || !comp || !q)
		return NULL;

	/* Get the top of the heap which is min			*/
	void *min_node = q->queue[0];

	int i = --q->queue_size;

	/* replace min by last of the queue 			*/
	q->queue[0] = q->queue[i];
	
	heapify(q, 0, comp);

	return min_node;
}


