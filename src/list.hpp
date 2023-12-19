// Copyright 2023 code.luetek.com
#ifndef LUE_CPP_LIB_LIST_HPP_  //  Inclusion Gaurd
#define LUE_CPP_LIB__LIST_HPP_
#include <utility>
template <typename Object>
class List {
    class Node {
     protected:
      Node *prev, *next;
      Node(Node *prev, Node *next): prev(prev), next(next) {}
      friend class List;
    };
    class DataNode : public Node {
      Object data;
      DataNode(const Object & data, Node *prev, Node *next): Node(prev, next), data(data) {}
      DataNode(Object && data, Node *prev, Node *next): Node(prev, next), data(std::move(data)) {}
      friend class List;
    };

    class ConstIterator {
      DataNode * node;
        public:
        explicit ConstIterator(DataNode * node): node(node) {}
        ConstIterator & operator++ () {
            node = (DataNode *)node->next;
            return *this;
        }
        const Object & operator * () {
            return node->data;
        }

        bool operator == (const ConstIterator & rhs) const {
            return this->node == rhs.node;
        }
        bool operator != (const ConstIterator & rhs) const {
            return this->node != rhs.node;
        }
        friend class List;
    };

    class Iterator: public ConstIterator {
     public:
        explicit Iterator(DataNode * node): ConstIterator(node) {}
        Object & operator * () {
            return this->node->data;
        }

        Iterator & operator ++() {
            this->node = reinterpret_cast<DataNode*>(this->node->next);
            return *this;
        }
    };
    // Sentinal nodes
    Node head, tail;
public:
    List(): head(Node(nullptr, nullptr)),  tail(Node(nullptr, nullptr)) {
      head.next = &tail;
      tail.prev = &head;
    }

    // insert obj before itr
    Iterator insert(Iterator it, const Object  & obj) {
        DataNode * current =  it.node;
        DataNode * newNode = new DataNode(obj, current->prev, current);
        current->prev->next = newNode;
        current->prev = newNode;
        return Iterator(newNode);
    }

    Iterator insert(Iterator it, Object  && obj) {
        DataNode * current =  it.node;
        DataNode * newNode = new DataNode(std::move(obj), current->prev, current);
        current->prev->next = newNode;
        current->prev = newNode;
        return Iterator(newNode);
    }

    Iterator begin() {
        return Iterator(reinterpret_cast<DataNode *>(this->head.next));
    }
    Iterator end() {
        return Iterator(reinterpret_cast<DataNode *>(&tail));
    }

    ConstIterator begin() const {
        return ConstIterator(reinterpret_cast<DataNode *>(this->head.next));
    }

     ConstIterator end() const {
        auto tailptr = reinterpret_cast<DataNode *>(const_cast<List<Object>::Node *>(&this->tail));
        return ConstIterator(tailptr);
    }
};
#endif
