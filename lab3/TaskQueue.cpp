
#include "TaskQueue.h"
#include <queue>

TaskQueue::
TaskQueue()
{
    // TODO: Your code here.
    //Initialize mutex
    smutex_init(&mtx);
    
    //Initialize condition variable
    scond_init(&cond);
}

TaskQueue::
~TaskQueue()
{
    // TODO: Your code here.
    //Initialize mutex
    smutex_destroy(&mtx);
    
    //Initialize condition variable
    scond_destroy(&cond);
}

/*
 * ------------------------------------------------------------------
 * size --
 *
 *      Return the current size of the queue.
 *
 * Results:
 *      The size of the queue.
 *
 * ------------------------------------------------------------------
 */
int TaskQueue::
size()
{
    // TODO: Your code here.
    smutex_lock(&mtx);
    int size = taskQueue.size();
    smutex_unlock(&mtx);
    return size;
}

/*
 * ------------------------------------------------------------------
 * empty --
 *
 *      Return whether or not the queue is empty.
 *
 * Results:
 *      true if the queue is empty and false otherwise.
 *
 * ------------------------------------------------------------------
 */
bool TaskQueue::
empty()
{
    // TODO: Your code here.
    smutex_lock(&mtx);
    bool empty = taskQueue.empty();
    smutex_unlock(&mtx);
    return empty;
}

/*
 * ------------------------------------------------------------------
 * enqueue --
 *
 *      Insert the task at the back of the queue.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void TaskQueue::
enqueue(Task task)
{
    // TODO: Your code here.
    smutex_lock(&mtx);
    taskQueue.push(task);
    scond_signal(&cond, &mtx);
    smutex_unlock(&mtx);
}

/*
 * ------------------------------------------------------------------
 * dequeue --
 *
 *      Remove the Task at the front of the queue and return it.
 *      If the queue is empty, block until a Task is inserted.
 *
 * Results:
 *      The Task at the front of the queue.
 *
 * ------------------------------------------------------------------
 */
Task TaskQueue::
dequeue()
{
    // TODO: Your code here.
    smutex_lock(&mtx);
    //Wait until the queue is not empty
    while(taskQueue.empty()){
        scond_wait(&cond, &mtx);
    }
    Task task = taskQueue.front();
    taskQueue.pop();
    smutex_unlock(&mtx);
    return task;
}

