typedef struct _node {
	struct _node *  next;
	struct _node *  prev;
	unsigned int *  dp;             // For save the former result.
	unsigned int    maxhd;          // max hamming distance
	unsigned int    minhd;
	float           average;
	int             progress;
	char *          until;
}node;

typedef struct _q {
	struct _node *  head;
	struct _node *  tail;
	unsigned int    size;
}Q;

Q *             makeQ();
node *          _makeN(char *, int, const unsigned int *, unsigned int);
int             empty(Q *);
int             insert(Q *, char *, int, unsigned int *,unsigned int);
int             insertN(Q * q, node * n);
node *          pop(Q *);

void            printQ(Q *);
void            freeN(node *);
void            freeQ(Q *);