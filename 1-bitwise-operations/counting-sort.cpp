#include <iostream>
#include <fstream>
#include <exception>
#include <chrono>
#include <iterator>
#include <optional>

using namespace std;

#define MAX_SIZE (10'000'000 / sizeof(BitsetItem))

using BitsetItem = unsigned long long;
using Bitset = BitsetItem*;


bool bitset_has(const Bitset& bitset, int size, int value) {
    constexpr BitsetItem one = 1;

    if (value >= size)
        return false;

    auto byte = value / ((int) sizeof(BitsetItem) * 8);
    auto offset = value % ((int) sizeof(BitsetItem) * 8);
    BitsetItem mask = one << offset;
    return (bitset[byte] & mask) != 0;
}

void bitset_push(Bitset bitset, int size, int value) {
    constexpr BitsetItem one = 1;

    if (value >= size)
        return;

    int byte = value / ((int) sizeof(BitsetItem) * 8);
    int offset = value % ((int) sizeof(BitsetItem) * 8);
    BitsetItem mask = one << offset;

    bitset[byte] = bitset[byte] | mask;
}


template<class InIter, class OutIter>
requires std::input_iterator<InIter> && std::output_iterator<OutIter, int>
void bitset_sort(InIter begin, InIter end, OutIter out_iter) {
    int size = MAX_SIZE / 8;
    auto* bitset = new BitsetItem[size];

    while (begin != end) {
        bitset_push(bitset, MAX_SIZE, *begin);
        begin++;
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        if (bitset_has(bitset, MAX_SIZE, i)) {
            *out_iter++ = i;
        }
    }
}

void run(const string& input_path, const string& output_path) {
    ifstream input(input_path);
    if (!input.is_open()) {
        throw runtime_error{"could not open input file"};
    }

    ofstream output(output_path);
    if (!output.is_open()) {
        throw runtime_error{"could not open output file"};
    }

    std::istream_iterator<int> input_begin{input};
    std::istream_iterator<int> input_end{};

    std::ostream_iterator<int> out_iter{output, "\n"};
    bitset_sort(input_begin, input_end, out_iter);
    output.close();
}

template<class T, class... Args, class Clock = chrono::high_resolution_clock>
requires invocable<T, Args...>
Clock::duration time_it(T&& function, const string& msg, Args&& ... args) {
    auto start_time = chrono::high_resolution_clock::now();
    optional<string> caught;
    try {
        function(std::forward<Args>(args)...);
    } catch (exception& e) {
        caught = e.what();
    }
    auto end_time = chrono::high_resolution_clock::now();
    auto delta = end_time - start_time;
    auto exec_time_ms = chrono::duration_cast<chrono::milliseconds>(delta).count();
    cout << "\n";
    if (caught.has_value())
        cout << "###  Terminated by uncaught exception: " << caught.value() << endl;

    cout << "###  " << msg << "\n"
         << "###  Execution time: " << exec_time_ms << " мс" << endl;
    return delta;
}

int main() try {
    time_it(run, "Small 10 items", "./tests/1_small_10.txt", "./sorted/1_small_10.txt");
    time_it(run, "Medium 1000 items", "./tests/2_medium_1000.txt", "./sorted/2_medium_10_000.txt");
    time_it(run, "Medium 10'000 items", "./tests/3_medium_10_000.txt", "./sorted/3_medium_10_000.txt");
    time_it(run, "Large 1'000'000 items", "./tests/4_large_1_000_000.txt", "./sorted/4_large_000_000.txt");
    time_it(run, "Large 10'000'000 items", "./tests/5_large_10_000_000.txt", "./sorted/5_large_10_000_000.txt");
} catch (runtime_error& e) {
    cerr << e.what();
}