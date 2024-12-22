#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& value, std::shared_ptr<Node> prevNode = nullptr) 
            : data(value), prev(prevNode) {}
    };

    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    size_t size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList() {
        clear();
    }

    size_t getSize() const {
        return size;
    }

    bool isEmpty() const {
        return size == 0;
    }

    void pushFront(const T& value) {
        auto newNode = std::make_shared<Node>(value, nullptr);
        if (head) {
            head->prev = newNode;
        } else {
            tail = newNode;
        }
        newNode->next = head;
        head = newNode;
        ++size;
    }

    void pushBack(const T& value) {
        auto newNode = std::make_shared<Node>(value, tail);
        if (tail) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
        ++size;
    }

    void popFront() {
        if (isEmpty()) {
            throw std::underflow_error("List is empty");
        }
        head = head->next;
        if (head) {
            head->prev.reset();
        } else {
            tail.reset();
        }
        --size;
    }

    void popBack() {
        if (isEmpty()) {
            throw std::underflow_error("List is empty");
        }
        tail = tail->prev.lock();
        if (tail) {
            tail->next.reset();
        } else {
            head.reset();
        }
        --size;
    }

    T& front() {
        if (isEmpty()) {
            throw std::underflow_error("List is empty");
        }
        return head->data;
    }

    T& back() {
        if (isEmpty()) {
            throw std::underflow_error("List is empty");
        }
        return tail->data;
    }

    void clear() {
        while (!isEmpty()) {
            popFront();
        }
    }

    std::vector<std::string> getValues() const {
        std::vector<std::string> values;
        for (auto current = head; current != nullptr; current = current->next) {
            for (const auto& pair : current->data) {
                values.push_back(pair.second);
            }
        }
        return values;
    }

    std::shared_ptr<Node> getNodeFromString(const std::string& value) {
        for (auto current = head; current != nullptr; current = current->next) {
            for (const auto& pair : current->data) {
                if (pair.second == value) {
                    return current;
                }
            }
        }
        return nullptr;
    }

    std::shared_ptr<Node> getNodeFromPosition(int position) const {
        if (position < 1 || position > size) {
            throw std::out_of_range("Position out of range");
        }
        auto current = head;
        for (int i = 1; i < position; ++i) {
            current = current->next;
        }
        return current;
    }

    void setNodeAt(int position, const T& value) {
        auto current = getNodeFromPosition(position);
        current->data = value;
    }

    int getPositionFromString(const std::string& value) {
        int position = 1;
        for (auto current = head; current != nullptr; current = current->next, ++position) {
            for (const auto& pair : current->data) {
                if (pair.second == value) {
                    return position;
                }
            }
        }
        return -1;
    }

    T getDataFromPosition(int position) {
        auto node = getNodeFromPosition(position);
        return node->data;
    }
};

#endif