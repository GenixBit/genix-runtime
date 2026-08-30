# Genix Runtime

The official runtime infrastructure for the **Genix programming language** by **GenixBit**.

> Status: pre-alpha. Runtime ABI version: **1**.

## Current runtime services

Native programs produced by `genix-lang` link against this repository.

Current ABI includes:

- `gb_runtime_init()` / `gb_runtime_shutdown()`
- `gb_alloc()`
- `gb_panic()`
- `gb_string_concat()` / `gb_string_equal()`
- `gb_print_int()` / `gb_print_float()` / `gb_print_bool()` / `gb_print_string()`
- `gb_input()`
- `gb_env_get()`
- `gb_fs_read_text()` / `gb_fs_write_text()`
- `gb_process_exit()`

Public header:

```text
include/genix/runtime.h
```

Portable C11 implementation:

```text
src/runtime.c
```

## Standard-library host bridge

The first OS-facing Genix standard-library APIs use this runtime boundary:

```text
io.input      → gb_input
fs.read_text  → gb_fs_read_text
fs.write_text → gb_fs_write_text
process.env   → gb_env_get
process.exit  → gb_process_exit
```

The Rust interpreter implements equivalent behavior for `gb run`, while native `gb build` emits the runtime calls above.

## Build and test

```bash
make
make test
```

`make` produces:

```text
build/libgenix_runtime.a
```

The runtime tests cover strings, tracked allocation lifecycle, filesystem text I/O, environment lookup fallback, and typed output.

## Compiler integration

```bash
export GENIX_RUNTIME=/path/to/genix-runtime
```

Native build pipeline:

```text
Genix source
   ↓
Genix frontend
   ↓
Typed Genix IR
   ↓
C11 backend
   ↓
generated application C
   +
Genix Runtime ABI
   ↓
native executable
```

The compiler validates `GENIX_RUNTIME_ABI_VERSION` before invoking the C compiler.

## Memory foundation

Runtime allocations are currently tracked and released at `gb_runtime_shutdown()`. This is a bootstrap model, not the final Genix ownership or memory-safety design.

## ABI policy

`GENIX_RUNTIME_ABI_VERSION` is currently `1`. Because ABI v1 is still pre-alpha, additive symbols may be introduced without changing the number when existing contracts stay compatible. Breaking symbol or representation changes require an ABI increment.

See `ABI.md` for the detailed contract.

## Repository layout

```text
include/genix/runtime.h
src/runtime.c
tests/runtime_test.c
ABI.md
Makefile
.github/workflows/ci.yml
```

## Next runtime milestones

- Structured I/O errors instead of fatal panic behavior
- Stable string representation
- Allocator abstraction and ownership model
- Path/platform abstractions
- Time and process APIs
- Networking primitives
- Threads and concurrency
- Windows/macOS/Linux portability CI

---

**Genix — by GenixBit**
