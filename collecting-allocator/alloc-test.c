#include "collecting-allocator.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_createBlock() {
    void* addr = malloc(10);
    Block* block = createBlock(addr, 10, NULL);

    assert(block != NULL);
    assert(block->memory == addr);
    assert(block->size == 10);
    assert(block->next == NULL);

    free(block->memory);
    free(block);
}

void test_createAllocator() {
    Allocator* allocator = createAllocator();

    assert(allocator != NULL);
    assert(allocator->head == NULL);
    assert(allocator->size == 0);

    free(allocator);
}

void test_alloc() {
    Allocator* allocator = createAllocator();
    assert(allocator != NULL);

    void* ptr1 = alloc(allocator, 50);
    void* ptr2 = alloc(allocator, 100);

    assert(ptr1 != NULL);
    assert(ptr2 != NULL);
    assert(allocator->size == 150);

    Block* current = allocator->head;
    while (current != NULL) {
        if (current->memory == ptr1 || current->memory == ptr2) {
            assert(current->size == 50 || current->size == 100);
        }
        current = current->next;
    }

    freeAlloc(allocator);
}

void test_freeAlloc() {
    Allocator* allocator = createAllocator();
    assert(allocator != NULL);

    void* ptr1 = alloc(allocator, 30);
    void* ptr2 = alloc(allocator, 60);

    freeAlloc(allocator);

    // Check if allocator is deallocated properly
    // After this, allocator should be freed and not accessible
    // So we should not access allocator->head or allocator->size.

    // No assertion needed here since the allocator should be cleaned up.
    // You could check memory leaks with a tool like Valgrind to ensure cleanup is correct.
}

int main() {
    test_createBlock();
    test_createAllocator();
    test_alloc();
    test_freeAlloc();

    printf("All tests passed!\n");
    return 0;
}

