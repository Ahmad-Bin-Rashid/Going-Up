#pragma once

#include <vector>
#include <stdexcept>

class Queue {
private:
    std::vector<int> data;
    int frontIndex;

public:
    Queue() : frontIndex(0) {}

    void enqueue(int value) {
        data.push_back(value);
    }

    int dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data[frontIndex++];
    }

    int front() const {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data[frontIndex];
    }

    bool empty() const {
        return frontIndex >= data.size();
    }

    bool isEmpty() const {
        return frontIndex >= data.size();
    }
};