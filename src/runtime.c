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

static char* gb_copy_string(const char* value) {
    if (value == NULL) {
        value = "";
    }
    size_t length = strlen(value);
    char* result = (char*)gb_alloc(length + 1);
    memcpy(result, value, length + 1);
    return result;
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

void gb_print_int(int64_t value) { printf("%lld\n", (long long)value); }
void gb_print_float(double value) { printf("%.15g\n", value); }
void gb_print_bool(bool value) { printf("%s\n", value ? "true" : "false"); }
void gb_print_string(const char* value) { printf("%s\n", value != NULL ? value : "<null>"); }

char* gb_input(const char* prompt) {
    if (prompt != NULL && prompt[0] != '\0') {
        fputs(prompt, stdout);
        fflush(stdout);
    }
    size_t capacity = 128;
    size_t length = 0;
    char* temporary = (char*)malloc(capacity);
    if (temporary == NULL) {
        gb_panic("out of memory while reading input");
    }
    int ch;
    while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
        if (ch == '\r') { continue; }
        if (length + 1 >= capacity) {
            capacity *= 2;
            char* grown = (char*)realloc(temporary, capacity);
            if (grown == NULL) {
                free(temporary);
                gb_panic("out of memory while growing input buffer");
            }
            temporary = grown;
        }
        temporary[length++] = (char)ch;
    }
    temporary[length] = '\0';
    char* result = gb_copy_string(temporary);
    free(temporary);
    return result;
}

char* gb_env_get(const char* name) {
    if (name == NULL || name[0] == '\0') {
        return gb_copy_string("");
    }
    const char* value = getenv(name);
    return gb_copy_string(value != NULL ? value : "");
}

GbOptionString gb_env_get_option(const char* name) {
    GbOptionString result;
    result.has_value = false;
    result.value = "";
    if (name == NULL || name[0] == '\0') {
        return result;
    }
    const char* value = getenv(name);
    if (value != NULL) {
        result.has_value = true;
        result.value = gb_copy_string(value);
    }
    return result;
}

char* gb_fs_read_text(const char* path) {
    if (path == NULL || path[0] == '\0') {
        gb_panic("fs.read_text requires a non-empty path");
    }
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        gb_panic("could not open file for reading");
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        gb_panic("could not seek file");
    }
    long size = ftell(file);
    if (size < 0) {
        fclose(file);
        gb_panic("could not determine file size");
    }
    rewind(file);
    char* result = (char*)gb_alloc((size_t)size + 1);
    size_t read = fread(result, 1, (size_t)size, file);
    if (read != (size_t)size && ferror(file)) {
        fclose(file);
        gb_panic("could not read file");
    }
    result[read] = '\0';
    fclose(file);
    return result;
}

GbResultString gb_fs_try_read_text(const char* path) {
    GbResultString result;
    result.ok = false;
    result.value = "";
    result.error = "";
    if (path == NULL || path[0] == '\0') {
        result.error = gb_copy_string("fs.try_read_text requires a non-empty path");
        return result;
    }
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        result.error = gb_copy_string("could not open file for reading");
        return result;
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        result.error = gb_copy_string("could not seek file");
        return result;
    }
    long size = ftell(file);
    if (size < 0) {
        fclose(file);
        result.error = gb_copy_string("could not determine file size");
        return result;
    }
    rewind(file);
    char* text = (char*)gb_alloc((size_t)size + 1);
    size_t read = fread(text, 1, (size_t)size, file);
    if (read != (size_t)size && ferror(file)) {
        fclose(file);
        result.error = gb_copy_string("could not read file");
        return result;
    }
    text[read] = '\0';
    fclose(file);
    result.ok = true;
    result.value = text;
    return result;
}

void gb_fs_write_text(const char* path, const char* text) {
    if (path == NULL || path[0] == '\0') {
        gb_panic("fs.write_text requires a non-empty path");
    }
    if (text == NULL) { text = ""; }
    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        gb_panic("could not open file for writing");
    }
    size_t length = strlen(text);
    if (length > 0 && fwrite(text, 1, length, file) != length) {
        fclose(file);
        gb_panic("could not write file");
    }
    if (fclose(file) != 0) {
        gb_panic("could not close written file");
    }
}

GbResultBool gb_fs_try_write_text(const char* path, const char* text) {
    GbResultBool result;
    result.ok = false;
    result.value = false;
    result.error = "";
    if (path == NULL || path[0] == '\0') {
        result.error = gb_copy_string("fs.try_write_text requires a non-empty path");
        return result;
    }
    if (text == NULL) { text = ""; }
    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        result.error = gb_copy_string("could not open file for writing");
        return result;
    }
    size_t length = strlen(text);
    if (length > 0 && fwrite(text, 1, length, file) != length) {
        fclose(file);
        result.error = gb_copy_string("could not write file");
        return result;
    }
    if (fclose(file) != 0) {
        result.error = gb_copy_string("could not close written file");
        return result;
    }
    result.ok = true;
    result.value = true;
    return result;
}

_Noreturn void gb_process_exit(int64_t code) {
    gb_runtime_shutdown();
    exit((int)code);
}
