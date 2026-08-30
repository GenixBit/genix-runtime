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

## Option carriers

Primitive Genix `Option<T>` values currently lower to tagged ABI structures:

```c
typedef struct { bool has_value; int64_t value; } GbOptionInt;
typedef struct { bool has_value; double value; } GbOptionFloat;
typedef struct { bool has_value; bool value; } GbOptionBool;
typedef struct { bool has_value; const char* value; } GbOptionString;
```

`has_value == true` represents `Some(value)`. `has_value == false` represents `None`.

These structures are bootstrap representations and may be generalized when Genix gains arbitrary generic types and user-defined enums.

## Result carriers

Primitive `Result<T,string>` values lower to:

```c
typedef struct { bool ok; int64_t value; const char* error; } GbResultInt;
typedef struct { bool ok; double value; const char* error; } GbResultFloat;
typedef struct { bool ok; bool value; const char* error; } GbResultBool;
typedef struct { bool ok; const char* value; const char* error; } GbResultString;
```

`ok == true` represents `Ok(value)`. `ok == false` represents `Err(error)`.

The native compiler uses these tags to lower exhaustive `match` and statement-level `?` propagation without relying on platform-specific C extensions.

## Typed output

```c
void gb_print_int(int64_t value);
void gb_print_float(double value);
void gb_print_bool(bool value);
void gb_print_string(const char* value);
```

## Host services

### Standard input

```c
char* gb_input(const char* prompt);
```

### Environment

Legacy bootstrap lookup:

```c
char* gb_env_get(const char* name);
```

Preferred safe lookup:

```c
GbOptionString gb_env_get_option(const char* name);
```

The safe function returns `None` when the variable does not exist instead of conflating absence with an empty value.

### Filesystem text I/O

Legacy panic-on-error functions remain temporarily available:

```c
char* gb_fs_read_text(const char* path);
void gb_fs_write_text(const char* path, const char* text);
```

Preferred recoverable APIs:

```c
GbResultString gb_fs_try_read_text(const char* path);
GbResultBool gb_fs_try_write_text(const char* path, const char* text);
```

Filesystem failures are returned through `Err(string)` instead of terminating the process.

### Process control

```c
_Noreturn void gb_process_exit(int64_t code);
```

## Standard-library mapping

| Genix API | Runtime ABI |
|---|---|
| `io.input(prompt)` | `gb_input` |
| `process.env(name)` | `gb_env_get` |
| `process.env_option(name)` | `gb_env_get_option` |
| `fs.read_text(path)` | `gb_fs_read_text` |
| `fs.try_read_text(path)` | `gb_fs_try_read_text` |
| `fs.write_text(path, text)` | `gb_fs_write_text` |
| `fs.try_write_text(path, text)` | `gb_fs_try_write_text` |
| `process.exit(code)` | `gb_process_exit` |

The direct mapping is a bootstrap compiler/runtime boundary, not the final general-purpose FFI design.

## Primitive ABI mapping

| Genix type | Runtime/native ABI |
|---|---|
| `int` | `int64_t` |
| `float` | `double` |
| `bool` | `bool` |
| `string` | `const char*` |
| `Option<int>` | `GbOptionInt` |
| `Option<float>` | `GbOptionFloat` |
| `Option<bool>` | `GbOptionBool` |
| `Option<string>` | `GbOptionString` |
| `Result<int,string>` | `GbResultInt` |
| `Result<float,string>` | `GbResultFloat` |
| `Result<bool,string>` | `GbResultBool` |
| `Result<string,string>` | `GbResultString` |
| void | `void` |

## Compatibility rules

- `genix-lang` validates `GENIX_RUNTIME_ABI_VERSION` before native compilation.
- `genix-stdlib/COMPATIBILITY` declares the runtime ABI it expects.
- Adding new symbols is allowed during the pre-alpha ABI v1 period when existing contracts remain compatible.
- Changing/removing existing symbols or representations requires an ABI version increment.
- Cross-repository CI builds and executes programs against the current compiler, runtime, and standard library.

---

**Genix — by GenixBit**
