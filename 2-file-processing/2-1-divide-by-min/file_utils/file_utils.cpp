#include "file_utils.hpp"

namespace file_utils {

    void print_file(std::istream& file) {
        int value;
        while (file >> value) {
            std::cout << value << std::endl;
        }
    }

    int count_numbers(std::istream& file) {
        int counter = 0;
        int _;
        while (file >> _) {
            counter++;
        }
        return counter;
    }

    void append_number(std::ostream& file, int value) {
        file.seekp(0, std::ios_base::end);
        file << value;
    }

    void mutate(std::fstream& file) {
        std::vector<int> items(std::istream_iterator<int>{file}, std::istream_iterator<int>{});

        file.clear();
        file.seekp(0, std::ios_base::beg);
        int first = items.front();
        int last = items.back();
        for (int item: items) {
            file << item * (first + last) << std::endl;
        }
    }

    get_number_result_t get_number(std::istream& file, int pos) {
        get_number_result_t result{};
        if (pos <= 0) {
            goto NOT_EXIST;
        }

        while (file >> result.number && pos - 1 > 0) {
            pos--;
        }

        if (!file) {
            goto NOT_EXIST;
        }

        result.exists = true;
        return result;

        NOT_EXIST:
        result.exists = false;
        return result;
    }
}

