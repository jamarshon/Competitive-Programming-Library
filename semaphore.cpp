using namespace std;
#include <iostream>
#include <thread>
#include <mutex> 
#include <condition_variable>
#include <atomic>

struct semaphore {
    string name;
    int value;
    mutex mtx;
    condition_variable cv;
};

void wait(semaphore *s) { // P()
    unique_lock<mutex> lck(s -> mtx); 
    s -> value--; 
    if(s -> value < 0) {
        (s -> cv).wait(lck);
    }
}

void signal(semaphore *s) { // V()
    unique_lock<mutex> lck(s -> mtx); 
    s -> value++; 
    if(s -> value <= 0) {
        (s -> cv).notify_one();
    }
}

int N = 10;

int bufferSize = N/2;
int* buffer = new int[bufferSize]();

int numberOfItems = 10; // number of items per thread
int numberOfThreads = 2; // pairs of consumers/producers to make

int in = 0; // index to produce to
int out = 0; // index to consume from

semaphore* mtx = new semaphore{"mtx", 1};
semaphore* full = new semaphore{"full", 0};
semaphore* empty = new semaphore{"empty", bufferSize};

atomic<bool> finishedThreadCreation(false);
void waitUntilCompleted(){ while(!finishedThreadCreation.load()) this_thread::yield(); }

void printBuffer(bool isProducer) {
    if(in == out && isProducer) { // everything is full
        cout << "[";
        for(int i = 0; i < bufferSize; i++) cout << buffer[i] << " ";
        cout << "]" << endl;
    } else {
        for(int i = 0; i < bufferSize; i++) {
            if(i == out) cout << "[ ";
            if(i == in ) cout << "] ";
            cout << buffer[i] << " ";
        }
        cout << endl;
    }
}

void produce(int threadID) {
    waitUntilCompleted();

    for(int i = 1; i <= numberOfItems; i++) {
        int item = (threadID+1)*i*10;
        wait(empty);
        wait(mtx);
        
        // add item to buffer
        buffer[in] = item;
        in = (in + 1) % bufferSize;
        printf("Thread #%d produced %d\n", threadID, item);
        // printBuffer(true);

        signal(mtx);
        signal(full);
    }
}

void consume(int threadID) {
    waitUntilCompleted();

    for(int i = 1; i <= numberOfItems; i++) {
        wait(full);
        wait(mtx);

        // remove an item from buffer
        int temp = buffer[out];
        buffer[out] = -1; // consumed item
        out = (out + 1) % bufferSize;
        printf("Thread #%d consumed %d\n", threadID, temp);
        // printBuffer(false);

        signal(mtx);
        signal(empty);
    }
}

int main() {
    thread producers[N];
    thread consumers[N];

    fill(buffer, buffer + bufferSize, -1);

    for(int i = 0; i < numberOfThreads; i++) {
        producers[i] = thread(produce, i);
        consumers[i] = thread(consume, numberOfThreads + i);
    }

    finishedThreadCreation.store(true);

    for(int i = 0; i < numberOfThreads; i++) {
        producers[i].join();
        consumers[i].join();
    }

    return 0;
}