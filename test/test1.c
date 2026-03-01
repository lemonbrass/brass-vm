#include <vm.h>


int main() {
  CPU cpu = cpu_create();
  cpu_free(&cpu);
  return 0;
}
