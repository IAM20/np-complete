#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "global.h"

Q *
makeQ()
{
	Q * result = (Q *)malloc(sizeof(Q));
	if (result == NULL) {
		fprintf(stderr, "Heap is full exit the program\n");
		exit(-1);
	}
	result->size = 0;
	result->head = result->tail = NULL;

	return result;
}

int
empty(Q * q) {
	if (q == NULL || q->size == 0) {
		return TRUE;
	}
	return FALSE;
}

int
insert (Q * q,
		char * until,
		int progress,
		unsigned int * dp,
		unsigned int maxhd)
{
	if (q == NULL) {
		fprintf(stderr, "Argument q must not be NULL\n");
		exit(-1);
	}

	node * target = _makeN(until, progress, dp, maxhd);
	return insertN(q, target);
}

int
insertN(Q * q, node * target)
{
	if (q->size == 0) {
		q->tail = q->head = target;
		target->next = target->prev = target;
		return ++q->size;
	}

	q->tail->next = target;
	q->head->prev = target;
	target->next = q->head;
	target->prev = q->tail;

	q->tail = target;

	return ++q->size;
}

node *
pop(Q * q)
{
	node * result = q->head;
	q->tail->next = result->next;
	result->next->prev = q->tail;

	q->head = q->head->next;
	result->next = result->prev = NULL;
	q->size--;

	return result;
}

void
printQ(Q * q)
{
	node * n = q->head;
	for (int i = 0; i < q->size; i++, n = n->next) {
		printf("%d <-> ", n->maxhd);
	} printf("\n");
}

void
freeN(node * n)
{
	if (n->until != NULL) {
		free(n->until);
		n->until = NULL;
	}
	free(n->dp);
	free(n);
}

void
freeQ(Q * q)
{
	while(!empty(q)) {
		node * n = pop(q);
		freeN(n);
	}
	free(q);
}

node *
_makeN (char * until,
		int progress,
		const unsigned int * dp,
		unsigned int maxhd)
{
	node * result = (node *)malloc(sizeof(node));
	if (result == NULL) {
		fprintf(stderr, "Heap is full exit the program\n");
		exit(-1);
	}

	result->dp = (unsigned int *)malloc(sizeof(int) * (i + 1));
	if (dp != NULL) {
		for (int l = 1; l <= i; l++) {
			result->dp[l] = dp[l];
		}
	} else {
		for (int l = 1; l <= i; l++) {
			result->dp[l] = 0;
		}
	}

	result->maxhd = maxhd;
	result->until = (char *)malloc(sizeof(char) * (m + 1));
	result->progress = progress;
	strcpy(result->until, until);

	return result;
}