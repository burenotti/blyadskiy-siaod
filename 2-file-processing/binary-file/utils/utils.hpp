//
// Created by Burenin Artem on 05.10.2023.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <filesystem>
#include <concepts>
#include <iostream>
#include <algorithm>


template<class F>
requires std::invocable<F, std::fstream&>
extern void process_file(const std::string& path, std::ios_base::openmode mode, F&& func) {
    using namespace std;

    if (((mode & ios_base::out) == 0) && !filesystem::exists(path)) {
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


struct CarRecord {
    char model[256] = "";
    // Format A000AA111\0
    char licence_plate[16] = "";
    char owner[256] = "";
    bool is_hijacked = false;
};


namespace binary {

    extern void marshall(std::ostream& out, CarRecord* car);

    extern void unmarshall(std::istream& in, CarRecord* car);

}

namespace text {
    extern void marshall(std::ostream& out, CarRecord* car);

    extern void unmarshall(std::istream& in, CarRecord* car);
}

namespace utils {

    extern void text_to_binary(std::istream& in, std::ostream& out);

    extern void binary_to_text(std::istream& in, std::ostream& out);

    extern void list_hijacked(std::istream& in, std::ostream& out);

    extern bool is_hijacked(std::istream& in, const char* target);

    extern void remove_record(const std::string& file, const char* target);


}


#endif //UTILS_HPP
