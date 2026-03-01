#ifndef VM_H
#define VM_H

#define NUM_REGS 16
#define HEAP_MAX 1024*1024

#include <stdint.h>
#include <da_arr.h>
#include <stdbool.h>

typedef enum {
  B_INT,
  B_CHAR,
  B_ANY,
  B_STRUCT,
} Type;

typedef struct {
  
} b_struct;

typedef struct {
  Type tag;
  union {
    int integer;
    char ch;
    void* any;
    b_struct str;
  } value;
} brass_val;


typedef struct {
  brass_val regs[NUM_REGS];
  brass_val* heap;
  uint8_t flags;
} CPU;


CPU cpu_create();
void cpu_free(CPU* cpu);

void dispatch(CPU* cpu);
brass_val* get_reg(CPU* cpu, size_t num);
brass_val* get_heap_reg(CPU* cpu, size_t num);

void bval_free(brass_val* val);
void bstruct_free(b_struct* str);

brass_val from_int(int integer);
brass_val from_char(char ch);
brass_val from_any(void* ptr);


#endif
