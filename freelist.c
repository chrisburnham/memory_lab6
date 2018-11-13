/*
 * freelist.c
 *
 *  Created on: Nov 8, 2017
 *      Author: ldamon
 */

#include "freelist.h"
#include <stdlib.h>
#include <stdio.h>

static Node * freelist;
static Node * freeTail;
  
  
void insertFirst(MemBlock block)
{
   Node * new = (Node *)malloc(sizeof(Node));
   new->block = block;
   new->prev = NULL;
   new->next = freelist;
   if (freelist != NULL)
     freelist->prev = new;
   else
     freeTail = new;
   freelist = new;
}

void insertAfter(MemBlock block, Node * location)
{
  if (location == NULL)
    {
      insertFirst(block);
      return;
    }

   Node * new = (Node *)malloc(sizeof(Node));
   new->block = block;
   new->prev = location;
   Node * next = location->next;
   location->next = new;
   new->next = next;
   if (next != NULL)
     next->prev = new;
   else
     freeTail = new;
}

void deleteNode(Node * oldNode)
{
   Node * next = oldNode->next;
   if (next != NULL)
     next->prev = oldNode->prev;
   else
      freeTail = oldNode->prev;
   if (oldNode->prev == NULL)  // it's the head
     freelist = next;
   else
      oldNode->prev->next = next;
   free(oldNode);
}


Node * getFirst()
{
  return freelist;
}

Node * getLast()
{
  return freeTail;
}


void dumpFreeList()
{
  printf("\nFree list: \n");
  Node * ptr;
  for (ptr = freelist; ptr != NULL; ptr = ptr->next)
  {
	 printf("size: %d addr: %d\n",ptr->block.size, ptr->block.startingAddr);
  }
}


