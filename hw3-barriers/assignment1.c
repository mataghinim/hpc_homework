#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define P 5
#define R 10

int rolls[P];
int wins[P];

pthread_barrier_t barrier;

void *player(void *arg) {
    int id = *(int *)arg;

    for (int round = 0; round < R; round++) {
        int roll = rand() % 6 + 1;

        rolls[id] = roll;
        printf("Round %d - Player %d rolled %d\n", round + 1, id + 1, roll);
        pthread_barrier_wait(&barrier);

        if (id == 0) {
            int max = rolls[0];
            for (int i = 1; i < P; i++) {
                if (rolls[i] > max) {
                    max = rolls[i];
                }
            }
            printf("Round %d winner(s): ", round + 1);
            for (int i = 0; i < P; i++) {
                if (rolls[i] == max) {
                    wins[i]++;
                    printf("Player %d ", i + 1);
                }
            }
            printf("\n\n");
        }

        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

int main() {
    pthread_t threads[P];
    int ids[P];

    pthread_barrier_init(&barrier, NULL, P);

    for (int i = 0; i < P; i++) {
        wins[i] = 0;
        ids[i] = i;
        pthread_create(&threads[i], NULL, player, &ids[i]);
    }

    for (int i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }

    int maxWins = wins[0];
    for (int i = 1; i < P; i++) {
        if (wins[i] > maxWins) {
            maxWins = wins[i];
        }
    }

    printf("Overall winner(s): ");
    for (int i = 0; i < P; i++) {
        if (wins[i] == maxWins) {
            printf("Player %d ", i + 1);
        }
    }
    printf("with %d wins\n", maxWins);

    pthread_barrier_destroy(&barrier);

    return 0;
}
