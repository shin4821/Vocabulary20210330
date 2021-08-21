//Array.h
#ifndef _ARRAY_H
#define _ARRAY_H
typedef signed long int Long;
template<typename T>

class Array {
public:
	Array(Long capacity = 256); //����Ʈ ������, �Ű����� ������.
	~Array();//�Ҹ���.
	Array(const Array& source); //���������
	Long Store(Long index, T object);
	Long Insert(Long index, T object);
	Long AppendForFront(T object);
	Long AppendForRear(T object);
	Long Delete(Long index);
	Long DeleteForFront();
	Long DeleteForRear();
	void Clear();
	Long Modify(Long index, T object);
	Long LinearSearchUnique(void* key, int(*compare)(void*, void*));
	void LinearSearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*));
	Long BinarySearchUnique(void* key, int(*compare)(void*, void*));
	void BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*));
	void SelectionSort(int(*compare)(void*, void*));
	void BubbleSort(int(*compare)(void*, void*));
	void InsertSort(int(*compare)(void*, void*));
	void Merge(const Array& one, const Array& other, int(*compare)(void*, void*));

	T& GetAt(Long index);
	T& operator[](Long index);
	T* operator+(Long index);
	Array& operator=(const Array& source);

	Long GetLength() const;
	Long GetCapacity() const;

private:
	T(*front);
	Long capacity;
	Long length;
};

template<typename T>
Array<T>::Array(Long capacity) {//����Ʈ ������, �Ű����� ������.
	this->front = new T[capacity];
	this->capacity = capacity;
	this->length = 0;
}

template<typename T>
Array<T>::~Array() {//�Ҹ���
	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}
}

template<typename T>
Array<T>::Array(const Array& source) { //���������
	Long i = 0;

	this->front = new T[source.capacity];
	while (i < source.length) {
		this->front[i] = source.front[i];
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

template<typename T>
Long Array<T>::Store(Long index, T object) {
	this->front[index] = object;
	this->length++;
	
	return index;
}

template<typename T>
Long Array<T>::Insert(Long index, T object) {
	T(*temps);
	Long i = 0;
	Long j = 0;

	//1. index, object�� �Է¹޴´�.
	//2. temps�� capacity+1��ŭ �迭�� �Ҵ��Ѵ�.
	temps = new T[this->capacity + 1];
	
	//3. array.front�� ���� index��°�� �����ϰ� temps�� �Ű����´�.
	while (i < index) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	j++;
	while (i < this->length) {
		temps[j] = this->front[i];
		j++;
		i++;
	}

	//4. array.front�迭�� �Ҵ������Ѵ�.
	if (this->front != 0) {
		delete[] this->front;
	}
	this->front = temps;

	//5. capacity�� 1�� ������Ų��.
	this->capacity++;

	//6. index��°�� object�� �����.
	this->front[index] = object;

	//7. length�� 1�� ������Ų��.
	this->length++;

	//8. index�� ����Ѵ�,
	return index;

	//9. ������.
}

template<typename T>
Long Array<T>::AppendForFront(T object) {
	Long index = 0;
	T(*temps);
	Long i = 0;
	Long j = 1;

	//1. object�� �Է¹޴´�.
	//2. temps�� capacity+1��ŭ �迭�� �Ҵ��Ѵ�.
	temps = new T[this->capacity + 1];

	//3. array.front�� temps�� �ι�°���� �Ű����´�.
	while (i < this->length) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	//4. array.front�迭�� �Ҵ������Ѵ�.
	if (this->front != 0) {
		delete[] this->front;
	}
	this->front = temps;

	//5. capacity�� 1�� ������Ų��.
	this->capacity++;

	//6. array�� index��°�� object�� ���´�.
	this->front[index] = object;

	//7. length�� 1�� ������Ų��.
	this->length++;

	//8. index�� ����Ѵ�.
	return index;

	//9. ������.
}

template<typename T>
Long Array<T>::AppendForRear(T object) {
	T(*temps);
	Long index;
	Long i = 0;

	//1. object�� �Է¹޴´�.
	//2. temps�� capacity+1��ŭ �迭�� �Ҵ��Ѵ�.
	temps = new T[this->capacity + 1];

	//3. array.front�� temps�� ù��°���� �Ű����´�.
	while (i < this->length) {
		temps[i] = this->front[i];
		i++;
	}
	//4. array.front�迭�� �Ҵ������Ѵ�.
	if (this->front != 0) {
		delete[]this->front;
	}
	this->front = temps;

	//5. capacity�� 1�� ������Ų��.
	this->capacity++;
	index = this->capacity - 1;

	//6. array�� index��°�� object�� ���´�.
	this->front[index] = object;

	//7. length�� 1�� ������Ų��.
	this->length++;

	//8. index�� ����Ѵ�.
	return  index;

	//9. ������.
}

template<typename T>
Long Array<T>::Delete(Long index) {
	T(*temps) = 0;
	Long i = 0;
	Long j = 0;

	if (this->capacity > 1) {
		temps = new T[this->capacity - 1];
	}
	while (i < index) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	i = index + 1;
	while (i < this->length) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}
	if (this->capacity > 1) {
		this->front = temps;
	}
	this->capacity--;
	this->length--;
	index = -1;

	return index;
}

template<typename T>
Long Array<T>::DeleteForFront() {
	Long index;
	T(*temps) = 0;
	Long i = 1;
	Long j = 0;

	//1. capacity�� 1���� Ŭ ���,
	if (this->capacity > 1) {
		//1.1 temps�� capacity-1��ŭ �迭�� �Ҵ��Ѵ�.
		temps = new T[this->capacity - 1];
	}
	//2. array.front�� �ι�°���� temps�� �Ű����´�.
	while (i < this->length) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	//3. array.front�� �Ҵ������Ѵ�.
	if (this->front != 0) {
		delete[] this->front;
		this->front = 0;
	}
	if (this->capacity > 1) {//capacity�� 1���� �� Ŭ��� temps�� �մ´�.
		this->front = temps;
	}
	//4. capacity�� 1�� ����.
	this->capacity--;

	//5. length�� 1�� ����.
	this->length--;

	//6. index�� 0�� �Ҵ��Ѵ�.
	index = -1;

	//7. index�� ����Ѵ�.
	return index;

	//8. ������.
}

template<typename T>
Long Array<T>::DeleteForRear() {
	T(*temps) = 0;
	Long index;
	Long i = 0;

	//1. capacity�� 1���� Ŭ ���,
	if (this->capacity > 1) {
		//1.1 temps�� capacity-1��ŭ �迭�� �Ҵ��Ѵ�.
		temps = new T[this->capacity - 1];
	}
	//2. length-1��ŭ array.front�� temps�� �Ű����´�.
	while (i < this->capacity - 1) {
		temps[i] = this->front[i];
		i++;
	}
	//3. array�迭�� �Ҵ������Ѵ�.
	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}
	if (this->capacity > 1) {
		this->front = temps;
	}
	//4. capacity�� 1�� ���ҽ�Ų��.
	this->capacity--;

	//5. length�� 1�� ���ҽ�Ų��.
	this->length--;

	//6. index�� 0�� ���´�.
	index = -1;

	//7. index�� ����Ѵ�.
	return index;

	//8. ������.
}

