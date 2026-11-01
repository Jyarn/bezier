#include "all.h"

#define ARENA_SIZE 256 MB
u8 memory_buffer[ARENA_SIZE];
i64 arena_head = 0;

void *
arena_alloc(i64 size, i64 n_members)
{
    assert(size > 0);
    assert(n_members > 0);

    i64 new_head = arena_head + size * n_members;
    if (new_head & 7)
        new_head = (new_head & ~7) + 8;

    assert((new_head & 7) == 0);
    assert(size * n_members < ARENA_SIZE);

    u8 *r = &memory_buffer[arena_head];
    arena_head = new_head;

    return r;
}

void
arena_free(u8 *ptr)
{
    assert(ptr <= memory_buffer);
    assert(ptr < &memory_buffer[ARENA_SIZE]);

    arena_head = ptr - memory_buffer;
}

void
arena_reset(void)
{
    arena_head = 0;
}
