#include <stdlib.h>
#include <stdio.h>

#ifndef ALLOC_H
#define ALLOC_H

typedef struct Block {
    void* memory;
    size_t size;
    struct Block* next;
} Block;

typedef struct {
    Block* head;
    Block* freeList; 
    size_t size;
} Allocator;

Block* createBlock(void* addr, size_t size, Block* next) {
    Block* block = (Block*)malloc(sizeof(Block));
    if (!block) {
        return NULL;
    }

    block->memory = addr;
    block->size = size;
    block->next = next;

    return block;
}

Allocator* createAllocator() {
    Allocator* allocator = (Allocator*)malloc(sizeof(Allocator));
    if (!allocator) {
        return NULL;
    }

    allocator->head = NULL;
    allocator->freeList = NULL; 
    allocator->size = 0;

    return allocator;
}

void* alloc(Allocator* allocator, size_t size) {
    if (!allocator) {
        return NULL;
    }

    Block* freeBlock = allocator->freeList;
    Block* prev = NULL;
    while (freeBlock) {
        if (freeBlock->size >= size) {
            if (prev) {
                prev->next = freeBlock->next;
            } else {
                allocator->freeList = freeBlock->next;
            }
            void* mem = freeBlock->memory;
            free(freeBlock); 
            allocator->size += size;
            return mem;
        }
        prev = freeBlock;
        freeBlock = freeBlock->next;
    }

    void* mem = malloc(size);
    if (!mem) {
        return NULL;
    }

    Block* newBlock = createBlock(mem, size, NULL);
    if (!newBlock) {
        free(mem);
        return NULL;
    }

    if (!allocator->head) {
        allocator->head = newBlock;
    } else {
        Block* head = allocator->head;
        while (head->next != NULL) {
            head = head->next;
        }
        head->next = newBlock;
    }

    allocator->size += size;

    return mem;
}

void freeAlloc(Allocator* allocator) {
    if (!allocator) {
        return;
    }

    Block* head = allocator->head;
    Block* prev = NULL;

    allocator->head = NULL;

    while (head) {
        prev = head;
        head = head->next;
        prev->next = allocator->freeList;
        allocator->freeList = prev;
    }

    Block* freeBlock = allocator->freeList;
    while (freeBlock) {
        Block* next = freeBlock->next;
        free(freeBlock->memory);
        free(freeBlock);
        freeBlock = next;
    }

    free(allocator);
}

#endif

