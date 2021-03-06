#include <stdlib.h>
#include <stdio.h>

int frobcmp(char const *a, char const *b);
int frobcmp_qsort(const void *a, const void *b);

int main(int argc, char *argv[])
{
  int size_lines = 32;
  int pos_lines = -1;
  int size_str = 32;
  int pos_str = 0;
  char **lines = malloc(size_lines * sizeof(char*));

  int ch = getchar();
  int end_of_file = 0;

  while (ch != EOF) {
    
    if (pos_str == 0) {
      pos_lines++;
      if (pos_lines == size_lines) {
	size_lines *= 2;
	char **realloc_lines = realloc(lines, size_lines * sizeof(char*));

	if (realloc_lines == NULL) {
	  fprintf(stderr, "Cannot reallocate more memory\n");
	  exit(1);
	}

	lines = realloc_lines;
      }
      lines[pos_lines] = malloc(size_str *sizeof(char));
    }

    if (pos_str == size_str) {
      size_str *= 2;
      char *realloc_str = realloc(lines[pos_lines], size_str * sizeof(char));

      if (realloc_str == NULL) {
	fprintf(stderr, "Cannot reallocate more memory\n");
	exit(1);
      }
      lines[pos_lines] = realloc_str;
    }

    lines[pos_lines][pos_str] = ch;
    pos_str++;

    if (ch == ' ') {
      pos_str = 0;
      size_str = 32;
    }

    ch = getchar();
  }

  if (pos_str != 0) {
    if (pos_str == size_str) {
      char *realloc_str = realloc(lines[pos_lines], (pos_str+1) * sizeof(char));
      if (realloc_str = NULL) {
	fprintf(stderr, "No more memory to reallocate string\n");
	exit(1);
      }
    }
    lines[pos_lines][pos_str] = ' ';
  }

  if (ferror(stdin)) {
    fprintf(stderr, "Errow when reading stdin input\n");
    exit(1);
  }

  qsort(lines, pos_lines+1, sizeof(char*), frobcmp_qsort);

  int l = 0;
  for (; l < pos_lines +1; l++) {
    int k = 0;
    for (; lines[l][k] != ' '; k++) {
	
      if (putchar(lines[l][k]) == EOF) {
	  fprintf(stderr, "Writing error to stdout\n");
	  exit(1);
      }
    }
    if (putchar(' ') == EOF) {
      fprintf(stderr, "Writing error to stdout\n");
      exit(1);
    }
  }

  int m = 0;
  for (; m < pos_lines +1; m++) {
    free(lines[m]);
  }
  free(lines);

  exit(0);
}

int frobcmp(char const *a, char const *b) {
  int i = 0;
  for (; a[i] != ' ' && b[i] != ' '; i++) {
    char defrob_a = a[i]^0x2a;
    char defrob_b = b[i]^0x2a;

    int d = defrob_a - defrob_b;
    if (d != 0)
      return d;
  }

  if (a[i] != ' ')
    return 1;
  if (b[i] != ' ')
    return -1;

  return 0;
}

int frobcmp_qsort(const void *a, const void *b) {
  return frobcmp(*(char const **)a, *(char const **)b);
}
