#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// thread function to print a message
void* print_message(void* arg) {
    char* message = (char*)arg;
    printf("%s\n", message);
    return NULL;
}

int main() {
    pthread_t thread1, thread2, thread3;
    char* message1 = "Thread 1 is running";
    char* message2 = "Thread 2 is running";
    char* message3 = "Thread 3 is running";


    // create 3 threads
    if (pthread_create(&thread1, NULL, print_message, (void*)message1) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    if (pthread_create(&thread2, NULL, print_message, (void*)message2) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    if (pthread_create(&thread3, NULL, print_message, (void*)message3) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    // wait for 3 threads to finish
    if (pthread_join(thread1, NULL) != 0) {
        perror("Failed to join thread 1");
        return 1;
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("Failed to join thread 2");
        return 1;
    }
    if (pthread_join(thread3, NULL) != 0) {
        perror("Failed to join thread 3");
        return 1;
    }
    printf("All threads are complete\n");
    return 0;
}
