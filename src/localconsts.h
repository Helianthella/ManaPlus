/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <SDL_version.h>

#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef USE_SDL2
#warning using SDL2 headers but configure set to use SDL1.2
#warning please add configure flag --with-sdl2
#define USE_SDL2
#endif
#else
#ifdef USE_SDL2
#error using SDL1.2 headers but configure set to use SDL2
#error please remove configure flag --with-sdl2
#endif
#endif

#define GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 \
    + __GNUC_PATCHLEVEL__)

#if !defined(__GXX_EXPERIMENTAL_CXX0X__)
#undef nullptr
#define nullptr 0
#define final
#define override
#define constexpr
#define constexpr2
#define noexcept
#define A_DELETE(func)
#define A_DELETE_COPY(func)
#else  // !defined(__GXX_EXPERIMENTAL_CXX0X__)
#if GCC_VERSION < 40700
#define final
#define override
#define constexpr
#define noexcept
// #define A_DELETE
// #define A_DELETE_COPY
#else  // GCC_VERSION < 40700
#define ADVGCC
#if GCC_VERSION < 40900
#define constexpr2
#else  // GCC_VERSION < 40900
#if __cpp_constexpr > 201309
#define constexpr2 constexpr
#else  // __cpp_constexpr > 201309
#define constexpr2
#endif  // __cpp_constexpr > 201309
#endif  // GCC_VERSION < 40900
#endif  // GCC_VERSION < 40700
#undef Z_NULL
#define Z_NULL nullptr
#define M_TCPOK
#define A_DELETE(func) func = delete
#define A_DELETE_COPY(name) name(const name &) = delete; \
    name &operator=(const name&) = delete;
#endif  // !defined(__GXX_EXPERIMENTAL_CXX0X__)

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#define A_WARN_UNUSED __attribute__ ((warn_unused_result))
#define DEPRECATED __attribute__ ((deprecated))
#define restrict __restrict__

#ifdef __INTEL_COMPILER
#define RETURNS_NONNULL
#else  // __INTEL_COMPILER
#if GCC_VERSION < 40900
#define RETURNS_NONNULL
#else  // GCC_VERSION < 40900
#define RETURNS_NONNULL __attribute__((returns_nonnull))
#endif  // GCC_VERSION < 40900
#endif  // __INTEL_COMPILER

#ifndef ENABLE_CILKPLUS
#define A_NONNULL(...) __attribute__((nonnull (__VA_ARGS__)))
#else
#define A_NONNULL(...)
#endif

#else
#define A_UNUSED
#define A_WARN_UNUSED
#define gnu_printf printf
#define DEPRECATED
#define restrict
#define RETURNS_NONNULL
#define A_NONNULL(...)
#endif
#ifdef __clang__
#define gnu_printf printf
#endif

#ifdef ENABLE_CILKPLUS
#include <cilk/cilk.h>
#endif

#ifdef ADVGCC
#define const2 const
#else  // ADVGCC
#define const2
#endif  // ADVGCC

#ifdef __GNUC__
#ifdef ENABLE_CILKPLUS
#if GCC_VERSION < 40900
#define cilk_for for
#define cilk_spawn
#define cilk_sync
#elif GCC_VERSION < 50000
#ifdef cilk_for
#undef cilk_for
#endif  // cilk_for
#define cilk_for for
#endif  // GCC_VERSION < 40900
#else  // ENABLE_CILKPLUS
#define cilk_for for
#define cilk_spawn
#define cilk_sync
#endif  // ENABLE_CILKPLUS
#endif  // __GNUC__

#define notfinal

#define FOR_EACH(type, iter, array) for (type iter = array.begin(), \
    iter##_end = array.end(); iter != iter##_end; ++ iter)

#define FOR_EACHR(type, iter, array) for (type iter = array.rbegin(), \
    iter##_end = array.rend(); iter != iter##_end; ++ iter)

#define FOR_EACHP(type, iter, array) for (type iter = array->begin(), \
    iter##_end = array->end(); iter != iter##_end; ++ iter)

#ifdef ENABLE_CHECKPLUGIN
#define A_NONNULLPOINTER __attribute__((nonnullpointer))
#else
#define A_NONNULLPOINTER
#endif

#ifdef ENABLE_CHECKS

#define CHECKLISTENERS \
    config.checkListeners(this, __FILE__, __LINE__); \
    serverConfig.checkListeners(this, __FILE__, __LINE__);

#else  // ENABLE_CHECKS

#define CHECKLISTENERS

#endif  // ENABLE_CHECKS

// #define DEBUG_CONFIG 1
// #define DEBUG_BIND_TEXTURE 1
// #define DISABLE_RESOURCE_CACHING 1
#define DUMP_LEAKED_RESOURCES 1
// #define DEBUG_DUMP_LEAKS1 1
// #define DEBUG_SDLFONT 1

// Android logging
// #define ANDROID_LOG 1

// nacl logging
#define NACL_LOG 1

// profiler
// #define USE_PROFILER 1

// draw calls
// #define DEBUG_DRAW_CALLS 1

// debug images usage
// #define DEBUG_IMAGES 1

// debug SDL surfaces
// #define DEBUG_SDL_SURFACES 1

// debug RWops usage
// #define DEBUG_PHYSFS 1

// use file access fuzzer
// #define USE_FUZZER 1

// use OpenGL debug features
// #define DEBUG_OPENGL 1

#ifdef DYECMD
#undef USE_FUZZER
#endif
#include "utils/perfomance.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
