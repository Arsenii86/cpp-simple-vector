// вставьте сюда ваш код для класса SimpleVector
// внесите необходимые изменения для поддержки move-семантики

#pragma once
#include <iostream>
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include "array_ptr.h"
#include <utility>
#include <iterator>


class ReserveProxyObj
{        
    public:
        size_t capacity_to_reserve=0;
        size_t size_to_reserve=0;  
        ReserveProxyObj(size_t number):capacity_to_reserve(number){};
    
};


template <typename Type>
class SimpleVector {
private:
    ArrayPtr<Type> arr;   
    size_t size_=0; 
    size_t capacity_=0;
    void RVecMove(SimpleVector&& other){
        size_=std::exchange(other.size_,size_);        
        capacity_=std::exchange(other.capacity_,capacity_);
        arr = std::move(other.arr);
    } 
    
public:
    
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size):SimpleVector(size, Type {}){}  
    
    SimpleVector(SimpleVector&& other){
        RVecMove(std::move(other));
    }     
    
    //копирующий конструктор   
   SimpleVector(const SimpleVector& other):size_(other.size_), capacity_(other.capacity_){
        ArrayPtr<Type> arr_tmp(capacity_);
        std::copy(other.arr.Get(),other.arr.Get()+size_,arr_tmp.Get());        
        arr.swap(arr_tmp);
    }
    
    SimpleVector(ReserveProxyObj obj){ 
        capacity_=obj.capacity_to_reserve;
        ArrayPtr<Type> arr_tmp(capacity_);
        arr.swap(arr_tmp);
    }
   
   
    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value):size_(size),capacity_(size){   
    if(size){
            ArrayPtr<Type> arr_tmp(size);
            arr.swap(arr_tmp);
            std::fill(arr.Get(),arr.Get()+size_,value);            
            }          
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init):size_(static_cast<size_t>(std::distance(init.begin(),init.end()))),capacity_(size_) {
        ArrayPtr<Type> arr_tmp(size_);
        arr.swap(arr_tmp);
        std::copy(init.begin(),init.end(),arr.Get()); 
    }
    
   
    SimpleVector& operator=(SimpleVector&& other) {
        RVecMove(std::move(other));
        return *this;
    };
    
    //оператор присваивания
    SimpleVector& operator=(const SimpleVector& rhs) {
    if(this!=&rhs){
        SimpleVector(rhs).swap(*this);
        }
        return *this;
    }
    

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        // Напишите тело самостоятельно
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return (size_)?false:true;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {        
            return arr[index];        
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {    
        return arr[index];     
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_)throw std::out_of_range("Wrong index");
        else{
            return arr[index];
        }        
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_)throw std::out_of_range("Wrong index");
        else{
           return arr[index];
        }
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_=0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {        
        if (new_size<=size_&&new_size<=capacity_){
            size_=new_size;
        }
        else if(new_size>size_&&new_size<=capacity_){
            std::generate(arr.Get()+size_,arr.Get()+new_size,[](){return Type{};}); 
            size_=new_size;
        }
        else if(new_size>size_&&new_size>=capacity_){
            ArrayPtr<Type> arr_tmp(new_size);            
            std::move( arr.Get(), arr.Get()+size_, arr_tmp.Get() );
            std::generate(arr_tmp.Get()+size_,arr_tmp.Get()+new_size,[](){return Type{};});            
            size_=new_size;
            capacity_=new_size;
            arr.swap(arr_tmp);
        }      
    }
    
    void Reserve(size_t new_capacity){
        if (new_capacity>capacity_){
           ArrayPtr<Type> arr_tmp(new_capacity);
           std::move( arr.Get(), arr.Get()+size_, arr_tmp.Get() );
           capacity_=new_capacity;
           arr.swap(arr_tmp);
        }
    }
     
    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
    if (size_>0){
        --size_;
        }
    } 
    

	//Не совсем понятно, оставлять эти реализации методов PushBack,Insert 
    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (size_<capacity_){
            ++size_;
           arr[size_-1]=item;   
        }  
        else{  
            if (capacity_==0)capacity_=1;            
            else capacity_*=2;
            ArrayPtr<Type> arr_tmp(capacity_);
            std::copy(arr.Get(),arr.Get()+size_,arr_tmp.Get());   
            ++size_;
            arr_tmp[size_-1]=item; 
            arr.swap(arr_tmp);
        }
    }
    
   
    Iterator Insert(ConstIterator pos, const Type& value) {
         if (size_<capacity_){            
            auto iter=const_cast<Type*>(pos);                 
            std::copy_backward(iter,&arr[size_],&arr[size_+1]);
            //std::copy_backward(iter,arr.Get()+size_,arr.Get()+size_+1);
            *iter=value;
            ++size_;            
            return iter;
        }  
        else{  
            auto iter=const_cast<Type*>(pos);
            int position=std::distance( begin(), iter);
            if (capacity_==0)capacity_=1;            
            else capacity_*=2;            
            ArrayPtr<Type> arr_tmp(capacity_);
            std::copy(arr.Get(),arr.Get()+size_,arr_tmp.Get());   
            arr.swap(arr_tmp);
            std::copy_backward(&arr[position],&arr[size_],&arr[size_]+1);  
            arr[position]=value; 
            ++size_;
            return &arr[position];
        }
    }
    


     void PushBack( Type&& item) {
        if (size_<capacity_){
            ++size_;
           arr[size_-1]=std::move(item);   
        }  
        else{        
            if (capacity_==0)capacity_=1;            
            else capacity_*=2;   
            ArrayPtr<Type> arr_tmp(capacity_);
            std::move(arr.Get(),arr.Get()+size_,arr_tmp.Get());
            ++size_;
            arr_tmp[size_-1]=std::move(item);
            arr.swap(arr_tmp);
        }
    }
    
    Iterator Insert(ConstIterator pos, Type&& value) {
       if (size_<capacity_){            
            auto iter=const_cast<Type*>(pos);                       
            std::move_backward(iter,&arr[size_],&arr[size_+1]);
            *iter=std::move(value);
            ++size_;            
            return iter;
        }  
        else{  
            auto iter=const_cast<Type*>(pos);
            int position=std::distance( begin(), iter);
            if (capacity_==0)capacity_=1;            
            else capacity_*=2;             
            ArrayPtr<Type> arr_tmp(capacity_);
            std::move(arr.Get(),arr.Get()+size_,arr_tmp.Get());
            arr.swap(arr_tmp);
            std::move_backward(&arr[position],&arr[size_],&arr[size_]+1);  
            arr[position]=std::move(value); 
            ++size_;
            return &arr[position];
        }
    }
    
    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        auto iter=const_cast<Type*>(pos);
        std::move(iter+1,&arr[size_],iter);
        --size_;
        return const_cast<Type*>(pos);
    }
    
    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        std::swap(size_,other.size_);
        std::swap(capacity_,other.capacity_);        
        arr.swap(other.arr);
    }
    
    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return arr.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return (arr.Get()+size_);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return arr.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return (arr.Get()+size_);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        if (size_==0) return nullptr;
        return (arr.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        if (size_==0) return nullptr;
        return (arr.Get()+size_);
    }
  
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {   
    return std::equal( lhs.begin(), lhs.end(),rhs.begin())&&lhs.GetSize()==rhs.GetSize();
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !std::equal( lhs.begin(), lhs.end(),rhs.begin())||lhs.GetSize()!=rhs.GetSize();
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::lexicographical_compare(lhs.begin(), lhs.end(),rhs.begin(),rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !std::lexicographical_compare( rhs.begin(), rhs.end(),lhs.begin(), lhs.end());
}


template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::lexicographical_compare( rhs.begin(), rhs.end(),lhs.begin(), lhs.end());
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !std::lexicographical_compare(lhs.begin(), lhs.end(),rhs.begin(),rhs.end());
} 


ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
};   