#include "queue.h"

void* h_pop(queue_t *queue);
bool h_push(queue_t *queue, void *data);

queue_t* create_queue(int capacity) {
    // allocate the structure
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    queue->capacity = capacity;
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;

    // allocate the array itself
    queue->array = (void **)malloc(capacity * sizeof(void *));

    return queue;
}

void delete_queue(queue_t *queue) {

    free(queue->array);
    free(queue);
}

//tail - first empty place - size of queue
int get_queue_size(queue_t *queue) {
    return queue->count;
}

void* get_from_queue(queue_t *queue, int idx) {
    if (idx >= queue->count || idx < 0) {
        return NULL;
    }

    return queue->array[(queue->head + idx) % queue->capacity];

}

bool push_to_queue(queue_t *queue, void *data) {
    if (queue->capacity == queue->count) {

        // create new array
        void **new_array = (void **)malloc(queue->capacity * 2 * sizeof(void *));

        int len = queue->count;
        int curr_index = queue->head;
        
        for (int i = 0; i < len; i++) {
            
            void* value = queue->array[curr_index];
            curr_index = (curr_index + 1) % queue->capacity;
            new_array[i] = value;

        }

        free(queue->array);
        queue->array = new_array;

        queue->head = 0;
        queue->tail = queue->count;
        queue->capacity = queue->capacity * 2;
        // queue->count is the same

    } 

    queue->array[queue->tail] = data;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->count++;

    return true;
}

void* pop_from_queue(queue_t *queue) {
    if (queue->count == 0) {
        return NULL;
    }

    void *value = queue->array[queue->head];
    queue->array[queue->head] = NULL;
    queue->head = (queue->head + 1) % queue->capacity;
    queue->count--;

    if (queue->count < queue->capacity / 2 && queue->count != 0) {

        // create new array
        void **new_array = (void **)malloc((queue->capacity / 3 * 2) * sizeof(void *));
        
        int len = queue->count;
        int curr_index = queue->head;
        
        for (int i = 0; i < len; i++) {
            
            void* value = queue->array[curr_index];
            curr_index = (curr_index + 1) % queue->capacity;
            new_array[i] = value;

        }
        
        free(queue->array);
        queue->array = new_array;
        
        queue->head = 0;
        queue->tail = queue->count;
        queue->capacity = queue->capacity / 3 * 2;
        // queue->count is the same


    }

    return value;
}
