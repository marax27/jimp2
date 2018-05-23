#ifndef _DOUBLY_LINKED_LIST_H
#define _DOUBLY_LINKED_LIST_H

#include <cstdint>
#include <iostream>

template<typename T> class DoublyLinkedList{
public:
	struct Node;

	DoublyLinkedList() : head(nullptr), tail(nullptr) {}
	DoublyLinkedList(const DoublyLinkedList &list) : head(nullptr), tail(nullptr) {
		copyList(list);
	}

	DoublyLinkedList& operator=(const DoublyLinkedList &list){
		clear();
		copyList(list);
		return *this;
	}

	~DoublyLinkedList(){
		clear();
	}

	void clear();
	void push_back(const T &elem);
	void push_front(const T &elem);
	T pop_back();

	Node& front() const { return *head; }
	Node& back() const { return *tail; }
	Node& next(const Node &node) const { return *(node.next); }
	Node& previous(const Node &node) const { return *(node.prev); }

	uint32_t size() const;

	bool operator==(const DoublyLinkedList &list) const;
	bool operator!=(const DoublyLinkedList &list) const{
		return !(*this == list);
	}

	struct Node{
		Node(const T &v)
			: value(v), next(nullptr), prev(nullptr) {}
		~Node(){
			value = T();
			next = nullptr;
			prev = nullptr;
		}

		T value;
		Node *next, *prev;
	};

	//------------------------------
	class iterator{
	public:
		iterator(const iterator &it)
			: current(it.current), list(it.list) {}
		
		iterator& operator++(){
			current = &list.next(*current);
			return *this;
		}

		iterator operator++(int){
			iterator r = *this;
			current = &list.next(*current);
			return r;
		}

		T& operator*(){
			return current->value;
		}

		bool operator==(const iterator &r){
			return current == r.current;
		}
		bool operator!=(const iterator &r){
			return !(*this == r);
		}
	
	private:
		Node *current;
		DoublyLinkedList &list;

		iterator(Node *_node, DoublyLinkedList &_list)
			: current(_node), list(_list) {}
		friend class DoublyLinkedList;
	};
	//------------------------------

	iterator begin(){
		iterator it(&front(), *this);
		return it;
	}

	iterator end(){
		iterator it(nullptr, *this);
		return it;
	}

private:
	Node *head;
	Node *tail;

	void copyList(const DoublyLinkedList &list);
};

template<typename T>
void DoublyLinkedList<T>::clear(){
	Node *p = head;
	Node *next = nullptr;
	while(p){
		next = p->next;
		delete p;
		p = next;
	}

	head = nullptr;
	tail = nullptr;
}

template<typename T>
void DoublyLinkedList<T>::push_back(const T &elem){
	Node *n = new Node(elem);
	if(head){
		tail->next = n;
		n->prev = tail;
		tail = n;
	}
	else{
		head = n;
		tail = n;
	}
}

template<typename T>
void DoublyLinkedList<T>::push_front(const T &elem){
	Node *n = new Node(elem);
	if(head){
		head->prev = n;
		n->next = head;
		head = n;
	}
	else{
		head = n;
		tail = n;
	}
}

template<typename T>
T DoublyLinkedList<T>::pop_back(){
	T result = T();
	if(head){
		result = tail->value;
		if(head == tail){
			delete tail;
			head = tail = nullptr;
		}
		else{
			Node *last = tail->prev;
			delete tail;
			tail = last;
			last->next = nullptr;
		}
	}
	return result;
}

template<typename T>
uint32_t DoublyLinkedList<T>::size() const{
	Node *p = head;
	uint32_t result = 0;
	while(p){
		p = p->next;
		++result;
	}
	return result;
}

template<typename T>
bool DoublyLinkedList<T>::operator==(const DoublyLinkedList<T> &list) const{
	Node *l = this->head;
	Node *r = list.head;
	while(l && r){
		if(l->value != r->value)  //check for different values
			return false;
		l = l->next;
		r = r->next;
	}
	return l == r;  //check for different sizes
}

//************************************************************

template<typename T>
void DoublyLinkedList<T>::copyList(const DoublyLinkedList<T> &list){
	const auto *current = &list.front();
	while(current){
		push_back(current->value);
		current = current->next;
	}
}

#endif  //_Doubly_LINKED_LIST_H
