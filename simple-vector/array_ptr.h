// вставьте сюда ваш код для класса ArrayPtr
// внесиnте в него изменения, 
// которые позволят реализовать move-семантику

#include <cassert>
#include <cstdlib>
#include <utility>
template <typename Type>
class ArrayPtr {
private:
    Type* raw_ptr_ = nullptr;
public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(size_t size): raw_ptr_((size)?new Type[size]:nullptr){}

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept:raw_ptr_(raw_ptr) {
    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;    
    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;
    
    ArrayPtr(ArrayPtr&& arr ) {
        std::swap(raw_ptr_,arr.raw_ptr_);
    };
    ArrayPtr& operator=(ArrayPtr&& arr){
        std::swap(raw_ptr_,arr.raw_ptr_);
        return *this;};
   
    ~ArrayPtr() {
       delete[] raw_ptr_;
       raw_ptr_=nullptr;
    }


    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        return std::exchange(raw_ptr_,nullptr);
    }
    
    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
       return *(raw_ptr_+index);
    }
    
    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
        return *(raw_ptr_+index);
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const { 
        if (raw_ptr_!=nullptr)return true;
        else return false;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        return raw_ptr_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {        
        raw_ptr_=std::exchange(other.raw_ptr_,raw_ptr_);
    }

};