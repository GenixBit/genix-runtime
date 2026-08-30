#ifndef GENIX_RUNTIME_H
#define GENIX_RUNTIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GENIX_RUNTIME_ABI_VERSION 1

void gb_runtime_init(void);
void gb_runtime_shutdown(void);

void* gb_alloc(size_t size);
_Noreturn void gb_panic(const char* message);

char* gb_string_concat(const char* left, const char* right);
bool gb_string_equal(const char* left, const char* right);

void gb_print_int(int64_t value);
void gb_print_float(double value);
void gb_print_bool(bool value);
void gb_print_string(const char* value);

#ifdef __cplusplus
}
#endif

#endif
