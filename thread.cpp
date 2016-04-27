#include <pthread.h>
#include <stdio.h>
#include <functional>
#include <unistd.h>  
#include "blocking_queue.h"

typedef std::function<void()> Functor; 

BlockingQueue<Functor> taskQueue;
bool running = true; 

void* thread_func(void *arg)
{
	while(running)
	{
		Functor task = taskQueue.take();
		task();
	}
	pthread_exit(NULL);
}

void add_func(int a, int b)
{
	printf("%d+%d=%d\n", a, b, a+b);
}

int main()
{	
	pthread_t ptids[5];
	for (int i = 0; i < 5; ++i)
	{
		pthread_create(&ptids[i], NULL, thread_func, NULL);
	}

	Functor task1 = ::std::bind(add_func, 1, 2);
	taskQueue.post(task1);
	
	Functor task2 = ::std::bind(add_func, 3, 4);
	taskQueue.post(task2);

	Functor task3 = ::std::bind(add_func, 5, 6);
	taskQueue.post(task3);
	

	for (int i = 0; i < 5; ++i)
	{
		pthread_join(ptids[i], NULL);
	}
	
	return 0;
}