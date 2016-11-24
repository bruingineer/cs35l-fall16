#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int compares = 0;

int frobcmp(char const *a, char const *b);
int frobcmp_qsort(const void *a, const void *b);

int main(int argc, char *argv[])
{

  struct stat buff;
  char *bytes;
  char **lines;
  int size_alloc;
  int size_file = 0;
  int position = 0;
  int bytes_read = 0;

  if (fstat(0, &buff) == -1) {
    fprintf(stderr, "Reading file error.\n");
    exit(1);
  }

  if (buff.st_size == 0) {
    size_alloc = 32;
  }
  else
    size_alloc = buff.st_size;

  bytes = malloc(size_alloc);

  if (bytes == NULL) {
    fprintf(stderr, "No more memory to allocate.\n");
    exit(1);
  }

  while ((bytes_read = read(0,bytes+position,size_alloc-position)) > 0) {
    size_file += bytes_read;
    position = size_alloc;

    fstat(0, &buff);

    if (buff.st_size != size_alloc) {
      if (buff.st_size == 0)
	size_alloc *= 2;
      else
	size_alloc = buff.st_size;

      char *bytes_realloc = realloc(bytes, size_alloc);

      if (bytes_realloc == NULL) {
	fprintf(stderr, "No more memory to reallocate.\n");
	exit(1);
      }

      bytes = bytes_realloc;
    }
  }

  if (bytes_read == -1) {
    fprintf(stderr, "reading error.\n");
    exit(1);
  }

  if (size_file > 0 && bytes[size_file-1] != ' ') {
    bytes = realloc(bytes, size_file + 1);
    bytes[size_file] = ' ';
    size_file++;
  }

  int pos_lines = -1;
  int size_lines = 32;
  lines = malloc(size_lines * sizeof(char*));

  if (lines == NULL) {
    fprintf(stderr, "No more memory to allocate.\n");
    exit(1);
  }

  int space_prev = 1;

  int iter;
  for (iter = 0; iter < size_file-1; iter++) {
    if (bytes[iter] == ' ') {
      space_prev = 1;
      continue;
    }

    if (space_prev) {
      pos_lines++;

      if (pos_lines == size_lines) {
	size_lines *= 2;
	char **lines_realloc = realloc(lines, size_lines * sizeof(char*));

	if (lines_realloc == NULL) {
	  fprintf(stderr, "No more memory to reallocate.\n");
	  exit(1);
	}

	lines = lines_realloc;
      }

      lines[pos_lines] = bytes + iter;
      space_prev = 0;
    }
  }

  qsort(lines, pos_lines + 1, sizeof(char*), frobcmp_qsort);

  int iter_lines;
  for (iter_lines = 0; iter_lines <= pos_lines; iter_lines++) {
    int len = 0;
    for (; lines[iter_lines][len] != ' '; len++);
    len++;
    write(1, lines[iter_lines], len);
  }

  free(bytes);
  free(lines);

  fprintf(stderr, "\nComparisons: %d\n", compares);
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
  compares++;
  return frobcmp(*(char const **)a, *(char const **)b);
}
