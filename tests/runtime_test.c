#include "genix/runtime.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    gb_runtime_init();

    char* message = gb_string_concat("Hello ", "Genix");
    assert(strcmp(message, "Hello Genix") == 0);
    assert(gb_string_equal("Genix", "Genix"));
    assert(!gb_string_equal("Genix", "Other"));

    const char* test_path = "genix_runtime_test.txt";
    gb_fs_write_text(test_path, "runtime file services work");
    char* contents = gb_fs_read_text(test_path);
    assert(strcmp(contents, "runtime file services work") == 0);
    remove(test_path);

    const char* safe_path = "genix_runtime_safe_test.txt";
    GbResultBool write_result = gb_fs_try_write_text(safe_path, "safe runtime IO");
    assert(write_result.ok);
    assert(write_result.value);

    GbResultString read_result = gb_fs_try_read_text(safe_path);
    assert(read_result.ok);
    assert(strcmp(read_result.value, "safe runtime IO") == 0);
    remove(safe_path);

    GbResultString missing_file = gb_fs_try_read_text("genix_runtime_missing_6F9E.txt");
    assert(!missing_file.ok);
    assert(missing_file.error != NULL);
    assert(strlen(missing_file.error) > 0);

    char* missing_env = gb_env_get("GENIX_RUNTIME_TEST_UNLIKELY_TO_EXIST_6F9E");
    assert(strcmp(missing_env, "") == 0);

    GbOptionString optional_env = gb_env_get_option("GENIX_RUNTIME_TEST_UNLIKELY_TO_EXIST_6F9E");
    assert(!optional_env.has_value);

    GbOptionInt some_int = { .has_value = true, .value = 42 };
    assert(some_int.has_value && some_int.value == 42);

    GbResultString ok_string = { .ok = true, .value = "Genix", .error = "" };
    assert(ok_string.ok && strcmp(ok_string.value, "Genix") == 0);

    gb_print_string(message);
    gb_print_int(42);
    gb_print_float(3.5);
    gb_print_bool(true);

    gb_runtime_shutdown();
    return 0;
}
