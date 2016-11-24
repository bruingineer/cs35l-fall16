#include <stdio.h>


int main() {
  for (int i = 0; i <=255; i++) {
    printf("\\%03o", i^42);
  }

  printf("'\\\n\\\n'");

  for (int i = 0; i <=255; i++) {
    printf("\\%03o", i);
  }

  printf("' | tr '\\n' ' '\n");

  return 0;
}
