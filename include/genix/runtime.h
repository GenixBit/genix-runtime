#ifndef GENIX_RUNTIME_H
#define GENIX_RUNTIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GENIX_RUNTIME_ABI_VERSION 1

typedef struct { bool has_value; int64_t value; } GbOptionInt;
typedef struct { bool has_value; double value; } GbOptionFloat;
typedef struct { bool has_value; bool value; } GbOptionBool;
typedef struct { bool has_value; const char* value; } GbOptionString;

typedef struct { bool ok; int64_t value; const char* error; } GbResultInt;
typedef struct { bool ok; double value; const char* error; } GbResultFloat;
typedef struct { bool ok; bool value; const char* error; } GbResultBool;
typedef struct { bool ok; const char* value; const char* error; } GbResultString;

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

/* Host-service APIs used by compiler-recognized standard-library intrinsics. */
char* gb_input(const char* prompt);
char* gb_env_get(const char* name);
GbOptionString gb_env_get_option(const char* name);
char* gb_fs_read_text(const char* path);
GbResultString gb_fs_try_read_text(const char* path);
void gb_fs_write_text(const char* path, const char* text);
GbResultBool gb_fs_try_write_text(const char* path, const char* text);
_Noreturn void gb_process_exit(int64_t code);

#ifdef __cplusplus
}
#endif

#endif
