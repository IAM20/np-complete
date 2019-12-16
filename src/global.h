typedef struct _pair {
	int     ch;
	int     count;
}pair;

int         i;                       // line
int         m;                       // width
char        smatrix[5001][1001];    // sparsed string matrix
int         usedch[1001][128];      // Used char in columns
pair        mostch[1001];

struct {
	char *  t;
	int     hamD;
} res;

#define TRUE 1
#define FALSE 0