# 2. (50 pts) Shared Memory and Address Conversion in C

## Question

Create two processes. Share a piece of memory allocated from the heap. Examine the virtual addresses and the physical addresses of this shared memory in the two processes respectively. Summarize your observations.

## Answer

### Methods
We created the shared memory using `mmap` and then looked at the virtual addresses and then converted them to physical addresses.

1. Allocating shared memory using `mmap`.
2. Forking the process to create a parent and child process.
3. Each process accessing and printing the virtual address and attempting to convert it to a physical address.
4. Outputing the results of the address conversions for both processes.

We used out previous code for converting virtual addresses to physical addresses.

**Command**:
```bash
gcc -g -O0 -o SharedMemory SharedMemory.c

./SharedMemory
```
### Results
Parent Process:
Virtual address of shared memory: 0x7f91f850b000 
Physical address of shared memory: 0x1c58000

Child Process:
Virtual address of shared memory: 0x7f91f850b000 
Physical address of shared memory: 0x1c58000

### Explanation of Observations

- **Virtual Address**: Both the parent and child processes see the same virtual address for the shared memory, as expected. This is because `mmap` shares the same memory space between processes.
- **Physical Address**: The physical address is the same for both processes. This is expected, as both processes are accessing the same piece of memory and map to the same physical location in RAM.

### Reference
- **Linux `mmap` Documentation**: [mmap - Linux manual page](https://man7.org/linux/man-pages/man2/mmap.2.html)
- **Linux `pagemap` Documentation**: [pagemap Documentation - Kernel.org](https://www.kernel.org/doc/html/latest/admin-guide/mm/pagemap.html)

