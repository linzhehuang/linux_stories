#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int canvas = 0; /* 0 - empty,  positive number means filled */
pthread_mutex_t accessable = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t canvas_filled = PTHREAD_COND_INITIALIZER;

void* paint_worker(void *args)
{
	int i = 0;
	for(;i < 1000; i++) {
		pthread_mutex_lock(&accessable);

		canvas += 1;
		printf("canvas = %d\n", canvas);

		pthread_cond_signal(&canvas_filled);
		pthread_mutex_unlock(&accessable);
	}
	return NULL;
}

void* erase_worker(void *args)
{
	for(;;) {
		pthread_mutex_lock(&accessable);
		// waitting canvas filled
		pthread_cond_wait(&canvas_filled, &accessable);

		canvas = 0;
		printf("earse canvas\n");

		pthread_mutex_unlock(&accessable);
	}
	return NULL;
}

int main(void)
{
	pthread_t eid, pid;
	pthread_create(&eid, NULL, erase_worker, NULL);
	pthread_create(&pid, NULL, paint_worker, NULL);

	pthread_join(eid, NULL);
	pthread_join(pid, NULL);
	return 0;
}
