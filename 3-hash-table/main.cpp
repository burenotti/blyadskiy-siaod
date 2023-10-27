//
// Created by Burenin Artem on 09.10.2023.
//

#include <iostream>
#include <binary.hpp>
#include "controller.hpp"

using namespace std;


int main() {
    string file_path;
    cout << "Введите имя файла: ";
    getline(cin, file_path, '\n');
    process_file(file_path, ios::binary | ios::in | ios::out, [](std::fstream& file) {
        Controller controller{&file};

        while (true) {
            cout << "1. Найти машину по номеру\n"
                 << "2. Удалить запись о машине по номеру \n"
                 << "3. Проиндексировать файл заново\n"
                 << "4. Выйти \n"
                 << "[1-4]:";

            int op;
            cin >> op;
            std::string licence;
            CarRecord record;
            bool ok;
            switch (op) {
                case 1:
                    cout << "Введите номер машины: ";
                    cin >> licence;
                    record = controller.get(licence);
                    if (strlen(record.licence_plate) == 0) {
                        cout << "Машина не найдена" << endl;
                        break;
                    }
                    cout << "Номер: " << record.licence_plate << "\n"
                         << "Марка машины: " << record.model << "\n"
                         << "Владелец: " << record.owner << "\n"
                         << "Числится в угоне: " << (record.is_hijacked ? "да" : "нет") << "\n";
                    break;
                case 2:
                    cout << "Введите номер машины: ";
                    cin >> licence;
                    ok = controller.erase(licence);
                    cout << (ok ? "Машина удалена" : "Машина не найдена") << endl;
                    break;
                case 3:
                    controller.index_all();
                    cout << "Готово" << endl;
                    break;
                case 4:
                    return;
                default:
                    std::cout << "Вы ввели неверный номер операции\n";
            }
        }
    });
}