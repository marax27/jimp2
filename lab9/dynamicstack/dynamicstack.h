#ifndef _DYNAMICSTACK_H_
#define _DYNAMICSTACK_H_

#define ALLOC_CHUNK 8

#include <cstdint>
#include <cstring>

template<typename T> class DynamicStack{
public:
	DynamicStack() : capacity(ALLOC_CHUNK), top(0){
		data = new T[ALLOC_CHUNK];
		if(!data)
			capacity = 0;
	}

	DynamicStack(const DynamicStack &stack){
		copyStack(stack);
	}

	DynamicStack& operator=(const DynamicStack &stack){
		if(data)
			delete[] data;
		copyStack(stack);
		return *this;
	}

	~DynamicStack(){
		delete[] data;
		capacity = top = 0;
	}

	void push(const T &elem);

	T pop(){
		return data[--top];
	}

	uint32_t getCapacity() const { return capacity; }
	uint32_t getSize() const { return top; }

	//------------------------------
	class iterator{
	public:
		iterator(const iterator &it)
			: index(it.index), stack(it.stack) {}
		
		iterator& operator++(){
			++index;
			return *this;
		}

		iterator operator++(int){
			iterator r = *this;
			++index;
			return r;
		}

		T& operator*(){
			return stack.data[index];
		}

		bool operator==(const iterator &r){
			return index == r.index;
		}
		bool operator!=(const iterator &r){
			return !(*this == r);
		}
	
	private:
		size_t index;
		DynamicStack &stack;

		iterator(size_t _idx, DynamicStack &_stack)
			: index(_idx), stack(_stack) {}
		friend class DynamicStack;
	};
	//------------------------------

	iterator begin(){
		iterator it(0, *this);
		return it;
	}

	iterator end(){
		iterator it(top, *this);
		return it;
	}

private:
	uint32_t capacity, top;
	T *data;

	void copyStack(const DynamicStack &stack);
};

template<typename T>
void DynamicStack<T>::push(const T &elem){
	if(top+1 == capacity){
		T *new_data = new T[capacity + ALLOC_CHUNK];
		if(!new_data)
			return;
		memcpy(new_data, data, top*sizeof(T));
		delete[] data;
		data = new_data;
		capacity += ALLOC_CHUNK;
	}

	data[top++] = elem;
}

template<typename T>
void DynamicStack<T>::copyStack(const DynamicStack<T> &stack){
	capacity = stack.capacity;
	top = stack.top;
	data = new T[capacity];
	memcpy(data, stack.data, top*sizeof(T));
}

#endif  //_DYNAMICSTACK_H_