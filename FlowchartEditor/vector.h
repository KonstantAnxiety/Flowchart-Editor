#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <fstream>

template <class T>
class Vector
{
private:
    T *data;
    size_t size;
    size_t capacity;
    void grow();
public:
    Vector();
    Vector(size_t n);
    Vector(const Vector<T> &object);
    ~Vector();
    void push_back(const T &object);
    void pop_back();
    void clear() { size = 0; };
    const T& at(size_t pos) const;
    void set(size_t pos, const T &object);
    void del(size_t pos);
    void insert(size_t pos, const T &object);
    bool empty() const;
    size_t getSize() const { return size; };
    size_t getCapacity() const {return capacity; };
    void badIndexMessage(bool ret) const;

    const T &operator[](size_t i) const;
    friend std::ostream& operator<<(std::ostream& out, const Vector &object)
    {
        for (size_t i = 0; i < object.size; ++i)
            out << object.data[i];
        return out;
    }
};

template<class T>
void Vector<T>::grow()
{
    capacity *= 2;
    T *tmpData = new T[capacity];
    for (size_t i = 0; i < size - 1; ++i) {
        tmpData[i] = data[i];
    }
    delete[] data;
    data = tmpData;
}

template<class T>
Vector<T>::Vector()
{
    data = new T[1];
    size = 0;
    capacity = 1;
}

template<class T>
Vector<T>::Vector(size_t n)
{
    if (n == 0) {
        data = new T[1];
        capacity = 1;
    } else {
        data = new T[n];
        capacity = n;
    }
    size = n;
}

template<class T>
Vector<T>::Vector(const Vector<T> &object)
{
    if (data)
        delete[] data;
    size = object.getSize();
    capacity = object.getCapacity();
    this->data = new T[capacity];
    for (size_t i = 0; i < size; ++i) {
        this->set(i, object.at(i));
    }
}

template<class T>
Vector<T>::~Vector()
{
    if (data)
        delete[] data;
    data = NULL;
}

template<class T>
void Vector<T>::push_back(const T &object)
{
    size++;
    if (capacity < size)
        grow();
    data[size - 1] = T(object);
}

template<class T>
void Vector<T>::pop_back()
{
    if (size > 0) {
        size--;
    } else {
        std::cout << "Vector is empty!\n";
    }
}

template<class T>
const T &Vector<T>::at(size_t pos) const
{
    if (size == 0) {
        this->badIndexMessage(1);
        throw 1; // Is there another way?
    } else if (pos >= size) {
        this->badIndexMessage(1);
        return data[0];
    }
    return data[pos];
}

template<class T>
const T &Vector<T>::operator[](size_t pos) const
{
    return at(pos);
}

template<class T>
void Vector<T>::set(size_t pos, const T &object)
{
    T tmp(object);
    if (pos >= size) {
        this->badIndexMessage(0);
        return;
    }
    data[pos] = tmp;
}

template<class T>
void Vector<T>::del(size_t pos)
{
    if (pos >= size) {
        this->badIndexMessage(0);
        return;
    }
    for (size_t i = pos; i < size - 1; ++i)
        this->set(i, this->at(i+1));
    size--;
}

template<class T>
void Vector<T>::insert(size_t pos, const T &object)
{
    if (pos >= size) {
        this->badIndexMessage(0);
        return;
    }
    size++;
    if (size > capacity)
        grow();
    for (size_t i = size-1; i > pos; --i)
        data[i] = data[i-1];
    data[pos] = object;
}

template<class T>
bool Vector<T>::empty() const
{
    if (size == 0)
        return true;
    return false;
}

template<class T>
void Vector<T>::badIndexMessage(bool ret) const
{
    std::cout << "Bad index!";
    if (ret)
        std::cout << " Aborting. Return value may be unexpected.";
    else
        std::cout << " Skipping.";
    std::cout << "\n";
}

#endif // VECTOR_H
