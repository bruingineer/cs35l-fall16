// tr2u.c
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(stderr, "Incorrect amount of arguments. Provide only two.\n");
    exit(1);
  }
  
  char *from = argv[1];
  char *to = argv[2];
 
  // check lengths
  int length_check_index = 0;
  for(; from[length_check_index] != 0 && to[length_check_index] != 0; length_check_index++);
  if (from[length_check_index] != 0 || to[length_check_index] != 0) {
    fprintf(stderr, "To and From must be equal lengths.\n");
    exit(1);
  }

  // check dups in from
  int first_dup_index = 0;
  for(; from[first_dup_index] != 0; first_dup_index++) {
    int second_dup_index = first_dup_index + 1;
    for(; from[second_dup_index] != 0; second_dup_index++) {
      if (from[first_dup_index] == from[second_dup_index]) {
	fprintf(stderr, "First arg must have no duplicate chars.\n");
	exit(1);
      }
    }
  }

  int read_value;
  int ch;

  while((read_value = read(0, &ch, 1)) > 0) {
    
    int index;
    for (index = 0; from[index] != 0; index++) {
      if (ch == from[index]) {
	ch = to[index];
	break;
      }
    }

    if (write(1,&ch,1) < 1) {
      fprintf(stderr, "Error while writing char.\n");
      exit(1);
    }
  }
	
  if (read_value == -1) {
    fprintf(stderr, "Error while reading character.\n");
    exit(1);
  }

  exit(0);
}
