#include <stdio.h>

int main() {
  int a = 0;
  int b = 1;
  int c = a + 1;
  int d = b / a;  // Divide by zero
  if (c) {
    int a = 1;
  }
  char i = (char) a;
  char j = getchar();
  return 0;
}
