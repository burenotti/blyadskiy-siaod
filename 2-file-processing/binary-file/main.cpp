#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <utils.hpp>

using namespace std;


int main() {
    bool ok;
    int opt;
    do {
        ok = true;
        cout << "Выберите операцию:\n"
             << "1. Вывести бинарный файл на экран\n"
             << "2. Преобразовать бинарный файл в текстовый\n"
             << "3. Преобразовать текстовый файл в бинарный\n"
             << "4. Вывести список украденных машин\n"
             << "5. Удалить сведения о машине по номеру записи\n"
             << "6. Проверить угнана ли машина по ее регистрационному знаку\n"
             << "[1-6]: ";

        cin >> opt;
        if (opt < 1 || opt > 6) {
            cout << "Введите корректное значение" << endl;
            ok = false;
        }

    } while (!ok);
    cin.ignore();
    std::string path;
    do {
        ok = true;
        cout << "Введите путь к файлу: ";
        std::getline(cin, path, '\n');
        if (!filesystem::exists(path)) {
            cout << "Данный файл не существует" << endl;
            ok = false;
        }
    } while (!ok);

    switch (opt) {
        case 1:
            process_file(path, ios::in | ios::binary, [](auto& in) {
                utils::binary_to_text(in, cout);
            });
            break;
        case 2:
            process_file(path, ios_base::in | ios_base::binary, [](auto& in) {
                process_file("output.txt", ios::trunc | ios_base::out, [&in](auto& out) {
                    utils::binary_to_text(in, out);
                    cout << "Файл преобразован и сохранен в output.txt" << endl;
                });
            });
            break;
        case 3:
            process_file(path, ios::in, [](auto& in) {
                process_file("output.bin", ios::out | ios::trunc | ios::binary, [&in](auto& out) {
                    utils::text_to_binary(in, out);
                    cout << "Файл преобразован и сохранен в output.bin" << endl;
                });
            });
            break;
        case 4:
            process_file(path, ios::in | ios::binary, [](auto& in) {
                utils::list_hijacked(in, cout);
            });
            break;
        case 5: {
            string licence;
            cout << "Введите номер машины: ";
            cin >> licence;
            utils::remove_record(path, licence.c_str());
        }
            break;
        case 6:
            process_file(path, ios::in | ios::binary, [](auto& in) {
                string licence;
                cout << "Введите номер машины: ";
                cin >> licence;
                bool ok = utils::is_hijacked(in, licence.c_str());
                if (ok) {
                    cout << "Машина в угоне";
                } else {
                    cout << "Машина не в угоне";
                }
            });
    }
}