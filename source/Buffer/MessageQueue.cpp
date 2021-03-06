#include "MessageQueue.h"

struct Node
{
    int type;
    int len;
    void* data;
};

/**
 * instantiates a new {MessageQeueue} object.
 *
 * @function   MessageQueue::MessageQueue
 *
 * @date       2015-03-18
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  MessageQueue::MessageQueue(int _elementSize) : elementSize(_elementSize)
 *
 * @param      _elementSize the size of each element in the message queue.
 */
MessageQueue::MessageQueue(int capacity, int elementSize)
    : elementSize(elementSize)
{
    this->hasMessage = CreateEvent(NULL,TRUE,FALSE,NULL);
    this->canEnqueue = CreateSemaphore(NULL,capacity,capacity,NULL);
    this->canDequeue = CreateSemaphore(NULL,0,capacity,NULL);
    this->access = CreateMutex(NULL, FALSE, NULL);
}

/**
 * appends the passed data to the message queue. if the queue is full, the
 *   function may block until there is room to store the element into the queue.
 *
 * @function   MessageQueue::enqueue
 *
 * @date       2015-03-18
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void MessageQueue::enqueue(int type, void* src)
 *
 * @param      type number indicating what kind of an element is being put into
 *   the queue.
 * @param      src pointer to the data that is being copied into the message
 *   queue.
 */
void MessageQueue::enqueue(int type, void* src)
{
    enqueue(type,src,elementSize);
}

/**
 * appends the passed data to the message queue. if the queue is full, the
 *   function may block until there is room to store the element into the queue.
 *
 * @function   MessageQueue::enqueue
 *
 * @date       2015-03-18
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void MessageQueue::enqueue(int type, void* src)
 *
 * @param      type number indicating what kind of an element is being put into
 *   the queue.
 * @param      src pointer to the data that is being copied into the message
 *   queue.
 * @param      len number of bytes to copy from {src} ino the buffer
 */
void MessageQueue::enqueue(int type, void* src, int len)
{
    // allocate a node
    Node* n = (Node*) malloc(sizeof(Node));
    void* data = malloc(len);

    // put the data into a node
    memcpy(data,src,len);
    n->type = type;
    n->len  = len;
    n->data = data;

    // obtain synchronization objects
    WaitForSingleObject(canEnqueue,INFINITE);
    WaitForSingleObject(access,INFINITE);

    // put the node into the queue
	messages.insert( messages.begin(), n );

    // now that the element is enqueued, it can be dequeued;
    // set the hasMessage event
    SetEvent(hasMessage);

    // release synchronization objects
    ReleaseMutex(access);
    ReleaseSemaphore(canDequeue,1,NULL);
}

/**
 * removes an element from the {MessageQueue}, and copies the data from the
 *   queue into {dest}.
 *
 * @function   MessageQueue::dequeue
 *
 * @date       2015-03-18
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void MessageQueue::dequeue(int* type, void* dest)
 *
 * @param      type pointer to an integer that will be assigned a number
 *   indicating what kind of an element was taken out from the queue.
 * @param      dest pointer to the location to copy the data from the
 *   {MessageQueue} into.
 */
void MessageQueue::dequeue(int* type, void* dest)
{
    int useless;
    dequeue(type,dest,&useless);
}


int MessageQueue::peekLen()
{
    // obtain synchronization objects
    WaitForSingleObject(access,INFINITE);
    int data = (*messages.begin())->len;
    ReleaseMutex(access);

    return data;

}


/**
 * removes an element from the {MessageQueue}, and copies the data from the
 *   queue into {dest}.
 *
 * @function   MessageQueue::dequeue
 *
 * @date       2015-03-18
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 *
 * @signature  void MessageQueue::dequeue(int* type, void* dest)
 *
 * @param      type pointer to an integer that will be assigned a number
 *   indicating what kind of an element was taken out from the queue.
 * @param      dest pointer to the location to copy the data from the
 *   {MessageQueue} into.
 * @param      len pointer to an integer that will be assigned a number indicating how big the dequeued element is.
 */
void MessageQueue::dequeue(int* type, void* dest, int* len)
{
    // obtain synchronization objects
    WaitForSingleObject(canDequeue,INFINITE);
    WaitForSingleObject(access,INFINITE);
	
    // remove the element from the queue
	Node * n = *--messages.end();
	messages.pop_back();

    // if the queue is empty, reset hasMessage
    if(messages.size() == 0)
    {
        ResetEvent(hasMessage);
    }

    // release synchronization objects
    ReleaseMutex(access);
    ReleaseSemaphore(canEnqueue,1,NULL);

    // get the data from the element
    memcpy(dest,n->data,n->len);
    *type = n->type;
    *len  = n->len;

    // deallocate the element
    free(n->data);
    free(n);
}

int MessageQueue::size()
{
    return messages.size();
}

void MessageQueue::clear()
{
    WaitForSingleObject( access, INFINITE );
    while( WaitForSingleObject( canDequeue, 10 ) != WAIT_TIMEOUT )
	{
		messages.pop_back();
		ReleaseSemaphore( canEnqueue, 1, NULL );
	}
    ReleaseMutex(access);
}
