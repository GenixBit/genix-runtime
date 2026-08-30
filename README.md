# Genix Runtime

The official runtime infrastructure for the **Genix programming language** by **GenixBit**.

> Status: early development / pre-alpha.

## Scope

This repository will provide the low-level runtime services required by compiled Genix programs, including:

- Program startup and shutdown
- Memory allocation interfaces
- Panic and fatal-error handling
- Concurrency/runtime scheduling primitives
- Platform abstraction
- Environment and process integration
- Native system calls and runtime bridges
- Runtime support for strings, collections, and other language-level values where required

## Design principles

The runtime should remain small, predictable, portable, and easy for the compiler to target. Public runtime ABI decisions will be documented before they are considered stable.

## Relationship to other repositories

- `genix-lang` emits code that targets runtime services.
- `genix-stdlib` exposes higher-level APIs that may use this runtime.
- `genix-docs` documents stable runtime behavior and language semantics.

## Initial roadmap

1. Define runtime ABI boundaries.
2. Establish platform-independent core types.
3. Add panic/error support.
4. Add allocator abstraction.
5. Add threading/concurrency primitives.
6. Integrate with the Genix compiler backend.

---

**Genix — by GenixBit**
