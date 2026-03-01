#ifndef DA_ARR_H
#define DA_ARR_H

#include<stdlib.h>
#include<stdalign.h>
#include<stddef.h>
#include<stdint.h>
#include<stdbool.h>


#ifndef DEFAULT_CAPACITY
#define DEFAULT_CAPACITY 4
#endif

#define DA_FUNC 
#define da_new(arr, cap) arr = _da_new_helper(sizeof(*arr), cap)
// AVOID Double Evaluation
#define da_len(arr) _da_len_helper(arr)
#define da_cap(arr) _da_cap_helper(arr)
#define da_get(arr, num) (_da_get_helper(arr, num) ? &arr[num] : NULL)

/* arr must be lvalue

 TODO: Realloc NULL rewrites arr instead of returning false. (fix using ({}) and use #if defined (__GNU__).....)
 #define da_push(arr, item) (\
   (arr) = _da_grow(sizeof(*arr), arr),\
   (arr) != NULL ? ((arr)[_da_get_data(arr)->count++] = item, true) : false\
)  */

#define da_push(arr, item) ({\
  bool res = false;\
  void* tmp = _da_grow(sizeof(*arr), arr);\
  if (tmp) {\
    res = true;\
    arr = tmp;\
    arr[_da_get_data(arr)->count++] = item;\
  }\
  res;\
})

#define da_pop(arr) _da_pop_helper(sizeof(*arr), arr)

#define da_free(arr) do { if (arr) free((da_arr*)arr - 1); arr = NULL; } while (0)

typedef struct {
  size_t count;
  size_t capacity;
  alignas(max_align_t) char padding[];
} da_arr;



DA_FUNC void* _da_new_helper(size_t size, size_t cap);
DA_FUNC bool _da_get_helper(void* arr, size_t id);
DA_FUNC void* _da_grow(size_t size, void* arr);
DA_FUNC void* _da_pop_helper(size_t size, void* arr);
DA_FUNC size_t _da_cap_helper(void* arr);
DA_FUNC size_t _da_len_helper(void* arr);

#ifdef DA_ARR_IMPLEMENTATION

DA_FUNC void* _da_new_helper(size_t size, size_t cap) {
  size_t total = size*cap + sizeof(da_arr);
  if (total > SIZE_MAX / size) return NULL;
  da_arr* data = malloc(total);
  if (!data) return NULL;
  void* arr = data + 1;
  data->capacity = cap;
  data->count = 0;
  return arr;
}

DA_FUNC da_arr* _da_get_data(void* arr) {
  if (!arr) return NULL;
  return (da_arr*)arr - 1;
}

DA_FUNC bool _da_get_helper(void* arr, size_t id) {
  da_arr* data =_da_get_data(arr);
  if (!data) return false;
  if (data->count <= id) return false;
  return true;
}

DA_FUNC void* _da_grow(size_t size, void* arr) {
  if (!arr) {
    arr = _da_new_helper(size, DEFAULT_CAPACITY);
    if (!arr) return NULL;
  }
  da_arr* data = _da_get_data(arr);
  
  // GROW
  if (data->count >= data->capacity) {
    size_t new_cap = (data->capacity) ? data->capacity*2 : DEFAULT_CAPACITY;
    if (new_cap > SIZE_MAX / size) return NULL;
    void* new_data = realloc(data, new_cap*size + sizeof(da_arr));
    if (!new_data) return NULL;
    data = new_data;
    data->capacity = new_cap;
    arr = (char*)data + sizeof(da_arr);
  }
  
  return arr;
}


DA_FUNC void* _da_pop_helper(size_t size, void* arr) {
  da_arr* data = _da_get_data(arr);
  if (!data) return NULL;
  if (data->count == 0) return NULL;
  data->count -= 1;
  return (char*)arr + size*data->count;
}

DA_FUNC size_t _da_cap_helper(void* arr) {
  da_arr* data = _da_get_data(arr);
  if (!data) return 0;
  return data->capacity;
}

DA_FUNC size_t _da_len_helper(void* arr) {
  da_arr* data = _da_get_data(arr);
  if (!data) return 0;
  return data->count;
}

#endif
#endif
