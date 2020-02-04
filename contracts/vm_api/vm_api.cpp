
#include <stdlib.h>
#include <stdint.h>
#include <stdexcept>

#include "vm_api.h"
static struct vm_api* s_api = nullptr;
static struct vm_api* s_api_ro = nullptr;

void vm_register_api(struct vm_api* api) {
   if (!api) {
      throw std::runtime_error("vm_api pointer can not be NULL!");
   }
   s_api = api;
}

struct vm_api* get_vm_api() {
   if (!s_api) {
//      print_stacktrace();
      throw std::runtime_error("vm api not specified!!!");
   }
   return s_api;
}
