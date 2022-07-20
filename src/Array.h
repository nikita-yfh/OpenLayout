#pragma once
#include <stdlib.h>
#include <string.h>

template<typename T>
class Array {
public:
	Array() {
		count = 0;
		items = nullptr;
	}
	Array(const Array &other) {
		items = nullptr;
		Init(other.count);
		for(int i = 0; i < count; i++)
			new (&items[i]) T(other.items[i]);
	}
	Array(const T *newItems, uint32_t newCount) {
		items = nullptr;
		Set(newItems, newCount);
	}

	~Array() {
		Clear();
	}
	void Init(uint32_t size) {
		Clear();
		count = size;
		if(count) {
			items = (T*) malloc(count * sizeof(T));
			for(int i = 0; i < count; i++)
				new (&items[i]) T();
		}
	}
	void Resize(uint32_t size) {
		if(size < count) {
			for(int i = size; i < count; i++)
				items[i].~T();
			count = size;
			Resize();
		} else if(size > count) {
			uint32_t oldCount = count;
			count = size;
			Resize();
			for(int i = oldCount; i < count; i++)
				new (&items[i]) T();
		}
	}
			
	void Clear() {
		if(items)
			free(items);
		count = 0;
	}
	void Add(const T &value) {
		count++;
		Resize();
		items[count - 1] = value;
	}
	void RemoveIndex(uint32_t index) {
		count--;
		for(int i = index; i < count; i++)
			items[i] = items[i + 1];
		Resize();
	}
	void RemoveLast() {
		count--;
		Resize();
	}
	bool Has(const T &value) const {
		for(int i = 0; i < count; i++)
			if(items[i] == value)
				return true;
		return false;
	}
	void Set(const T *newItems, int newCount) {
		Clear();
		if(newCount) {
			items = (T*) malloc(newCount * sizeof(T));
			memcpy(items, newItems, newCount * sizeof(T));
		} else
			items = nullptr;
		count = newCount;
	}
	bool Empty() const {
		return count == 0;
	}
	uint32_t Size() const {
		return count;
	}
	T &operator[](uint32_t index) {
		return items[index];
	}
	const T &operator[](uint32_t index) const {
		return items[index];
	}
	T &First() {
		return items[0];
	}
	const T &First() const {
		return items[0];
	}
	T &Last() {
		return items[count - 1];
	}
	const T &Last() const {
		return items[count - 1];
	}
protected:
	T *Find(T value) {
		for(int i = 0; i < count; i++)
			if(items[i] == value)
				return &items[i];
		return nullptr;
	}

	void Resize() {
		items = (T*) realloc(items, count * sizeof(T));
	}

	uint32_t count;
	T *items;
private:
	void operator=(const Array&) = delete;
};

template<typename T>
class UniqueArray : public Array<T> {
public:
	using Array<T>::count;
	using Array<T>::items;
	using Array<T>::Find;
	using Array<T>::Has;
	using Array<T>::Resize;

	UniqueArray() {}
	UniqueArray(const UniqueArray &other)
		: Array<T>(other) {}

	void Add(const T &value) {
		if(Has(value))
			return;
		count++;
		Resize();
		items[count - 1] = value;
	}
	void Remove(const T &value) {
		T *i = Find(value);
		if(!i) return;
		count--;
		*i = items[count]; // Move last item to deleted
		Resize();
	}
};

template<typename T>
class UniqueSortArray : public UniqueArray<T> {
protected:
	using Array<T>::count;
	using Array<T>::items;
	using Array<T>::Find;
	using Array<T>::Has;
	using Array<T>::Resize;
public:
	UniqueSortArray() {}
	UniqueSortArray(const UniqueSortArray &other)
		: UniqueArray<T>(other) {}

	void Add(const T &value) {
		if(Has(value))
			return;
		count++;
		Resize();
		for(int i = count - 1; i >= 0; i--) {
			if(items[i - 1] < value || i == 0) {
				items[i] = value;
		   		break;
			}		
			items[i] = items[i - 1];
		}
	}
	void Remove(const T &value) {
		T *i = Find(value);
		if(!i) return;

		const T *last = &items[--count];

		while(i < last) {
			*i = *(i + 1);
			i++;
		}

		Resize();
	}
};