template<typename T>
void Array<T>::Clear() {
	//1. array�� �Ҵ������Ѵ�.
	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}
	//2. capacity�� 0���� ����ġ�Ѵ�.
	this->capacity = 0;
	
	//3. length�� 0���� ����ġ�Ѵ�.
	this->length = 0;

	//4. ������.
}

template<typename T>
Long Array<T>::Modify(Long index, T object) {
	//1. index, object�� �Է¹޴´�.
	//2. array.front�� index��°�� object�� ���´�.
	this->front[index] = object;

	//3. index�� ����Ѵ�.
	return index;
	//4. ������.	
}

template<typename T>
Long Array<T>::LinearSearchUnique(void* key, int(*compare)(void*, void*)) {
	Long index = -1;
	Long i = 0;

	//1. key���� �Է¹޴´�.
	//2. index�� 0�̰�, length���� �۰ų� ���� ���� �ݺ��Ѵ�.
	while (i < this->length && compare(this->front + i, key) != 0) {
		i++;
	}
	if (i < this->length) {
		index = i;
	}
	return index;
}

template<typename T>
void Array<T>::LinearSearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;
	*count = 0;

	//1. key���� �Է¹޴´�.
	//2. indexes�� length��ŭ �迭�� �Ҵ��Ѵ�.
	*indexes = new Long[this->length];
	//3. length��ŭ �ݺ��Ѵ�.
	while (i < this->length) {
		if (compare(this->front + i, key) == 0) {
			//3.1 key�� array���� ������, ��ġ�� indexes�� ���´�.
			(*indexes)[j] = i;
			j++;
			//3.2 key�� array���� ������, ������ ����.
			(*count)++;
		}
		i++;
	}
	//4. indexes�� count�� ����Ѵ�.
	//5. ������.
}

