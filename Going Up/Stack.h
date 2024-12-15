#pragma once

#include <memory>
#include <stdexcept>

template <typename T>
class Stack {
    struct Node {
        T data;
        std::unique_ptr<Node> next;
    };

private:
    std::unique_ptr<Node> head;

public:
    Stack() = default;

    Stack(const Stack& other) {
        if (other.head) {
            head = std::make_unique<Node>();
            head->data = other.head->data;

            Node* current = head.get();
            Node* otherCurrent = other.head->next.get();

            while (otherCurrent) {
                current->next = std::make_unique<Node>();
                current = current->next.get();
                current->data = otherCurrent->data;
                otherCurrent = otherCurrent->next.get();
            }
        }
    }

    void push(const T& data) {
        auto newNode = std::make_unique<Node>();
        newNode->data = data;
        newNode->next = std::move(head);
        head = std::move(newNode);
    }

    T pop() {
        if (head == nullptr) {
            throw std::out_of_range("Stack<>::pop(): empty stack");
        }

        auto oldHead = std::move(head);
        head = std::move(oldHead->next);
        return oldHead->data;
    }

    const T& top() const {
        if (head == nullptr) {
            throw std::out_of_range("Stack<>::top(): empty stack");
        }

        return head->data;
    }

    bool empty() const {
        return head == nullptr;
    }

    size_t size() const {
        size_t count = 0;
        auto current = head.get();
        while (current != nullptr) {
            ++count;
            current = current->next.get();
        }

        return count;
    }

    void clear() {
        while (head != nullptr) {
            head = std::move(head->next);
        }
    }

    ~Stack() {
        clear();
    }
};