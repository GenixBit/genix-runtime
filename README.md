# Genix Runtime

The official runtime infrastructure for the **Genix programming language** by **GenixBit**.

> Status: pre-alpha. Runtime ABI version: **1**.

## What exists today

The runtime is now an executable part of the Genix toolchain rather than only a roadmap. Native programs produced by `genix-lang` link against this repository.

Current ABI services include:

- `gb_runtime_init()` / `gb_runtime_shutdown()` lifecycle hooks
- `gb_alloc()` tracked runtime allocation
- `gb_panic()` fatal runtime errors
- `gb_string_concat()`
- `gb_string_equal()`
- `gb_print_int()`
- `gb_print_float()`
- `gb_print_bool()`
- `gb_print_string()`

The public C header is:

```text
include/genix/runtime.h
```

The portable C11 implementation is:

```text
src/runtime.c
```

## Build and test

```bash
make
make test
```

`make` produces:

```text
build/libgenix_runtime.a
```

## Compiler integration

The Genix compiler can locate the runtime through:

```bash
export GENIX_RUNTIME=/path/to/genix-runtime
```

A native Genix build currently compiles generated C together with `src/runtime.c` and includes `include/` through the C compiler.

Conceptually:

```text
.gb source
   ↓
Genix compiler
   ↓
Genix IR
   ↓
C11 backend
   ↓
generated application C
   +
genix-runtime/src/runtime.c
   ↓
native executable
```

## Memory foundation

Runtime-owned allocations are tracked and released during `gb_runtime_shutdown()`. This is a bootstrap memory model intended to prevent generated string-concatenation allocations from being permanently unmanaged while the final Genix ownership/memory-safety model is designed.

This is **not yet** the final memory model for the language.

## ABI policy

`GENIX_RUNTIME_ABI_VERSION` is currently `1`.

The ABI is still experimental. Breaking changes are allowed during pre-alpha development and must be coordinated with `genix-lang`.

See `ABI.md` for the current contract.

## Repository layout

```text
include/genix/runtime.h
src/runtime.c
tests/runtime_test.c
Makefile
.github/workflows/ci.yml
```

## Next runtime milestones

- Version compatibility checks between compiler and runtime
- Stable string representation
- Allocator abstraction and ownership model
- File/process/platform services
- Runtime diagnostics
- Threads and concurrency primitives
- Standard-library bridges
- Windows/macOS/Linux portability CI

---

**Genix — by GenixBit**
