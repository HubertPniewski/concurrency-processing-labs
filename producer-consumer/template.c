#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include "pqueue.h"
#include <sys/file.h>
#include <fcntl.h>

char *filename = "queue.dat";
int itemId = 0;
int itemsInStock = 0;

typedef struct item item;
struct item {
	int id;
};

int countItemsInStock(pqueue *q) {
  	int items = 0;
    while (q != NULL) {
      	q = q->next;
      	items++;
    }
    return items;
}

item* produce() {
	item *it = (item *)malloc(sizeof(item));
	it->id = itemId++;
    sleep(rand() % 2 + 1);
	return it;
}

void consume(item *it) {
	free(it);
}

void producer(pid_t childPid) {
    int local_id = 0;
    while (1) {
        sleep(2);

        item *it = malloc(sizeof(item));
        it->id = local_id++;

        int fd = open(filename, O_RDWR | O_CREAT, 0666);
        flock(fd, LOCK_EX);

        pqueue* queue = NULL;
        qunserialize(&queue, sizeof(item), filename);

        if (countItemsInStock(queue) >= 5) {
      		printf("Production stopped, not enoguh place in the warehouse.\n");
      		free(it);
            flock(fd, LOCK_UN);
            close(fd);
            continue;
        }

        qinsert(&queue, it, 0);

        qserialize(queue, sizeof(item), filename);

        flock(fd, LOCK_UN);
        close(fd);

        printf("PRODUCER: Produced a product with ID: %d\n", it->id);
    }
}

void consumer() {
    while (1) {
        sleep(4);
        int fd = open(filename, O_RDWR);
        if (fd < 0) continue;
        flock(fd, LOCK_EX);

        pqueue* queue = NULL;
        qunserialize(&queue, sizeof(item), filename);

        for (int i = 0; i < rand() % 4 + 1; i++) {
          	pqueue *node = qpop(&queue);
        	if (node != NULL) {
            	item* it = (item*)node->data;
            	printf("CONSUMER: Bought the product with ID: %d\n", it->id);

            	if(it) free(it);
            	free(node);
        	} else {
            	printf("CONSUMER: Lack of a product!\n");
            }
        }

        qserialize(queue, sizeof(item), filename);

        flock(fd, LOCK_UN);
        close(fd);
    }
}

int main(int argc, char **argv) {
  	srand(getpid());
    remove(filename);
  	pqueue *qu = NULL;
    qserialize(qu, sizeof(item), filename);

	pid_t pid = fork();
    if (pid == 0) {
      	consumer();
    } else {
      	producer(pid);
    }

	return 0;
}

