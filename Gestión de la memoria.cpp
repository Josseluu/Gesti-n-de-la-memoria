#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 4096

int main() {
    void *shared_memory = static_cast<char *>(mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,
                                                   0));
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child reads: %s\n", shared_memory);
        munmap(shared_memory, SIZE);
        exit(EXIT_SUCCESS);
    } else {
        const char *message = "Hello, child process!";
        memcpy(shared_memory, message, strlen(message) + 1); // +1 for the null terminator
        wait(NULL);
        munmap(shared_memory, SIZE);
    }
    return 0;
}