#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef enum {
    HIGH,
    NORMAL
} Priority;

typedef struct {
    int order_id;
    int distance_km;
    Priority priority;
} DeliveryOrder;

int main() {
    DeliveryOrder orders[10000];
    int threshold = 20;
    int thread_high_count[4] = {0, 0, 0, 0};

    #pragma omp parallel num_threads(4)
    {
        #pragma omp single
        {
            for (int i = 0; i < 10000; i++) {
                orders[i].order_id = i + 1;
                orders[i].distance_km = rand() % 30;
            }
        }


        #pragma omp for
        for (int i = 0; i < 10000; i++) {
            if (orders[i].distance_km < threshold)
                orders[i].priority = HIGH;
            else
                orders[i].priority = NORMAL;
        }

        #pragma omp barrier

        int id = omp_get_thread_num();
        thread_high_count[id] = 0;

        #pragma omp for
        for (int i = 0; i < 10000; i++) {
            if (orders[i].priority == HIGH) {
                thread_high_count[id]++;
            }
        }

        #pragma omp barrier

        #pragma omp single
        {
            int total = 0;

            for (int i = 0; i < 4; i++) {
                printf("Thread %d HIGH count: %d\n", i, thread_high_count[i]);
                total += thread_high_count[i];
            }

            printf("Total HIGH priority orders: %d\n", total);
        }
    }

    return 0;
}
