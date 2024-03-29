#ifndef _PRIORITY_QUEUE
#define _PRIORITY_QUEUE

#include <stdio.h>
#include "huffman.h" //provides the definition of symbol

#define PARENT(i) (((i) - 1) / 2)
#define LEFTCHILD(i) (2 * (i) + 1)
#define RIGHTCHILD(i) (2 * (i) + 2)


typedef struct priority_queue {

	/* priority queue to contain atmost 256 symbols 			*/
	void *queue[CHAR_RANGE];

	int queue_size;

} priority_queue;


/* 
 * user interface functions							
 */
void init_priority_queue(priority_queue *q);

void queue_insert(priority_queue *q,  node *nd, int (*comp) (void *, void *));
void *queue_extract_min(priority_queue *q, int (*comp) (void *, void *));


/*! 
 * @brief this function swap the two node pointer
 *
 * @param[in] first pointer to the pointer of node
 * @param[in] second pointer to the pointer of node
 */
static inline void exchange(void **a, void **b){
	void *temp = *a;
	*a = *b;
	*b = temp;
	return;
}

/*! 
 * @brief this function checks if queue is empty
 *
 * @param[in] pointer to the priority queue
 * @param[out] returns 1 if queue is empty
 */

static inline int queue_is_empty(priority_queue *q){
	if(q == NULL)
		return 1;
	return q->queue_size == 0;
}


static inline int get_queue_size(priority_queue *q){
	if(q == NULL)
		return -1;
	return q->queue_size;
}

#endif




