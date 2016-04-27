#include "blocking_queue.h"
#include <pthread.h>

template<typename T>  
BlockingQueue<T>::BlockingQueue () : queue_ ()  
{  
	mutex_ = PTHREAD_MUTEX_INITIALIZER;
	condvar_ = PTHREAD_COND_INITIALIZER;
}  

template<typename T>  
void BlockingQueue<T>::post (const T& task)  
{
    pthread_mutex_lock(&mutex_);
    queue_.push_back (task);  
    pthread_mutex_unlock(&mutex_);
    pthread_cond_signal(&condvar_);   
}  


template<typename T>  
T BlockingQueue<T>::take ()  
{  
    pthread_mutex_lock(&mutex_); 

    while(queue_.size() == 0)
    {
    	pthread_cond_wait(&condvar_, &mutex_);
    }

    assert (!queue_.empty ());  
    T front (queue_.front ());  
    queue_.pop_front ();  

    return front;  
}  

template<typename T>  
size_t BlockingQueue<T>::size() const  
{  
    pthread_mutex_lock(&mutex_);
    return queue_.size();  
    pthread_mutex_unlock(&mutex_);
}  

