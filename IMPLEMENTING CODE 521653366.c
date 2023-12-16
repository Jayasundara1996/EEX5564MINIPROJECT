#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_QUEUE_SIZE 5
#define TIME_SLICE 2

typedef struct {
    int process_id;
    int priority;
    int burst_time;
} Process;

typedef struct {
    Process* queue[MAX_QUEUE_SIZE];
    int front, rear;
} Queue;

void initializeQueue(Queue* q) {
    q->front = q->rear = -1;
}

int isQueueEmpty(Queue* q) {
    return q->front == -1;
}

void enqueue(Queue* q, Process* process) {
    if (q->rear == MAX_QUEUE_SIZE - 1) {
        printf("Queue is full!\n");
        return;
    }

    if (q->front == -1) {
        q->front = 0;
    }

    q->rear++;
    q->queue[q->rear] = process;
}

Process* dequeue(Queue* q) {
    if (isQueueEmpty(q)) {
        printf("Queue is empty!\n");
        return NULL;
    }

    Process* process = q->queue[q->front];

    if (q->front == q->rear) {
        initializeQueue(q);
    } else {
        q->front++;
    }

    return process;
}

void executeProcess(Process* process) {
    printf("Executing Process %d (Priority: %d, Burst Time: %d)\n", process->process_id, process->priority, process->burst_time);
}

int main() {
    Queue queues[MAX_QUEUE_SIZE];
    Process processes[MAX_PROCESSES];

    // Initialize queues
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        initializeQueue(&queues[i]);
    }

    // Initialize processes
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].process_id = i + 1;
        processes[i].priority = i % MAX_QUEUE_SIZE;
        processes[i].burst_time = (i % MAX_QUEUE_SIZE) + 1;
        enqueue(&queues[i % MAX_QUEUE_SIZE], &processes[i]);
    }

    // Execute processes in a round-robin manner with time slices
    int time = 0;
    while (1) {
        for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
            if (!isQueueEmpty(&queues[i])) {
                Process* current_process = dequeue(&queues[i]);
                executeProcess(current_process);
                time += TIME_SLICE;
                current_process->burst_time -= TIME_SLICE;

                // Move the process to the lower priority queue if its burst time is not completed
                if (current_process->burst_time > 0) {
                    enqueue(&queues[i + 1], current_process);
                } else {
                    printf("Process %d completed!\n", current_process->process_id);
                }
            }
        }

        // Check if all processes are completed
        int allCompleted = 1;
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (processes[i].burst_time > 0) {
                allCompleted = 0;
                break;
            }
        }

        if (allCompleted) {
            break;
        }
    }

    return 0;
}
