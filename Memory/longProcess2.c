#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
    printf("Process ID: %d\n", getpid());
    printf("This program will run indefinitely. Press Enter to exit.\n");

    // Allocate some memory to measure the memory usage, here we are allocating 100MB
    char *buffer = (char *)malloc(100 * 1024 * 1024);
    if (buffer == NULL) {
        perror("malloc");
        return 1;
    }

    // Initialize the allocated memory
    for (int i = 0; i < 100 * 1024 * 1024; i++) {
        buffer[i] = 'a';
    }

    // Memory map an anonymous page
    void *mapped_page = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapped_page == MAP_FAILED) {
        perror("mmap");
        free(buffer);
        return 1;
    }

    getchar(); // Wait for user input to exit

    // Clean up, that is, memory unmap
    if (munmap(mapped_page, 4096) == -1) {
        perror("munmap");
    }

    free(buffer);
    return 0;
}
