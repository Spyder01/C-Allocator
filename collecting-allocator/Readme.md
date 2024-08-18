# Collecting Allocator

The Collecting Allocator is a custom memory allocator designed to manage and track allocated memory blocks. It provides basic memory allocation and deallocation functionality, keeping track of all allocated blocks and allowing for their collective management.

## Features

- **Dynamic Allocation:** Allocates memory blocks of varying sizes.
- **Memory Tracking:** Keeps track of all allocated blocks and their sizes.
- **Automatic Deallocation:** Frees all allocated memory blocks when the allocator is deallocated.

## Data Structures

### `Block`

The `Block` structure represents an individual memory block managed by the allocator. It contains:
- `void* memory`: Pointer to the allocated memory.
- `size_t size`: Size of the allocated memory block.
- `struct Block* next`: Pointer to the next block in the list.

### `Allocator`

The `Allocator` structure manages the collection of memory blocks:
- `Block* head`: Pointer to the first block in the list.
- `size_t size`: Total size of all allocated memory blocks.

## Functions

### `Block* createBlock(void* addr, size_t size, Block* next)`

Creates a new `Block` with the specified memory address, size, and next block pointer.

**Parameters:**
- `void* addr`: Memory address of the block.
- `size_t size`: Size of the block.
- `Block* next`: Pointer to the next block in the list.

**Returns:**
- `Block*`: Pointer to the newly created block, or `NULL` if allocation fails.

### `Allocator* createAllocator()`

Creates a new `Allocator` instance.

**Returns:**
- `Allocator*`: Pointer to the newly created allocator, or `NULL` if allocation fails.

### `void* alloc(Allocator* allocator, size_t size)`

Allocates memory of the specified size and tracks it using the `Allocator`.

**Parameters:**
- `Allocator* allocator`: Pointer to the allocator instance.
- `size_t size`: Size of the memory to allocate.

**Returns:**
- `void*`: Pointer to the allocated memory, or `NULL` if allocation fails.

### `void freeAlloc(Allocator* allocator)`

Frees all allocated memory blocks and deallocates the `Allocator` instance.

**Parameters:**
- `Allocator* allocator`: Pointer to the allocator instance.

## Example Usage

### Simple Linked List

This example demonstrates how to use the Collecting Allocator to create and manage a simple linked list.

```c
#include "alloc.h"
#include <stdio.h>

// Define the structure for a linked list node
typedef struct Node {
    int value;
    struct Node* next;
} Node;

// Function to create a new node
Node* createNode(Allocator* allocator, int value) {
    // Allocate memory for the node using the allocator
    Node* node = (Node*)alloc(allocator, sizeof(Node));
    if (!node) {
        return NULL;
    }
    
    // Initialize the node
    node->value = value;
    node->next = NULL;
    
    return node;
}

// Function to print the linked list
void printList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    // Create the allocator
    Allocator* allocator = createAllocator();
    if (!allocator) {
        fprintf(stderr, "Failed to create allocator\n");
        return 1;
    }

    // Create nodes for the linked list
    Node* head = createNode(allocator, 1);
    Node* second = createNode(allocator, 2);
    Node* third = createNode(allocator, 3);

    if (!head || !second || !third) {
        fprintf(stderr, "Failed to allocate memory for nodes\n");
        freeAlloc(allocator);
        return 1;
    }

    // Link the nodes
    head->next = second;
    second->next = third;

    // Print the linked list
    printList(head);

    // Free all allocated memory
    freeAlloc(allocator);
    
    return 0;
}
```

### Explanation

1. **Define the Node Structure:** A `Node` structure represents each element in the linked list. It includes an integer value and a pointer to the next node.

2. **Create Node Function:** The `createNode` function uses the `alloc` function from the allocator to allocate memory for a new node. It initializes the node's value and sets its `next` pointer to `NULL`.

3. **Print List Function:** The `printList` function traverses the linked list and prints the value of each node.

4. **Main Function:**
   - Create an `Allocator` instance.
   - Allocate memory for three nodes in the linked list.
   - Link the nodes together to form the list.
   - Print the linked list to verify the result.
   - Free all allocated memory using the `freeAlloc` function.

