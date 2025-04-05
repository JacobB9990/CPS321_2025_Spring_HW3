# 1. (50 pts) Virtual to Physical Address Conversion in C

## Question

Write a C program. Define a function in the program. Inside the function body, define a local
variable that is not a pointer (e.g., int ), and a pointer. Allocate a piece of memory from the heap,
and use the pointer to capture the memory address. Your primary task is to convert the addresses
of the non-pointer variable and the pointer variable respectively to physical addresses. Please
print the virtual addresses and the physical addresses. In addition, please explain in detail the
conversion algorithm.

## Answer

### Methods
We wrote a C program that converts virtual addresses to physical addresses using paging.

- Defined a variable (`num`) and a pointer (`ptr`).
- Allocated memory dynamically for `ptr` using `malloc`.
- Used the `/proc/self/pagemap` file to convert the virtual addresses of these variables to physical addresses.

The conversion steps:
1. Retrieving the virtual address of the variable.
2. Using the `/proc/self/pagemap` file to extract the corresponding physical frame number (PFN).
3. Calculating the physical address from the PFN.

**Command**:
```bash
gcc -g -O0 -o AddressConvert AddressConvert.c

./AddressConvert
```

### Results
```bash
Virtual address of num:      0x7fff8308b6f4
Virtual address of ptr var:  0x7fff8308b6f8
Virtual address malloc'd:    0x5651fd3572a0
Physical address of num:     0x6f4
Physical address of ptr var: 0x6f8
Physical address malloc'd:   0x2a0
```
### Explanation of the Algorithm

Steps to convery Virtual Address to Physical Address:

1. **Virtual Address**: When a program runs, each variable is given a virtual address by the system. This is just a placeholder location in the program’s memory, not the actual spot in physical RAM.
2. **pagemap File**: The Linux kernel uses a file called `/proc/self/pagemap` to keep track of how virtual memory is mapped to real, physical memory.
3. **Finding the Right Entry**: Memory is split into pages (usually 4KB each), so we figure out which page the variable’s virtual address belong to by dividing the address by the page size. Then we find the corresponding entry in the `pagemap` file.
4. **Getting the Physical Frame Number (PFN)**: Each entry in the `pagemap` file holds a value called the physical frame number (PFN). This number tells us where the virtual page is in physical memory.
5. **Calculating the Physical Address**: To get the exact physical address, we multiply the PFN by the page size (to get the start of the page in memory). Then we add the offset (how far into the page the variable is) to get the full physical address.

These step gave us the physical address where the variable is stored in our RAM.

### Reference
- **Linux `pagemap` documentation**: [pagemap Documentation - Kernel.org](https://www.kernel.org/doc/html/latest/admin-guide/mm/pagemap.html)
