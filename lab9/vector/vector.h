#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cstddef>
#include <cstring>

#define ALLOC_CHUNK 8

template<typename T> class Vector{
public:
	explicit Vector(std::size_t cap = ALLOC_CHUNK)
		: top(0), capacity(cap) {
		
		data = new T[capacity];
		if(!data)
			capacity = 0;
	}

	Vector(const Vector &vector){
		copyVector(vector);
	}

	Vector& operator=(const Vector &vector){
		if(data)
			delete[] data;
		copyVector(vector);	
		return *this;	
	}

	~Vector(){
		if(data)
			delete[] data;
		top = capacity = 0;
		data = nullptr;
	}

	T& operator[](std::size_t index){
		return data[index];
	}

	const T& operator[](std::size_t index) const{
		return data[index];
	}

	void push_back(const T &elem){
		if(top == capacity){
			T *new_data = new T[capacity + ALLOC_CHUNK];
			if(!new_data)
				return;  //failed to allocate more memory
			capacity += ALLOC_CHUNK;
			memcpy(new_data, data, sizeof(T)*top);
			delete[] data;
			data = new_data;
		}

		data[top++] = elem;
	}

	T pop_back(){
		return top ? data[--top] : T();
	}

	void erase(std::size_t idx){
		while(idx < top-1){
			data[idx] = data[idx+1];
			++idx;
		}
		--top;
	}

	std::size_t size() const {
		return top;
	}

	//------------------------------
	class iterator{
	public:
		iterator(const iterator &it)
			: index(it.index), vector(it.vector) {}
		
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
			return vector.data[index];
		}

		bool operator==(const iterator &r){
			return index == r.index;
		}
		bool operator!=(const iterator &r){
			return !(*this == r);
		}
	
	private:
		size_t index;
		Vector &vector;

		iterator(size_t _idx, Vector &_vector)
			: index(_idx), vector(_vector) {}
		friend class Vector;
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
	std::size_t top, capacity;
	T *data;

	void copyVector(const Vector &vector){
		top = vector.top;
		capacity = vector.capacity;
		data = new T[capacity];
		memcpy(data, vector.data, top*sizeof(T));
	}
};

#endif  //_VECTOR_H_