#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#define PAGE_SIZE 4096
#define ENTRY_SIZE 8
#define PFN_MASK 0x7FFFFFFFFFFFFF

uint64_t get_phys_addr(void *vaddr) {
    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 0;
    }

    uint64_t offset = ((uint64_t)vaddr / PAGE_SIZE) * ENTRY_SIZE;
    uint64_t entry;

    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return 0;
    }

    if (read(fd, &entry, ENTRY_SIZE) != ENTRY_SIZE) {
        perror("read");
        close(fd);
        return 0;
    }

    close(fd);

    // Check if page is present
    if (!(entry & (1ULL << 63))) {
        fprintf(stderr, "Page not present\n");
        return 0;
    }

    uint64_t pfn = entry & PFN_MASK;
    return (pfn * PAGE_SIZE) + ((uint64_t)vaddr % PAGE_SIZE);
}

void convert() {
    int num = 123;
    int *ptr = malloc(sizeof(int));
    if (!ptr) {
        perror("malloc");
        return;
    }
    *ptr = 456;

    printf("Virtual address of num:      %p\n", (void *)&num);
    printf("Virtual address of ptr var:  %p\n", (void *)&ptr);
    printf("Virtual address malloc'd:    %p\n", (void *)ptr);

    uint64_t phys_num = get_phys_addr(&num);
    uint64_t phys_ptr = get_phys_addr(&ptr);
    uint64_t phys_heap = get_phys_addr(ptr);

    printf("Physical address of num:     0x%lx\n", phys_num);
    printf("Physical address of ptr var: 0x%lx\n", phys_ptr);
    printf("Physical address malloc'd:   0x%lx\n", phys_heap);

    free(ptr);
}

int main() {
    convert();
    return 0;
}
