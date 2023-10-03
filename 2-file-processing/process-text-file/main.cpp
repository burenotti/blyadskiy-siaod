#include <iostream>
#include "lib/file.h"

using namespace std;


int main() {

    string file_path;
    cout << "Введите путь к текстовому файлу: ";
    getline(cin, file_path, '\n');

    cout << "1. Вывести файл \n"
         << "2. Добавить число в файл \n"
         << "3. Считать файл по порядковому номеру \n"
         << "4. Посчитать количество числе в файле \n"
         << "5. Умножить каждое число в файле на сумму первого и последнего элементов \n"
         << "Выберите операцию [1-5]: ";

    int opt;
    cin >> opt;
    if (opt == 1) {
        display_content(file_path);
    } else if (opt == 2) {
        int number;
        cout << "Введите число, которое хотите добавить: ";
        cin >> number;
        append_number(file_path, number);
    } else if (opt == 3) {
        int pos;
        cout << "Введите порядковый номер числа: ";
        cin >> pos;
        bool ok;
        int number = number_at_pos(file_path, pos, ok);
        if (!ok) {
            return 0;
        }
        cout << "Порядковый номер" << pos << " имеет число " << number << endl;
    } else if (opt == 4) {
        int c = count(file_path);
        if (c == -1) {
            return 0;
        }
        cout << "В файле " << c << " чисел" << endl;
    } else if (opt == 5) {
        if (!multiply_by_sum_of_first_and_last(file_path)) {
            cout << "При обработке файла произошла ошибка" << endl;
        }
    } else if (opt == 6) {
        return 0;
    }
}
