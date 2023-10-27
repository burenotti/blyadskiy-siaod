//
// Created by Burenin Artem on 28.09.2023.
//

#ifndef FILE_UTILS
#define FILE_UTILS

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>

namespace file_utils {

    struct get_number_result_t {
        int number;
        bool exists;
    };


    extern void print_file(std::istream& file);

    extern int count_numbers(std::istream& file);

    extern get_number_result_t get_number(std::istream& file, int pos);

    extern void append_number(std::ostream& file, int value);

    extern void mutate(std::fstream& file);
}

#endif //FILE_UTILS
