/*
 * freelist.h
 *
 *  Created on: Nov 8, 2017
 *      Author: ldamon
 */

#ifndef FREELIST_H_
#define FREELIST_H_

#define BLOCK_SIZE 256

typedef struct node Node;
typedef struct memblock MemBlock;

/* MemBlock:
 *
 * a structure for use keeping track of memory allocations
 * this structure can then be inserted into a Node, where it
 * would represent a free block of memory.
 *
 * It can also be used in a list of allocated memory.  I
 * recommend creating an array of MemBlocks, where the array
 * is indexed by the allocation ID.  You can then keep track
 * of the MemBlocks that are allocated, and then when they are
 * freed you can add them onto the free list.
 */
struct memblock{
	int startingAddr;
	int size;
};

/*
 * Node:
 *
 * Each element of the free list is a Node *.  You should never need
 * to allocate a Node directly -- instead, you'll create a MemBlock and
 * then use the functions below to ask for the MemBlock to be added to
 * the free list.
 */
struct node {
	Node * next;
	Node * prev;
	MemBlock block;
};

/* insert block as the first entry in the free list */
void insertFirst(MemBlock block);

/* insert a block in the free chain after the node location,
   inserts at the beginning if location is NULL */
void insertAfter(MemBlock block, Node * location);

/* delete the indicated node from the free list */
void deleteNode(Node * oldNode);

/* get the first node in the free list */
Node * getFirst();

/* get the last node in the free list */
Node * getLast();

/* dump the free list to stdout */
void dumpFreeList();

#endif /* FREELIST_H_ */
