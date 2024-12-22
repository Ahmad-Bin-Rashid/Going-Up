#pragma once

#include <vector>
#include <stdexcept>

template <typename T>
class Queue {
private:
    std::vector<T> data;
    int frontIndex;

public:
    Queue() : frontIndex(0) {}

    void enqueue(T value) {
        data.push_back(value);
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data[frontIndex++];
    }

    T front() const {
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

    int getSize() const {
        return data.size();
    }
};