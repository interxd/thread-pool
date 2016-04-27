#pragma once
#include <pthread.h>
#include <list>

template<typename T>  
class BlockingQueue  
{  
public:  
    BlockingQueue () : queue_ ()  
    {  
  		mutex_ = PTHREAD_MUTEX_INITIALIZER;
  		condvar_ = PTHREAD_COND_INITIALIZER;
    }  
  
    void post (const T& task)  
    {
        pthread_mutex_lock(&mutex_);
        queue_.push_back (task);  
        pthread_mutex_unlock(&mutex_);
        pthread_cond_signal(&condvar_);   
    }  
  
    T take ()  
    {  
        pthread_mutex_lock(&mutex_); 

        while(queue_.size() == 0)
        {
        	pthread_cond_wait(&condvar_, &mutex_);
        }

        //assert (!queue_.empty ());  
        T front (queue_.front ());  
        queue_.pop_front ();  
  		pthread_mutex_unlock(&mutex_);
        return front;  
    }  
  
    size_t size() const  
    {  
        pthread_mutex_lock(&mutex_);
        return queue_.size();  
        pthread_mutex_unlock(&mutex_);
    }  
  
private:  
    BlockingQueue (const BlockingQueue& rhs);  
    BlockingQueue& operator = (const BlockingQueue& rhs);  
  
private:  
    mutable pthread_mutex_t mutex_;   
    pthread_cond_t condvar_;
    std::list<T> queue_;  
};  