#include "genix/runtime.h"

#include <assert.h>
#include <string.h>

int main(void) {
    gb_runtime_init();

    char* message = gb_string_concat("Hello ", "Genix");
    assert(strcmp(message, "Hello Genix") == 0);
    assert(gb_string_equal("Genix", "Genix"));
    assert(!gb_string_equal("Genix", "Other"));

    gb_print_string(message);
    gb_print_int(42);
    gb_print_float(3.5);
    gb_print_bool(true);

    gb_runtime_shutdown();
    return 0;
}
