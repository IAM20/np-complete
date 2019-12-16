/***
 * Following LINUX kernel coding convention.
 * REFERENCE : https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "queue.h"
#include "util.h"
#include "global.h"

void        _getinfo(int, char **);
void        _init_mtx();
void        _print_info();
void        _print_result();
int         _fin();
long long   _current_t();

long long   _start_t;       // Start time of this program.
FILE *      _input;         // input file.

int
main(int argc, char * argv[])
{
	_getinfo(argc, argv);   // Initiating information of FILE & getting current time.
	_init_mtx();            // Getting data from FILE.
	_print_info();          // Print data, for debug.
	np_solve();             // Solving the data.
	_print_result();        // Print result.
	return _fin();          // Finishing the program (Free the file and write result.)
}

void
_print_result()
{
	FILE * output = fopen("output.txt", "wt");
	fprintf(output, "string t: %s\n", res.t);
	fprintf(output, "d-value: %d\n", res.hamD);
	fclose(output);
}

int
_fin()
{
	fclose(_input);
	free(res.t);
	long long fin_t = _current_t();
	printf("%lld milliseconds consumed\n", fin_t - _start_t);

	return 0;
}

void
_print_info()
{
	printf("m : %d\t\t\ti : %d\n", m, i);
	printf("Most frequently used char arr:");
	for (int w = 0; w < m; w++) {
		printf("%c", mostch[w].ch);
	} printf("\n\n");
}

void
_getinfo(int argc, char * argv[])
{
	_start_t = _current_t();
	char * string = "input.txt";

	if (argc == 2) {
		size_t length = strlen(argv[1]);
		string = (char *)malloc(sizeof(length));
		strcpy(string, argv[1]);
	}

	FILE * result = fopen(string, "rt");

	if (result == NULL) {
		perror("Failed to open file.");
		exit(-1);
	}

	_input = result;
}

void
_init_mtx()
{
	if (_input == NULL) {
		fprintf(stderr, "Arg _input must not be null");
		exit(-1);
	}

	char ch, before = '\0';
	int curline = 1;
	int sidx = 0;

	while((ch = (char)fgetc(_input)) != EOF) {
		if (ch == '\n') {
			smatrix[curline][sidx] = '\0';
			m = sidx;
			sidx = 0;
			curline++;
			before = ch;
			continue;
		}
		usedch[sidx][(int)ch]++;
		smatrix[curline][sidx++] = ch;
		before = ch;

		if (mostch[sidx - 1].count < usedch[sidx - 1][(int)ch]) {
			mostch[sidx - 1].count = usedch[sidx - 1][(int)ch];
			mostch[sidx - 1].ch = ch;
		}
	}
	if (before == '\n') {
		curline--;
	}
	i = curline;

	res.t = (char *)malloc(sizeof(char) * (m + 1));
	res.hamD = 0;
}

long long
_current_t() {
	struct timeval te;
	gettimeofday(&te, NULL);
	long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;

	return milliseconds;
}