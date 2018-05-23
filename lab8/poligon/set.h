#ifndef _SET_H_
#define _SET_H_

#include <cstddef>
#include <cstring>
#include <algorithm>
#include "vector.h"

// #define DEBUG_MODE

#ifdef DEBUG_MODE
#include <iostream>
#endif  //DEBUG_MODE

template<typename T> class Set{
public:
	typedef std::size_t size_t;
	static const size_t npos = -1;

	Set(){}
	Set(const Set &s) : data(s.data) {}
	~Set(){}

	Set& operator=(const Set &s){
		data = s.data;
		return *this;
	}

	// Add 'elem' to set.
	void insert(const T &elem){
		if(find(elem) == npos){
			data.push_back(elem);
			heapSort();
		}
	}

	// Remove 'elem' from set.
	void erase(const T &elem){
		auto idx = find(elem);
		if(idx != npos){
			data[idx] = data[data.size()-1];
			data.pop_back();
			heapSort();
		}
	}

	// Check if 'elem' is in set.
	bool in(const T &elem) const {
		return find(elem) != npos;
	}

	size_t size() const {
		return data.size();
	}

	bool operator==(const Set &right) const {
		if(size() != right.size())
			return false;
		for(size_t i = 0; i != size(); ++i){
			if(data[i] != right.data[i])
				return false;
		}
		return true;
	}

	bool operator!=(const Set &right) const {
		return !(*this == right);
	}

	//------------------------------
	class iterator{
	public:
		iterator(const iterator &it) : index(it.index), set(it.set) {}

		iterator& operator++(){
			++index;
			return *this;
		}

		iterator operator++(int){
			iterator r = *this;
			++index;
			return r;
		}
		
		// iterator cannot modify elements of set.
		const T& operator*(){
			return set.data[index];
		}

		bool operator==(const iterator &r){
			return index == r.index;
		}
		bool operator!=(const iterator &r){
			return !(*this == r);
		}

	private:
		size_t index;
		Set &set;

		iterator(size_t _idx, Set &_set) : index(_idx), set(_set) {}
		friend class Set;
	};
	//------------------------------

	iterator begin(){
		iterator it(0, *this);
		return it;
	}

	iterator end(){
		iterator it(data.size(), *this);
		return it;
	}

	#ifdef DEBUG_MODE
	void print() const {
		std::cout << "{ ";
		for(size_t i=0; i!=data.size(); ++i)
			std::cout << data[i] << ' ';
		std::cout << "}\n";
	}
	#endif  //DEBUG_MODE

private:
	Vector<T> data;

	// Use binary search algorithm to find 'elem' in set.
	size_t find(const T &elem) const;

	// Data sort.
	void heapSort();
	void buildHeap();
	void heapify(size_t end, size_t idx);
};

//************************************************************

template<typename T>
typename Set<T>::size_t Set<T>::find(const T &elem) const {
	if(!data.size())
		return npos;

	int64_t l = 0,
	        r = data.size()-1,
	        m;
	while(l <= r){
		m = (l + r)/2;
		if(data[m] < elem)
			l = m+1;
		else if(data[m] > elem)
			r = m-1;
		else
			return m;
	}
	return npos;
}

template<typename T>
void Set<T>::heapSort(){
	if(!data.size())
		return;
	buildHeap();
	for(size_t i = data.size()-1; i > 0; --i){
		std::swap(data[i], data[0]);
		heapify(i, 0);
	}
}

template<typename T>
void Set<T>::buildHeap(){
	auto ds = data.size();
	for(size_t i = ds/2; i > 0; --i)
		heapify(ds, i-1);
}

template<typename T>
void Set<T>::heapify(Set<T>::size_t end, Set<T>::size_t idx){
		size_t maximum = idx, l, r;
		l = 2*idx + 1;
		r = l + 1;

		if(l < end && data[idx] < data[l])
			maximum = l;
		if(r < end && data[maximum] < data[r])
			maximum = r;
		if(maximum != idx){
			std::swap(data[maximum], data[idx]);
			if(2*maximum + 1 < end)
				heapify(end, maximum);
		}
	}

#endif  //_SET_H_