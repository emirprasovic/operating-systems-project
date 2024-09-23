#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    getchar(); // Wait for user input to exit

    free(buffer);
    return 0;
}
