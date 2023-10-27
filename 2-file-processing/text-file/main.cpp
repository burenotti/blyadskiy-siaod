#include <iostream>
#include <fstream>
#include <filesystem>
#include <file_utils.hpp>

using namespace std;

template<class F>
requires invocable<F, fstream&>
void process_file(const string& path, ios_base::openmode mode, F&& func) {

    if (!filesystem::exists(path)) {
        cout << "Provided file does not exist" << endl;
        return;
    }

    fstream file{path, mode};

    if (!file.is_open()) {
        cout << "Unable to open file" << endl;
        return;
    }

    func(file);

    if (!file.eof() && file.fail()) {
        cout << "During file processing an error occurred. Result may be incorrect" << endl;
    }
    file.close();
}


int main() {

    string file_path;
    cout << "Enter path to file: ";
    getline(cin, file_path, '\n');


    INPUT:

    cout << "1. Print text file content \n"
         << "2. Append new line to the end of the file \n"
         << "3. Read a number in the file using its ordering number \n"
         << "4. Count numbers in the file \n"
         << "5. Divide by minimum \n"
         << "Select operation [1-5]: \n";

    int op;
    cin >> op;

    switch (op) {
        case 1:
            process_file(file_path, ios_base::in, [](istream& file) {
                file_utils::print_file(file);
            });
            break;
        case 2:
            process_file(file_path, ios_base::app, [](ostream& file) {
                int value;
                cin >> value;

                file_utils::append_number(file, value);
            });
            break;
        case 3:
            process_file(file_path, ios_base::in, [](istream& file) {
                int pos;
                cout << "Enter position in the file: ";
                cin >> pos;

                auto [value, exists] = file_utils::get_number(file, pos);
                if (!exists) {
                    cout << "There is no number on position " << pos << " in the file";
                }

                cout << "The " << pos << "-th number is " << value << endl;
            });
            break;
        case 4:
            process_file(file_path, ios_base::in, [](istream& file) {
                cout << "There are " << file_utils::count_numbers(file) << " numbers in the file";
            });
            break;
        case 5:
            process_file(file_path, ios_base::in | ios_base::out, [](fstream& file) {
                file_utils::mutate(file);
            });
            break;
        default:
            cout << "You've selected invalid option. Please, try again." << endl;
            goto INPUT;
    }
}
