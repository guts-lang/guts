#include <ucc.h>

static PURE CONST FORCEINLINE int
foo(int a, int b) {
  return a + b;
}

NORETURN void
die(int code) {
  exit(code);
}

int main(void) {
  assert(10200003UL == VERNO(1, 2, 3));
  assert(5 == foo(2, 3));
  die(0);
}
