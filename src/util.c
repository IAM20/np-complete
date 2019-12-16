#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "util.h"
#include "global.h"

typedef unsigned int uint;
typedef unsigned char uchar;

uint            _cal_hamm(const char *,const char *,int);       // Calculating two string's hamming distance.
uint            _max_hamm(const char *, int);                   // Return the max hamming distance of target string.
uint            _dp_hamm(node *, char, int);                    // Calculating hamming distance by DP.
int             _is_valid(node *, const uint *);                // Check the validity of node. (For backtracking)
int             _found_answer(node *, const uint *);            // We found the answer.
void            _modify_min(uint *, int, int);                  // Modify min array.
uint *          _init_result_arr();                             // Make result array for backtracking.

Q *             _init_bfs(uint *);                              // Make bfs queue.
node *          _bfs(Q *, uint *);                              // Executing bfs.
void            _gc(Q *, uint *, node *);                       // Executing GC

void
np_solve()
{
	/* Initiating phase */
	uint * result_arr = _init_result_arr(); // For saving the min result value in width (index).
	Q * q = _init_bfs(result_arr);          // Make essentials before executing BFS.

	node * result = _bfs(q, result_arr);    // Execute BFS.

	strcpy(res.t, result->until);           // Save the result
	res.hamD = result->maxhd;
	//printf("d hamm distance : %u\n", _max_hamm(result->until, m));
	_gc(q, result_arr, result);
}

node *
_bfs(Q * q, uint * result_arr)
{
	if (q == NULL || result_arr == NULL) {
		fprintf(stderr, "FATAL ERROR : argument q and result_arr must not be NULL\n");
		return NULL;
	}

	node * result = _makeN("", 0, NULL, 0);

	int d = 0;
	while (!empty(q)) {
		node * n = pop(q);
		if (!_is_valid(n, result_arr)) {
			freeN(n);
			continue;
		}

		if (_found_answer(n, result_arr)) {
			result->maxhd = n->maxhd;
			result->progress = n->progress;
			result->minhd = n->minhd;
			strcpy(result->until, n->until);
			freeN(n);
			break;
		}
		d++; // for debug
		//fprintf(stderr, "%d\n", d);

		int untilW = n->progress; // Human readable width (start from 1)
		uint untilM = n->maxhd;
		char * str = n->until;
		uint * dp = n->dp;

		for (uchar idx = 0; idx < 128; idx++) {
			if (usedch[untilW][idx]) {
				str[untilW] = idx;
				str[untilW + 1] = '\0';
				node * tmp = _makeN(str, untilW + 1, dp, untilM);
				uint ret = _dp_hamm(tmp, idx, untilW + 1);

				if (ret < result_arr[untilW]) {
					_modify_min(result_arr, untilW, result_arr[untilW] - ret);
				} else if (ret > result_arr[untilW]) {//idx != mostch[untilW].ch ) { --> For general string.
					/* Useless cut. */
					freeN(tmp);
					continue;
				}

				insertN(q, tmp);
			}
		}

		freeN(n);
	}
	printf("Total iter : %d\n", d);
	if (result == NULL || result->progress == 0) {
		fprintf(stderr, "NOT FOUND!\n");
		exit(1);
	}
	return result;
}

void
_gc(Q * q, uint * result_arr, node * result)
{   // Do not change this sequence.
	free(result);
	freeQ(q);
	free(result_arr);
}

uint
_cal_hamm(const char * l,const char * r, int width)
{
	uint result = 0;
	if (l == NULL || r == NULL) {
		fprintf(stderr, "Argument left and right must not be NULL");
		exit(-1);
	}

	for (int j = 0; j < width; j++) {
		if (l[j] != r[j]) {
			result++;
		}
	}

	return result;
}

uint
_max_hamm(const char * str, int width)
{
	uint max = 0;
	for (int l = 1; l <= i; l++) {
		if (max == width) {
			return max;
		}

		uint hamm = _cal_hamm(str, smatrix[l], width);
		if (max < hamm) {
			max = hamm;
		}
	}
	return max;
}

uint
_dp_hamm(node * n, char ch, int width)
{
	uint max = n->maxhd;
	uint min = n->maxhd;
	unsigned long long sum = 0;
	for (int l = 1; l <= i; l++) {
		if (smatrix[l][width - 1] != ch) {
			uint last = ++n->dp[l];
			if (last > max) {
				max = last;
			}
			if (last < min) {
				min = last;
			}
			sum += last;
		}
	}

	n->maxhd = max;
	n->minhd = min;
	n->average = (float)sum / i;

	return max;
}


int
_is_valid(node * n, const uint * min)
{
	int cur_width = n->progress - 1;
	return !(n->maxhd > min[cur_width] ||
	         (n->progress == m && n->maxhd != min[cur_width]));
}

int
_found_answer(node * n, const uint * min)
{
	return (n->progress == m && n->maxhd == min[n->progress - 1]);
}

void
_modify_min(uint * min, int idx, int amount)
{
	for (int w = idx; w < m; w++) {
		min[w]-= amount;
	}
}

uint *
_init_result_arr()
{
	uint * result = (uint *)malloc(sizeof(int) * m);
	if (result == NULL) {
		fprintf(stderr, "Heap is full\n");
		exit(-1);
	}

	for (uint idx = 0; idx < m; idx++) {
		result[idx] = idx + 1;
	}
	return result;
}

Q *
_init_bfs(uint * result_arr) // Result array for dp.
{
	Q * q = makeQ();
	for (unsigned char idx = 0; idx < 128; idx++) {
		if (usedch[0][idx]) {
			char str[2];
			str[0] = idx;
			str[1] = '\0';

			node * n = _makeN(str, 1, NULL, 0);
			uint maxhd = _dp_hamm(n, idx, 1);
			if (maxhd < result_arr[0]) {
				result_arr[0] = maxhd;
				result_arr[1] = maxhd + 1;
			}
			insertN(q, n);
		}
	}
	return q;
}
