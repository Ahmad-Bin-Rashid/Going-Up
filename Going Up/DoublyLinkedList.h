template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& value) : data(value), next(nullptr), prev() {}
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
        auto newNode = std::make_shared<Node>(value);
        newNode->next = head;
        if (head) {
            head->prev = newNode;
        } else {
            tail = newNode;
        }
        head = newNode;
        ++size;
    }

    void pushBack(const T& value) {
        auto newNode = std::make_shared<Node>(value);
        newNode->prev = tail;
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

    void printForward() const {
        for (auto current = head; current != nullptr; current = current->next) {
            std::cout << current->data << " ";
        }
        std::cout << std::endl;
    }

    void printBackward() const {
        for (auto current = tail; current != nullptr; current = current->prev.lock()) {
            std::cout << current->data << " ";
        }
        std::cout << std::endl;
    }

    std::shared_ptr<Node> getFirstNode() const {
        return head;
    }

    std::shared_ptr<Node> getNextNode(const std::shared_ptr<Node>& node) const {
        if (node) {
            return node->next;
        }
        return nullptr;
    }

    std::shared_ptr<Node> getPrevNode(const std::shared_ptr<Node>& node) const {
        if (node) {
            return node->prev.lock();
        }
        return nullptr;
    }
};