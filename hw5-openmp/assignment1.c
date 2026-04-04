#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef enum {
    FAST,
    MEDIUM,
    SLOW
} LogCategory;

typedef struct {
    int request_id;
    int user_id;
    int response_time_ms;
    LogCategory category;
} LogEntry;

int main() {
    LogEntry logs[20000];

    #pragma omp parallel num_threads(4)
    {
        #pragma omp single
        {
            for (int i = 0; i < 20000; i++) {
                logs[i].request_id = i + 1;
                logs[i].user_id = rand() % 1000;
                logs[i].response_time_ms = rand() % 500;
            }
        }

        #pragma omp barrier

        #pragma omp for
        for (int i = 0; i < 20000; i++) {
            if (logs[i].response_time_ms < 100)
                logs[i].category = FAST;
            else if (logs[i].response_time_ms <= 300)
                logs[i].category = MEDIUM;
            else
                logs[i].category = SLOW;
        }

        #pragma omp barrier

        #pragma omp single
        {
            int fast = 0, medium = 0, slow = 0;

            for (int i = 0; i < 20000; i++) {
                if (logs[i].category == FAST) {
                    fast++;
                }
                else if (logs[i].category == MEDIUM) {
                    medium++;
                }
                else {
                    slow++;
                }
            }

            printf("FAST: %d\n", fast);
            printf("MEDIUM: %d\n", medium);
            printf("SLOW: %d\n", slow);
        }
    }

    return 0;
}
