/*
 * =====================================================================================
 *
 *       Filename:  ring_buf.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/12/2016 10:33:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 20

typedef int ElemType;

typedef struct _CircleQueue
{
    ElemType data[QUEUE_SIZE];
    int front;
    int rear;
    int count;
}CircleQueue;

int init_queue(CircleQueue *queue)
{
    queue->front = queue->rear = 0;
    queue->count = 0;

    return 0;
}

int en_queue(CircleQueue *queue, ElemType e)
{
    if (queue->count == QUEUE_SIZE)
        return -1;

    queue->data[queue->rear] = e;
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->count++;
}


ElemType de_queue(CircleQueue *queue)
{
    if (queue->count == 0)
        return -1;

    ElemType e = queue->data[queue->front];
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    queue->count--;

    return e;
}