template<typename T>
Long Array<T>::BinarySearchUnique(void* key, int(*compare)(void*, void*)) {
	Long index = -1;
	Long high = this->length - 1;
	Long low = 0;
	Long middle;

	//1. key���� �Է¹޴´�.
	//2. low�� high���� ���ų� �۰�, array���� key�� �ƴѵ��� �ݺ��Ѵ�.
	middle = (low + high) / 2;
	while (low <= high && compare(this->front + middle, key) != 0) {
		if (compare(this->front + middle, key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	if (low <= high) {
		index = middle;
	}
	return index;
}





template<typename T>
void Array<T>::BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*)) {
	Long low = 0;
	Long high = this->length - 1;
	Long middle;
	Long i;
	Long j = 0;
	*count = 0;

	//1. key���� �Է¹޴´�.
	//2. indexes�� length��ŭ �迭�� �Ҵ��Ѵ�,
	*indexes = new Long[this->length];
	middle = (low + high) / 2;

	//3. low�� high���� ���ų� ����, array���� key�� �ƴѵ��� �ݺ��Ѵ�.
	while (low <= high && compare(this->front + middle, key) != 0) {
		if (compare(this->front + middle, key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	//4. middle�������� ���� ÷�� ��ġ���� ���� ���� �ִ��� Ȯ���Ѵ�.
	i = middle - 1;
	while (i >= 0 && compare(this->front + i, key) == 0) {
		i--;
	}
	i++;
	//5. front(i)�� ���� key�� ���� length���� ���ų� �������� �ݺ��Ѵ�.
	while (i < this->length && compare(this->front + i, key) == 0) {
		//5.1 key�� array���� ������, ��ġ�� indexes�� ���´�.
		(*indexes)[j] = i;
		j++;
		//5.2. key�� array���� ������, ������ ����.
		(*count)++;
		i++;
	}
	//6. indexes, count�� ����Ѵ�.
	//7. ������.
}



template<typename T>
void Array<T>::SelectionSort(int(*compare)(void*, void*)) {
	T temp;
	Long index;
	Long i = 0;
	Long j;

	//1. length-1��ŭ �ݺ��Ѵ�.
	while (i < this->length - 1) {
		index = i;
		j = i + 1;
		//1.1 ���� ���� ���� ã�Ƴ���.
		while (j < this->length) {
			if (compare(this->front + j, this->front + index) < 0) {
				index = j;
			}
			j++;
		}
		temp = this->front[index];
		this->front[index] = this->front[i];
		this->front[i] = temp;
		i++;
	}
}

template<typename T>
void Array<T>::BubbleSort(int(*compare)(void*, void*)) {
	bool ret = false;
	T temp;
	Long i = 0;
	Long j;

	//1. length���� ���� ret�� true�� �ƴѵ��� �ݺ��Ѵ�.
	while (i < this->length - 1 && ret != true) {
		j = 0;
		ret = true;
		while (j < this->length - i - 1) {
			if (compare(this->front + j, this->front + (j + 1)) > 0) {
				temp = this->front[j];
				this->front[j] = this->front[j + 1];
				this->front[j + 1] = temp;
				ret = false;
			}
			j++;
		}
		i++;
	}
}

template<typename T>
void Array<T>::InsertSort(int(*compare)(void*, void*)) {
	T temp;
	Long i = 1;
	Long j;

	//1. length-1��ŭ �ݺ��Ѵ�.
	while (i < this->length) {
		temp = this->front[i];
		j = i - 1;
		while (j >= 0 && compare(&temp, this->front + j) < 0) {
			this->front[j + 1] = this->front[j];
			j--;
		}
		this->front[j + 1] = temp;
		i++;
	}
}

template<typename T>
void Array<T>::Merge(const Array& one, const Array& other, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;
	Long k = 0;

	//1. one, other�� �Է¹޴´�.
	//2. array.front�� �Ҵ������Ѵ�.
	if (this->front != 0) {
		delete[]this->front;
	}
	//3. one.length�� other.length�� �ո�ŭ array�� �迭�� �Ҵ��Ѵ�.
	this->front = new T[one.length + other.length];
	this->capacity = one.length + other.length;
	this->length = 0;

	//4. one.length���� ���ų� �۰�, other.length���� ���ų� �������� �ݺ��Ѵ�.
	while (i < one.length && j < other.length) {
		//4.1 one�� other�� ������ ��
		if (compare(one.front + i, other.front + j) < 0) {
			//4.1.1 �� �������� front�� ����.
			this->front[k] = one.front[i];
			this->length++;
			k++;
			i++;
		}
		else {
			this->front[k] = other.front[j];
			this->length++;
			k++;
			j++;
		}
	}
	//5. one.length���� ���ų� ������ front�� ����.
	while (i < one.length) {
		this->front[k] = one.front[i];
		this->length++;
		k++;
		i++;
	}
	//6. other.length���� ���ų� ������ front�� ����.
	while (j < other.length) {
		this->front[k] = other.front[j];
		this->length++;
		k++;
		j++;
	}
}

template <typename T>
T& Array<T>::operator[](Long index) {
	return this->front[index];
}

template<typename T>
T* Array<T>::operator+(Long index) {
	return this->front + index;
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& source) {

	Long i = 0;

	if (this->front != 0) {
		delete[]this->front;
	}
	this->front = new T[source.capacity];
	while (i < source.length) {
		this->front[i] = source.front[i];
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}



template<typename T>
inline Long Array<T>::GetLength() const {
	return this->length;
}

template<typename T>
inline Long Array<T>::GetCapacity()const{
	return this->capacity;
}



template<typename T>
T& Array<T>::GetAt(Long index) {
	//1. index�� �Է¹޴´�.
	//2. array.front�� index��° ���� ����Ѵ�.
	return this->front[index];
}




#endif//_ARRAY_H







