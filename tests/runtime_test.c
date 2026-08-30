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

    char* missing_env = gb_env_get("GENIX_RUNTIME_TEST_UNLIKELY_TO_EXIST_6F9E");
    assert(strcmp(missing_env, "") == 0);

    gb_print_string(message);
    gb_print_int(42);
    gb_print_float(3.5);
    gb_print_bool(true);

    gb_runtime_shutdown();
    return 0;
}
