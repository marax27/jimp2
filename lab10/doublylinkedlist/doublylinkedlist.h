#ifndef _DOUBLY_LINKED_LIST_H
#define _DOUBLY_LINKED_LIST_H

#include <cstdint>
#include <iostream>

#include "bit.h"

#define DEBUG_MODE

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

		bool operator==(const iterator &r) const{
			return current == r.current;
		}
		bool operator!=(const iterator &r) const{
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





//************************************************************





template<> class DoublyLinkedList<bool>{
	class OctoNode;
public:
	class Node;
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

	void clear(){
		OctoNode *p = head;
		OctoNode *next = nullptr;
		while(p){
			next = p->next;
			delete p;
			p = next;
		}
		head = tail = nullptr;
	}

	void push_back(bool elem){
		if(!tail || tail->number_used == 8){
			OctoNode *p = new OctoNode();
			if(tail){
				tail->next = p;
				p->prev = tail;
				tail = p;
			}
			else
				tail = head = p;
		}

		tail->get(tail->number_used++) = elem;
	}

	void push_front(bool elem){
		if(!head || head->number_used == 8){
			OctoNode *p = new OctoNode();
			if(head){
				head->prev = p;
				p->next = head;
				head = p;
			}
			else
				head = tail = p;
		}

		head->data <<= 1;
		head->get(0) = elem;
		++head->number_used;
	}

	bool pop_back(){
		while(!tail->number_used){
			auto *p = tail->prev;
			delete tail;
			tail = p;
			p->next = nullptr;
		}

		bool result = tail->get(--tail->number_used);
		return result;
	}

#ifdef DEBUG_MODE
	void printList(){
		OctoNode *p = head;
		std::cout << "(H:" << size() << ")";
		while(p){
			std::cout << " -> [";
			for(int i=0; i!=p->number_used; ++i)
				std::cout << (int)p->get(i);
			std::cout << "]:" << (int)p->number_used;
			p = p->next;
		}
		std::cout << '\n';
	}
#endif

	Node front() const { return Node(head, 0); }
	Node back() const { return Node(nullptr, 0); }
	Node next(const Node &node) const {
		if(node.index == 7)
			return Node(node.base->next, 0);
		else
			return Node(node.base, node.index+1);
	}
	Node previous(const Node &node) const {
		if(!node.index)
			return Node(node.base->prev, 7);
		else
			return Node(node.base, node.index-1);
	}

	uint32_t size() const {
		OctoNode *p = head;
		uint32_t result = 0;
		while(p){
			result += p->number_used;
			p = p->next;
		}
		return result;
	}

	bool operator==(const DoublyLinkedList &list) const{
		Node l = Node(this->head, 0);
		Node r = Node(list.head, 0);
		while(true){
			if(l.value != r.value)
				return false;

			if(l == this->back() && r == list.back())
				break;
			else if((l == this->back()) ^ (r == list.back()))
				return false;
			l = this->next(l);
			r = list.next(r);
		}
	}

	bool operator!=(const DoublyLinkedList &list) const{
		return !(*this == list);
	}

	//------------------------------
	class Node{
		Node(OctoNode *on, byte_t idx)
			: base(on), index(idx), value(on->get(index)) {}

		OctoNode *base;
		byte_t index;
		
		friend class DoublyLinkedList;
	public:
		BitRef value;

		bool operator==(const Node &r) const{
			return index == r.index && base == r.base;
		}

		bool operator!=(const Node &r) const{
			return !(*this == r);
		}
	};
	//------------------------------
	class iterator{
	public:
		iterator(const iterator &it)
			: current(it.current), list(it.list) {}
		
		iterator& operator++(){
			current = list.next(current);
			return *this;
		}

		iterator operator++(int){
			iterator r = *this;
			current = list.next(current);
			return r;
		}

		BitRef operator*(){
			return current.value;
		}

		bool operator==(const iterator &r) const{
			return current == r.current;
		}
		bool operator!=(const iterator &r) const{
			return !(*this == r);
		}
	
	private:
		Node current;
		DoublyLinkedList &list;

		iterator(Node _node, DoublyLinkedList &_list)
			: current(_node), list(_list) {}
		friend class DoublyLinkedList;
	};
	//------------------------------	

	iterator begin(){
		iterator it(front(), *this);
		return it;
	}

	iterator end(){
		iterator it(back(), *this);
		return it;
	}

private:
	OctoNode *head;
	OctoNode *tail;

	//------------------------------
	struct OctoNode{
		OctoNode() : data(0), number_used(0) {}

		byte_t data;
		byte_t number_used;
		OctoNode *prev, *next;

		BitRef get(byte_t index){
			// if(index >= number_used)
			// 	return BitRef(&(next->data), index%8);
			// else
			return BitRef(&data, index);
		}

		ConstBitRef get(byte_t index) const{
			if(index >= number_used)
				return ConstBitRef(&(next->data), index%8);
			else
				return ConstBitRef(&data, index);
		}
	};
	//------------------------------

	void copyList(const DoublyLinkedList &list){
		clear();
		const OctoNode *n = list.head;
		OctoNode **modifier = &head;
		OctoNode *previous = nullptr;

		while(n){
			OctoNode *c = new OctoNode();
			c->number_used = n->number_used;
			c->data = n->data;
			c->prev = previous;
			*modifier = c;
			previous = *modifier;
			modifier = &(*modifier)->next;
			n = n->next;
		}
	}
};

#endif  //_Doubly_LINKED_LIST_H
