#include <stdlib.h>
#define DA_ARR_IMPLEMENTATION
#include <da_arr.h>
#include <vm.h>


void dispatch(CPU* cpu) {
  (void) cpu;
  // TODO
}


brass_val* get_reg(CPU* cpu, size_t num) {
  return &cpu->regs[num];
}

brass_val* get_heap_reg(CPU* cpu, size_t num) {
  return da_get(cpu->heap, num);
}

brass_val from_int(int integer) {
  brass_val val = {0};
  val.tag = B_INT;
  val.value.integer = integer;
  return val;
}

CPU cpu_create() {
  CPU cpu = {0};
  da_new(cpu.heap, DEFAULT_CAPACITY);
  return cpu;
}

void cpu_free(CPU* cpu) {
  for (size_t i = 0; i < da_len(cpu->heap); i++) {
    bval_free(&cpu->heap[i]);
  }
  for (size_t i = 0; i < NUM_REGS; i++) {
    bval_free(&cpu->regs[i]);
  }
  da_free(cpu->heap);
}

brass_val from_char(char ch) {
  brass_val val = {0};
  val.tag = B_CHAR;
  val.value.ch = ch;
  return val;
}

brass_val from_any(void* ptr) {
  brass_val val = {0};
  val.tag = B_ANY;
  val.value.any = ptr;
  return val;
}

void bval_free(brass_val* val) {
  switch (val->tag) {
    case B_ANY: free(val->value.any);
    case B_STRUCT: bstruct_free(&val->value.str);
    default: {}
  }
}

void bstruct_free(b_struct* str) { (void)str; }
