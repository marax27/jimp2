#ifndef _SINGLY_LINKED_LIST_H
#define _SINGLY_LINKED_LIST_H

#include <cstdint>
#include <iostream>

template<typename T> class SinglyLinkedList{
public:
	struct Node;

	SinglyLinkedList() : head(nullptr), tail(nullptr) {}
	SinglyLinkedList(const SinglyLinkedList &list) : head(nullptr), tail(nullptr){
		copyList(list);
	}

	SinglyLinkedList& operator=(const SinglyLinkedList &list){
		clear();
		copyList(list);
		return *this;
	}

	~SinglyLinkedList(){
		clear();
	}

	void clear();
	void push_back(const T &elem);
	T pop_back();
	
	Node& front() const { return *head; }
	Node& next(const Node &node) const { return *(node.next); }

	uint32_t size() const;

	bool operator==(const SinglyLinkedList &list) const;
	bool operator!=(const SinglyLinkedList &list) const{
		return !(*this == list);
	}

	struct Node{
		Node(const T &v) : value(v), next(nullptr) {}
		~Node(){
			value = T();
			next = nullptr;
		}

		T value;
		Node *next;
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
		SinglyLinkedList &list;

		iterator(Node *_node, SinglyLinkedList &_list)
			: current(_node), list(_list) {}
		friend class SinglyLinkedList;
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

	void copyList(const SinglyLinkedList &list);
};

template<typename T>
void SinglyLinkedList<T>::clear(){
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
void SinglyLinkedList<T>::push_back(const T &elem){
	Node *n = new Node(elem);
	if(head){
		tail->next = n;
		tail = n;
	}
	else{
		head = n;
		tail = n;
	}
}

template<typename T>
T SinglyLinkedList<T>::pop_back(){
	T result = T();
	if(head){
		result = tail->value;
		if(head == tail){
			delete tail;
			head = tail = nullptr;
		}
		else{
			Node *current = head;
			while(current->next != tail)
				current = current->next;
			delete tail;
			current->next = nullptr;
			tail = current;
		}
	}
	return result;
}

template<typename T>
uint32_t SinglyLinkedList<T>::size() const{
	Node *p = head;
	uint32_t result = 0;
	while(p){
		p = p->next;
		++result;
	}
	return result;
}

template<typename T>
bool SinglyLinkedList<T>::operator==(const SinglyLinkedList<T> &list) const{
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
void SinglyLinkedList<T>::copyList(const SinglyLinkedList<T> &list){
	const auto *current = &list.front();
	while(current){
		push_back(current->value);
		current = current->next;
	}
}

#endif  //_SINGLY_LINKED_LIST_H
