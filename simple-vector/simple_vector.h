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
    ArrayPtr<Type> arr1;
    size_t capacity_=0;
    size_t size_=0; 
    
    void Func(SimpleVector&& other){        
        size_t size_tmp=size_;
        size_t capacity_tmp=capacity_;       
        size_=other.size_;
        capacity_=other.capacity_;
        other.size_=size_tmp;
        other.capacity_=capacity_tmp;
        ArrayPtr<Type> arr3(capacity_);
        for (size_t i=0; i<size_; i++){
            arr3[i]=std::move(other.arr1[i]);           
        }
        arr1.swap(arr3);
    } 
    
public:
    
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size){
        size_=size;
        capacity_=size;
        if(size){
            ArrayPtr<Type> arr3(size);
            arr1.swap(arr3);
            for(size_t i=0;i<size_;i++){                
                arr1[i]=std::move(Type {});
                }            
            }        
    }  
    
    SimpleVector(SimpleVector&& other){
        Func(std::move(other));
    }     
    
    //копирующий конструктор   
   SimpleVector(const SimpleVector& other) {
        size_=other.size_;
        capacity_=other.capacity_;
        ArrayPtr<Type> arr3(capacity_);         
        for (size_t i=0; i<size_; i++){
            arr3[i]=other.arr1[i];           
        }   
        arr1.swap(arr3);
    }
    
    SimpleVector(ReserveProxyObj obj){ 
        capacity_=obj.capacity_to_reserve;
        ArrayPtr<Type> arr3(capacity_);
        arr1.swap(arr3);
    }
   
   
    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value){
    size_=size;
    capacity_=size; 
    
    if(size){
            ArrayPtr<Type> arr3(size);
            arr1.swap(arr3);
            for (size_t i=0;i<size;i++){
                arr1[i]=value;
                }
            }          
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        size_=static_cast<size_t>(std::distance(init.begin(),init.end()));
        capacity_=size_;
        ArrayPtr<Type> arr3(size_);
        arr1.swap(arr3);
        for (size_t i=0;i<size_;i++){
            arr1[i]=*(init.begin()+i);               
        }
    }
    
   
    SimpleVector& operator=(SimpleVector&& other) {
        Func(std::move(other));
        return *this;
    };
    
    //оператор присваивания
    SimpleVector& operator=(const SimpleVector& rhs) {
    if(this!=&rhs){
        size_=rhs.size_;
        capacity_=rhs.capacity_;
        ArrayPtr<Type> arr3(capacity_);
        for (size_t i=0;i<size_;i++){
            arr3[i]=rhs[i];               
            }        
        arr1.swap(arr3);
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
            return arr1[index];        
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {    
        return arr1[index];     
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_)throw std::out_of_range("Wrong index");
        else{
            return arr1[index];
        }        
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_)throw std::out_of_range("Wrong index");
        else{
           return arr1[index];
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
            for(size_t i=size_;i<new_size;i++){
                
                arr1[i]=std::move(Type {});
            }
            size_=new_size;
        }
        else if(new_size>size_&&new_size>=capacity_){
            ArrayPtr<Type> arr2(new_size);            
            std::move( arr1.Get(), arr1.Get()+size_, arr2.Get() );
            for(auto iter=arr2.Get()+size_;iter!=arr2.Get()+new_size;iter+=1){                
                *iter=std::move(Type {});
            }
            size_=new_size;
            capacity_=new_size;
            arr1.swap(arr2);
        }      
    }
    
    void Reserve(size_t new_capacity){
        if (new_capacity>capacity_){
           ArrayPtr<Type> arr2(new_capacity);
           std::move( arr1.Get(), arr1.Get()+size_, arr2.Get() );
           capacity_=new_capacity;
           arr1.swap(arr2);
        }
    }
     
    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
    if (size_>0){
        --size_;
        }
    } 
    

	//Не совсем понятно, оставлять эти реализации методов PushBack,Insert 
   /* // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (capacity_==0){
            capacity_=1;
            size_=1;
            ArrayPtr<Type> arr3(capacity_);
            arr3[0]=std::move(item);
            arr1.swap(arr3);
            }
        else if (size_<capacity_){
            ++size_;
           arr1[size_-1]=item;   
        }  
        else{        
            capacity_*=2;
            ArrayPtr<Type> arr3(capacity_);
            for (size_t i=0;i<size_;i++){
                arr3[i]=arr1[i];               
            }
            ++size_;
            arr3[size_-1]=item; 
            arr1.swap(arr3);
        }
    }
    
   
    Iterator Insert(ConstIterator pos, const Type& value) {
        if (capacity_==0){
            capacity_=1;
            size_=1;
            ArrayPtr<Type> arr3(capacity_);
            arr3[0]=value;
            arr1.swap(arr3);
            return &arr1[0];
        }
        else if (size_<capacity_){            
            auto iter=const_cast<Type*>(pos);                 
            std::copy_backward(iter,&arr1[size_],&arr1[size_+1]);
            *iter=value;
            ++size_;            
            return iter;
        }  
        else{  
            auto iter=const_cast<Type*>(pos);
            int position=std::distance( begin(), iter);
            capacity_*=2;            
            ArrayPtr<Type> arr3(capacity_);
            for (size_t i=0;i<size_;i++){
                arr3[i]=arr1[i];  
                }
            arr1.swap(arr3);
            std::copy_backward(&arr1[position],&arr1[size_],&arr1[size_]+1);  
            arr1[position]=value; 
            ++size_;
            return &arr1[position];
        }
    }
    */


     void PushBack( Type&& item) {
        if (capacity_==0){
            capacity_=1;
            size_=1;
            ArrayPtr<Type> arr3(capacity_);
            arr3[0]=std::move(item);
            arr1.swap(arr3);
            }
        else if (size_<capacity_){
            ++size_;
           arr1[size_-1]=std::move(item);   
        }  
        else{        
            capacity_*=2;
            ArrayPtr<Type> arr3(capacity_);
            for (size_t i=0;i<size_;i++){
                arr3[i]=std::move(arr1[i]); 
            }
            ++size_;
            arr3[size_-1]=std::move(item);
            arr1.swap(arr3);
        }
    }
    
    Iterator Insert(ConstIterator pos, Type&& value) {
        if (capacity_==0){
            capacity_=1;
            size_=1;
            ArrayPtr<Type> arr3(capacity_);
            arr3[0]=std::move(value);
            arr1.swap(arr3);
            return &arr1[0];
        }
        else if (size_<capacity_){            
            auto iter=const_cast<Type*>(pos);                       
            std::move_backward(iter,&arr1[size_],&arr1[size_+1]);
            *iter=std::move(value);
            ++size_;            
            return iter;
        }  
        else{  
            auto iter=const_cast<Type*>(pos);
            int position=std::distance( begin(), iter);
            capacity_*=2;            
            ArrayPtr<Type> arr3(capacity_);
            for (size_t i=0;i<size_;i++){
                arr3[i]=std::move(arr1[i]);  
                }
            arr1.swap(arr3);
            std::move_backward(&arr1[position],&arr1[size_],&arr1[size_]+1);  
            arr1[position]=std::move(value); 
            ++size_;
            return &arr1[position];
        }
    }
    
    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        auto iter=const_cast<Type*>(pos);
        std::move(iter+1,&arr1[size_],iter);
        --size_;
        return const_cast<Type*>(pos);
    }
    
    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        size_t size_tmp=other.size_;
        size_t capacity_tmp=other.capacity_;
        other.size_=size_;
        other.capacity_=capacity_;
        size_=size_tmp;
        capacity_=capacity_tmp;
        arr1.swap(other.arr1);
    }
    
    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return arr1.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return (arr1.Get()+size_);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return arr1.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return (arr1.Get()+size_);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        if (size_==0) return nullptr;
        return (arr1.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        if (size_==0) return nullptr;
        return (arr1.Get()+size_);
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
