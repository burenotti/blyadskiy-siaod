#include "file.h"


void display_content(const std::string& file_path) {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        return;
    }

    int value;
    while (file >> value) {
        if (!file.eof() && file.fail()) {
            std::cout << "Произошла ошибка при чтении файла" << std::endl;
            return;
        }
        std::cout << value << std::endl;
    }
    file.close();
}

int count(const std::string& file_path) {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        return -1;
    }

    int counter = 0;
    int value;
    while (file >> value) {
        counter++;
        if (!file.eof() && file.fail()) {
            std::cout << "Произошла ошибка при чтении файла" << std::endl;
            return -1;
        }
    }
    file.close();
    return counter;
}

void append_number(const std::string& file_path, int value) {
    std::ofstream file(file_path, std::ios_base::app);

    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        return;
    }
    file << value;
    file.close();
}


bool multiply_by_sum_of_first_and_last(const std::string& file_path) {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        file.close();
        return false;
    }

    std::vector<int> items;

    int value;
    while (file) {
        file >> value;
        items.push_back(value);
    }
    if (items.empty()) {
        file.close();
        std::cout << "Невозможно провести операцию на пустом файле" << std::endl;
        return false;
    }
    file.close();
    std::ofstream ofile(file_path);

    if (!ofile.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        ofile.close();
        return false;
    }

    for (int i = 0; i < items.size(); i++) {
        ofile << items[i] * (items.front() + items.back()) << std::endl;
    }

    ofile.close();
}

int number_at_pos(const std::string& file_path, int pos, bool& ok) {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        ok = false;
        return 0;
    }

    int value;
    for (int i = 1; i <= pos; i++) {
        file >> value;
        if (file.eof()) {
            std::cout << "В файле менее " << pos << " чисел" << std::endl;
            ok = false;
            return -1;
        }
        if (!file.eof() && file.fail()) {
            std::cout << "Произошла ошибка при чтении файла" << std::endl;
            ok = false;
            return -1;
        }
    }
    file.close();
    ok = true;
    return value;
}

