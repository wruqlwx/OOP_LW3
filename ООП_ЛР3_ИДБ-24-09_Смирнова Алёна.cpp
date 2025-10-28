#include <iostream>
#include <stdexcept>
#include <string>

// 1. КЛАССЫ ОШИБОК
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

// 2. ШАБЛОННЫЙ КЛАСС ДЛЯ МАССИВА
template <typename T>
class PointerArray {
private:
    T** data;           // Массив указателей
    size_t size;        // Текущий размер
    size_t capacity;    // Максимальный размер

public:
    // Конструктор
    PointerArray() : size(0), capacity(10) {
        data = new T * [capacity];
    }

    // Деструктор
    ~PointerArray() {
        for (size_t i = 0; i < size; ++i) {
            delete data[i];
        }
        delete[] data;
    }

    // Оператор [] для доступа к элементам
    T*& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Индекс вне границ массива!");
        }
        return data[index];
    }

    // Добавление элемента
    void add(T* item) {
        if (size >= capacity) {
            // Увеличиваем массив в 2 раза
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

    // Получение размера
    size_t getSize() const { return size; }
};

// 3. ФУНКЦИЯ ДЛЯ UNDERFLOW_ERROR
double safeSubtract(double a, double b) {
    double result = a - b;
    if (result < -1000) {  // Наша нижняя граница
        throw std::underflow_error("Результат меньше -1000: " + std::to_string(result));
    }
    return result;
}

// 4. ГЛАВНАЯ ФУНКЦИЯ
int main() {
    setlocale(LC_ALL, "Russian");

    std::cout << "=== ПРОГРАММА ДЛЯ РАБОТЫ С ОШИБКАМИ ===" << std::endl;

    // Создаем массив для ошибок
    PointerArray<ProgramError> errors;

    // Добавляем ошибки в массив
    errors.add(new OutOfMemoryError());
    errors.add(new IOException());

    // Работа с массивом через оператор []
    std::cout << "\nСписок ошибок:" << std::endl;
    for (size_t i = 0; i < errors.getSize(); ++i) {
        errors[i]->print();
    }

    // Проверка исключения при выходе за границы
    std::cout << "\nПроверка выхода за границы:" << std::endl;
    try {
        errors[10]->print();  // Несуществующий индекс
    }
    catch (const std::out_of_range& e) {
        std::cout << "Поймали ошибку: " << e.what() << std::endl;
    }

    // Проверка underflow_error
    std::cout << "\nПроверка underflow_error:" << std::endl;
    try {
        double badResult = safeSubtract(5, 2000);  // 5 - 2000 = -1995 (< -1000)
        std::cout << "Результат: " << badResult << std::endl;
    }
    catch (const std::underflow_error& e) {
        std::cout << "Поймали underflow: " << e.what() << std::endl;
    }

    std::cout << "\nПрограмма завершена!" << std::endl;
    return 0;
}