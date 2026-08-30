# Genix Runtime ABI v1

This document defines the current pre-alpha ABI contract between generated Genix native code and `genix-runtime`.

## Version

```c
#define GENIX_RUNTIME_ABI_VERSION 1
```

The ABI is experimental until Genix reaches a stable language/runtime release.

## Lifecycle

```c
void gb_runtime_init(void);
void gb_runtime_shutdown(void);
```

Generated executables call `gb_runtime_init()` before the Genix `main` function and `gb_runtime_shutdown()` after it returns.

Both functions are safe for the current single-process bootstrap runtime. Shutdown releases allocations tracked by `gb_alloc()`.

## Allocation

```c
void* gb_alloc(size_t size);
```

`gb_alloc` returns runtime-managed storage. Allocation failure terminates through `gb_panic`.

The v1 runtime tracks allocations until shutdown. Individual deallocation is not exposed yet because the final Genix ownership model has not been finalized.

## Panic

```c
_Noreturn void gb_panic(const char* message);
```

`gb_panic` writes a Genix runtime panic message to standard error, performs runtime shutdown, and exits with status code `70`.

## Strings

The current native ABI represents a Genix string as a UTF-8-compatible null-terminated C string pointer:

```c
const char*
```

This representation is temporary and may change when the stable Genix string/ownership model is introduced.

Runtime functions:

```c
char* gb_string_concat(const char* left, const char* right);
bool gb_string_equal(const char* left, const char* right);
```

Concatenated strings are allocated through `gb_alloc` and are released by runtime shutdown.

## Output

```c
void gb_print_int(int64_t value);
void gb_print_float(double value);
void gb_print_bool(bool value);
void gb_print_string(const char* value);
```

These functions define the first portable output boundary used by Genix's `print(...)` operation in native builds.

## Primitive ABI mapping

| Genix type | Runtime/native ABI |
|---|---|
| `int` | `int64_t` |
| `float` | `double` |
| `bool` | `bool` |
| `string` | `const char*` |
| void | `void` |

## Stability rules

During pre-alpha development:

- ABI changes may be breaking.
- `genix-lang` and `genix-runtime` must be updated together when the ABI changes.
- Changes to public symbols should increment `GENIX_RUNTIME_ABI_VERSION` when compatibility is broken.
- Compiler CI should build and execute native programs against the current runtime repository.

---

**Genix — by GenixBit**
