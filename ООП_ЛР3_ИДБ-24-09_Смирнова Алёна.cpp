#include <iostream>
#include <stdexcept>
#include <string>

class ProgramError {
protected:
    std::string message;
public:
    ProgramError(const std::string& msg) : message(msg) {}
    virtual void print() const {
        std::cout << "Ошибка: " << message << std::endl;
    }
    virtual ~ProgramError() {}
};

class OutOfMemoryError : public ProgramError {
public:
    OutOfMemoryError() : ProgramError("Недостаточно памяти") {}
};

class IOException : public ProgramError {
public:
    IOException() : ProgramError("Ошибка ввода/вывода") {}
};

template <typename T>
class PointerArray {
private:
    T** data;           
    size_t size;        
    size_t capacity;    

public:
    PointerArray() : size(0), capacity(10) {
        data = new T * [capacity];
    }

    ~PointerArray() {
        for (size_t i = 0; i < size; ++i) {
            delete data[i];
        }
        delete[] data;
    }

    T*& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Индекс вне границ массива!");
        }
        return data[index];
    }

    void add(T* item) {
        if (size >= capacity) {
            capacity *= 2;
            T** newData = new T * [capacity];
            for (size_t i = 0; i < size; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[size++] = item;
    }
    size_t getSize() const { return size; }
};

double safeSubtract(double a, double b) {
    double result = a - b;
    if (result < -1000) {  
        throw std::underflow_error("Результат меньше -1000: " + std::to_string(result));
    }
    return result;
}


int main() {
    setlocale(LC_ALL, "Russian");

    std::cout << "=== ПРОГРАММА ДЛЯ РАБОТЫ С ОШИБКАМИ ===" << std::endl;

    PointerArray<ProgramError> errors;

    errors.add(new OutOfMemoryError());
    errors.add(new IOException());

    std::cout << "\nСписок ошибок:" << std::endl;
    for (size_t i = 0; i < errors.getSize(); ++i) {
        errors[i]->print();
    }

    std::cout << "\nПроверка выхода за границы:" << std::endl;
    try {
        errors[10]->print(); 
    }
    catch (const std::out_of_range& e) {
        std::cout << "Поймали ошибку: " << e.what() << std::endl;
    }

    std::cout << "\nПроверка underflow_error:" << std::endl;
    try {
        double badResult = safeSubtract(5, 2000); 
        std::cout << "Результат: " << badResult << std::endl;
    }
    catch (const std::underflow_error& e) {
        std::cout << "Поймали underflow: " << e.what() << std::endl;
    }

    std::cout << "\nПрограмма завершена!" << std::endl;
    return 0;
}