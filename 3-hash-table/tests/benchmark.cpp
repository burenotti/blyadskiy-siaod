#define CATCH_CONFIG_MAIN

#include <sstream>
#include <catch2/catch2.hpp>
#include <binary.hpp>
#include <controller.hpp>

void put_random_c_str(std::mt19937_64& random, char* dest, size_t size) {
    memset(dest, '\0', size);
    for (size_t i = 0; i < size - 1; i++) {
        dest[i] = 'A' + (char) (random() % ('Z' - 'A'));
    }
}

void generate_random_record(std::mt19937_64& random, CarRecord* record) {
    record->is_hijacked = static_cast<bool>(random() % 2);
    put_random_c_str(random, record->owner, 30);
    put_random_c_str(random, record->model, 30);
    put_random_c_str(random, record->licence_plate, 10);
}

void generate_random_stream(std::iostream& stream, std::mt19937_64& random, size_t size) {
    CarRecord tmp;
    for (size_t i = 0; i < size; i++) {
        generate_random_record(random, &tmp);
        binary::marshall(stream, &tmp);
    }
}

TEST_CASE_METHOD(Controller, "controller search benchmark", "[controller]") {

    std::mt19937_64 random{std::random_device{}()};


//    BENCHMARK_ADVANCED("search 100 items")(Catch::Benchmark::Chronometer meter) {
//            std::stringstream stream100{std::ios::binary | std::ios::in | std::ios::out};
//            generate_random_stream(stream100, random, 100);
//            this->file = &stream100;
//            this->index_all();
//            this->map.visit_all([&](const std::string& key, int) -> bool {
//                meter.measure([&] {
//                    this->get(key);
//                });
//                return true;
//            });
//        };


    BENCHMARK_ADVANCED("search 1'000 items")(Catch::Benchmark::Chronometer meter) {
            std::stringstream stream{std::ios::binary | std::ios::in | std::ios::out};
            generate_random_stream(stream, random, 1'000);
            this->file = &stream;
            this->index_all();
            this->map.visit_all([&](const std::string& key, int) -> bool {
                meter.measure([&] {
                    return this->get(key);
                });
                return true;
            });
        };

}
