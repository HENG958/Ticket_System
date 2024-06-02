#ifndef LIST_HPP_
#define LIST_HPP_

#include <iostream>

namespace trainsys {

template <class TypeName>
class list 
{
   public:
    virtual void clear() = 0;
    virtual int length() const = 0;
    virtual void insert(int i, const TypeName &x) = 0;
    virtual void remove(int i) = 0;
    virtual int search(const TypeName &x) const = 0;
    virtual TypeName visit(int i) const = 0;
    virtual void traverse() const = 0;
    virtual ~list() {};
};

template <class TypeName>
class List: public list<TypeName> 
{
private:
    TypeName *data;
    int cur_length;
    int max_size;
    void doubleSpace();

public:
    List(int initSize = 10);
    ~List() 
    { 
        delete[] data; 
    }
    void clear() 
    { 
        cur_length = 0;
    }
    int length() const 
    { 
        return cur_length; 
    }
    bool empty() const 
    { 
        return cur_length == 0; 
    }
    void insert(int i, const TypeName &x);
    void pushBack(const TypeName &x);
    void popBack();
    void remove(int i);
    int search(const TypeName &x) const;
    TypeName back() const;
    TypeName visit(int i) const 
    { 
        return data[i]; 
    }
    void traverse() const;
};

template <class TypeName>
List<TypeName>::List(int initSize) 
{
    data = new TypeName[initSize];
    max_size = initSize;
    cur_length = 0;
}

template <class TypeName>
int List<TypeName>::search(const TypeName &x) const 
{
    int i;
    for (i = 0; i < cur_length && data[i] != x; i++);
    if (i == cur_length) return -1;
    else return i;
}

template <class TypeName>
void List<TypeName>::traverse() const 
{
    for (int i = 0; i < cur_length; i++) std::cout << data[i] << ' ';
    std::cout << std::endl;
}

template <class TypeName>
void List<TypeName>::insert(int i, const TypeName &x) 
{
    if (cur_length == max_size)  
        doubleSpace();          
    for (int j = cur_length; j > i; j--)
        data[j] = data[j - 1]; 
    data[i] = x;   
    ++cur_length;
}

template <class TypeName>
void List<TypeName>::pushBack(const TypeName &x) 
{
    insert(length(), x);
}

template <class TypeName>
void List<TypeName>::popBack() 
{
    remove(length() - 1);
}

template <class TypeName>
void List<TypeName>::doubleSpace() 
{
    TypeName *temp = data;
    max_size *= 2;
    data = new TypeName[max_size];            
    for (int i = 0; i < cur_length; i++) data[i] = temp[i]; 
    delete[] temp;         
}

template <class TypeName>
void List<TypeName>::remove(int i) 
{
    for (int j = i; j < cur_length - 1; j++)
        data[j] = data[j + 1]; 
    --cur_length;
}

template <class TypeName>
TypeName List<TypeName>::back() const 
{
    return visit(length() - 1);
}

}

#endif 