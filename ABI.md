# Genix Runtime ABI v1

This document defines the current pre-alpha ABI contract between generated Genix native code and `genix-runtime`.

## Version

```c
#define GENIX_RUNTIME_ABI_VERSION 1
```

ABI v1 is still pre-alpha. New symbols may be added without changing the ABI number when existing signatures and representations remain compatible. Breaking changes to existing ABI contracts must increment the version.

## Lifecycle

```c
void gb_runtime_init(void);
void gb_runtime_shutdown(void);
```

Generated executables initialize the runtime before Genix `main` and shut it down after `main` returns.

## Allocation and panic

```c
void* gb_alloc(size_t size);
_Noreturn void gb_panic(const char* message);
```

The bootstrap allocator tracks allocations until runtime shutdown. Individual deallocation is intentionally not public until the Genix ownership model is finalized.

## Strings

The current bootstrap native ABI represents strings as null-terminated UTF-8-compatible C string pointers.

```c
char* gb_string_concat(const char* left, const char* right);
bool gb_string_equal(const char* left, const char* right);
```

The representation is temporary and may change in a future breaking ABI revision.

## Typed output

```c
void gb_print_int(int64_t value);
void gb_print_float(double value);
void gb_print_bool(bool value);
void gb_print_string(const char* value);
```

These functions back native Genix `print(...)` and the first `io` standard-library APIs.

## Host services

ABI v1 now includes additive host-service functions used by the bootstrap standard-library intrinsic boundary.

### Standard input

```c
char* gb_input(const char* prompt);
```

Writes the optional prompt, reads one line from standard input, strips the line ending, and returns a runtime-managed string.

### Environment

```c
char* gb_env_get(const char* name);
```

Returns a runtime-managed copy of the environment-variable value. A missing variable currently returns an empty string because Genix does not yet have `Option`/nullable result types.

### Filesystem text I/O

```c
char* gb_fs_read_text(const char* path);
void gb_fs_write_text(const char* path, const char* text);
```

`gb_fs_read_text` returns runtime-managed text. Bootstrap file failures terminate through `gb_panic`; structured `Result`-based I/O is planned after language error types are implemented.

### Process control

```c
_Noreturn void gb_process_exit(int64_t code);
```

Performs runtime shutdown and terminates the process with the supplied exit status.

## Standard-library mapping

The current native compiler lowers these official Genix APIs directly to runtime symbols:

| Genix API | Runtime ABI |
|---|---|
| `io.input(prompt)` | `gb_input` |
| `fs.read_text(path)` | `gb_fs_read_text` |
| `fs.write_text(path, text)` | `gb_fs_write_text` |
| `process.env(name)` | `gb_env_get` |
| `process.exit(code)` | `gb_process_exit` |

This is a bootstrap compiler/runtime boundary, not the final general-purpose FFI design.

## Primitive ABI mapping

| Genix type | Runtime/native ABI |
|---|---|
| `int` | `int64_t` |
| `float` | `double` |
| `bool` | `bool` |
| `string` | `const char*` |
| void | `void` |

## Compatibility rules

- `genix-lang` validates `GENIX_RUNTIME_ABI_VERSION` before native compilation.
- `genix-stdlib/COMPATIBILITY` declares the runtime ABI it expects.
- Adding new symbols is allowed during the pre-alpha ABI v1 period when existing contracts remain compatible.
- Changing/removing existing symbols or representations requires an ABI version increment.
- Cross-repository CI builds and executes programs against the current compiler, runtime, and standard library.

---

**Genix — by GenixBit**
