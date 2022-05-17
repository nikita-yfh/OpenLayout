#pragma once
#include <stdlib.h>
#include <string.h>

template<typename T>
class UniqueArray {
public:
	UniqueArray() {
		count = 0;
		items = nullptr;
	}
	~UniqueArray() {
		Clear();
	}
	void Init(uint32_t size) {
		Clear();
		count = size;
		items = (T*) malloc(count * sizeof(T));
	}
	void Clear() {
		if(items)
			free(items);
		count = 0;
	}
	void Add(const T &value) {
		if(Find(value))
			return;
		count++;
		items = (T*) realloc(items, count * sizeof(T));
	}
	void Remove(const T &value) {
		T *i = Find(value);
		if(!i) return;
		count--;
		*i = items[count]; // Move last item to deleted
		items = (T*) realloc(items, count * sizeof(T));
	}
	void RemoveIndex(uint32_t index) {
		count--;
		for(int i = index; i < count; i++)
			items[i] = items[i + 1];
		items = (T*) realloc(items, count * sizeof(T));
	}
	bool Has(const T &value) const {
		for(int i = 0; i < count; i++)
			if(items[i] == value)
				return true;
		return false;
	}
	void Set(const T *newItems, int newCount) {
		Clear();
		items = (T*) malloc(newCount * sizeof(T));
		memcpy(items, newItems, newCount * sizeof(T));
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
protected:
	T *Find(T value) {
		for(int i = 0; i < count; i++)
			if(items[i] == value)
				return &items[i];
		return nullptr;
	}

	uint32_t count;
	T *items;
private:
	UniqueArray(const UniqueArray&) = delete;
	void operator=(const UniqueArray&) = delete;
};

template<typename T>
class UniqueSortArray : public UniqueArray<T> {
	using UniqueArray<T>::count;
	using UniqueArray<T>::items;
	using UniqueArray<T>::Find;

public:
	void Add(const T &value) {
		if(Find(value))
			return;
		count++;
		items = (T*) realloc(items, count * sizeof(T));
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

		items = (T*) realloc(items, count * sizeof(T));
	}
};



