#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGEMAP_LENGTH 8
#define GET_PFN(X) ((X) & 0x7FFFFFFFFFFFFF)

uint64_t virtual_to_physical(void *virtual_addr) {
    uint64_t value;
    uint64_t offset = (uint64_t)virtual_addr / PAGE_SIZE * PAGEMAP_LENGTH;
    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd < 0) {
        perror("open pagemap");
        return 0;
    }

    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("lseek pagemap");
        close(fd);
        return 0;
    }

    if (read(fd, &value, PAGEMAP_LENGTH) != PAGEMAP_LENGTH) {
        perror("read pagemap");
        close(fd);
        return 0;
    }

    close(fd);

    if (!(value & (1ULL << 63))) {
        return 0;
    }

    uint64_t pfn = GET_PFN(value);
    uint64_t phys_addr = (pfn * PAGE_SIZE) + ((uint64_t)virtual_addr % PAGE_SIZE);
    return phys_addr;
}

int main() {
    void *shared_mem;
    pid_t pid;

    shared_mem = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    pid = fork();
    if (pid == 0) {
        uint64_t phys_addr = virtual_to_physical(shared_mem);
        printf("Child Process:\n");
        printf("Virtual address of shared memory: %p\n", shared_mem);
        printf("Physical address of shared memory: 0x%lx\n", phys_addr);
    } else if (pid > 0) {
        uint64_t phys_addr = virtual_to_physical(shared_mem);
        printf("Parent Process:\n");
        printf("Virtual address of shared memory: %p\n", shared_mem);
        printf("Physical address of shared memory: 0x%lx\n", phys_addr);
        wait(NULL);
    } else {
        perror("fork");
        return 1;
    }

    return 0;
}

