#include "genix/runtime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct GbAllocation {
    void* pointer;
    struct GbAllocation* next;
} GbAllocation;

static GbAllocation* gb_allocations = NULL;
static bool gb_runtime_initialized = false;

void gb_runtime_init(void) {
    gb_runtime_initialized = true;
}

void gb_runtime_shutdown(void) {
    GbAllocation* current = gb_allocations;
    while (current != NULL) {
        GbAllocation* next = current->next;
        free(current->pointer);
        free(current);
        current = next;
    }
    gb_allocations = NULL;
    gb_runtime_initialized = false;
}

void* gb_alloc(size_t size) {
    if (!gb_runtime_initialized) {
        gb_runtime_init();
    }

    if (size == 0) {
        size = 1;
    }

    void* pointer = malloc(size);
    if (pointer == NULL) {
        gb_panic("out of memory");
    }

    GbAllocation* record = (GbAllocation*)malloc(sizeof(GbAllocation));
    if (record == NULL) {
        free(pointer);
        gb_panic("out of memory while tracking allocation");
    }

    record->pointer = pointer;
    record->next = gb_allocations;
    gb_allocations = record;
    return pointer;
}

_Noreturn void gb_panic(const char* message) {
    const char* text = message != NULL ? message : "unknown runtime failure";
    fprintf(stderr, "Genix runtime panic: %s\n", text);
    gb_runtime_shutdown();
    exit(70);
}

char* gb_string_concat(const char* left, const char* right) {
    if (left == NULL || right == NULL) {
        gb_panic("cannot concatenate a null string");
    }

    size_t left_len = strlen(left);
    size_t right_len = strlen(right);
    char* result = (char*)gb_alloc(left_len + right_len + 1);
    memcpy(result, left, left_len);
    memcpy(result + left_len, right, right_len);
    result[left_len + right_len] = '\0';
    return result;
}

bool gb_string_equal(const char* left, const char* right) {
    if (left == NULL || right == NULL) {
        return left == right;
    }
    return strcmp(left, right) == 0;
}

void gb_print_int(int64_t value) {
    printf("%lld\n", (long long)value);
}

void gb_print_float(double value) {
    printf("%.15g\n", value);
}

void gb_print_bool(bool value) {
    printf("%s\n", value ? "true" : "false");
}

void gb_print_string(const char* value) {
    printf("%s\n", value != NULL ? value : "<null>");
}
