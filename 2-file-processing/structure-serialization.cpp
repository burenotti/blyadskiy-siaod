#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <cstring>
#include <fstream>

#ifdef __WINNT
#include <winsock.h>
#endif

using namespace std;


struct Car;

struct from_bytes_return_t {
    unique_ptr<Car> car;
    bool ok;
};

struct to_bytes_return_t {
    unique_ptr<const char> bytes;
    size_t size;
};

struct Car {

    static constexpr char END = 0b0111'1111;

    Car(string brand, string owner, string license_plate_number, bool is_hijacked) {
        _brand = std::move(brand);
        _owner = std::move(owner);
        _licence = std::move(license_plate_number);
        _is_hijacked = is_hijacked;
    }


    string_view brand() {
        return _brand;
    }

    string_view owner() {
        return _owner;
    }

    string_view licence() {
        return _licence;
    }

    bool hijacked() {
        return _is_hijacked;
    }

    to_bytes_return_t to_bytes() {
        size_t buffer_size = (_owner.size() + 1 + _brand.size() + 1
                              + _licence.size() + 1 +
                              sizeof(_is_hijacked) + sizeof(END));
        char* buffer = new char[buffer_size];

        char* cursor = buffer;

        // Put licence plate number to the buffer
        strcpy(cursor, _licence.c_str());
        cursor += _licence.size() + 1;

        // Put brand name to the buffer
        strcpy(cursor, _brand.c_str());
        cursor += _brand.size() + 1;


        // Put owner to the buffer
        strcpy(cursor, _owner.c_str());
        cursor += _owner.size() + 1;

        // Put hijacked flag to the buffer
        *reinterpret_cast<bool*>(cursor) = _is_hijacked;
        cursor += sizeof(_is_hijacked);

        // Put end mark to the buffer
        *cursor = END;

        return to_bytes_return_t{
                unique_ptr<const char>{buffer},
                buffer_size,
        };
    }

    static from_bytes_return_t from_bytes(const char* bytes, int size) {

        from_bytes_return_t result{
                .car = std::unique_ptr<Car>(new Car()),
                .ok = true,
        };
        const char* cursor = bytes;

        if (size < sizeof(size_t)) {
            result.ok = false;
            return result;
        }

        result.car->_licence = string(cursor);
        cursor += result.car->_licence.size() + 1;

        if (distance(bytes, cursor) > size) {
            result.ok = false;
            return result;
        }

        result.car->_brand = string(cursor, 3);
        cursor += result.car->_brand.size() + 1;

        if (distance(bytes, cursor) > size) {
            result.ok = false;
            return result;
        }

        result.car->_owner = string(cursor);
        cursor += result.car->_owner.size() + 1;

        if (distance(bytes, cursor) > size) {
            result.ok = false;
            return result;
        }

        result.car->_is_hijacked = reinterpret_cast<const bool*>(cursor)[0];
        cursor++;

        if (distance(bytes, cursor) > size) {
            result.ok = false;
            return result;
        }

        return result;
    }


private:

    Car() = default;

private:
    string _owner;
    string _brand;
    string _licence;
    bool _is_hijacked = false;
};


int main() {

    vector<Car> cars = {
            Car{"BMW", "Vasya pupkin", "K111PA", true},
            Car{"BMW", "Vasya Nikolayevich pupkin", "K111PA", true},
            Car{"BMW", "John pupkin", "K111PA", true},
            Car{"BMW", "Vasya pupkin", "K111PA", true},
    };

    ofstream out("output.txt", std::ios_base::binary);

    for (auto car: cars) {
        auto [bytes, size] = car.to_bytes();
        out.write(bytes.get(), (int) size);
    }

    out.close();

    ifstream in("output.txt", std::ios_base::binary);

    string buffer;
    int counter = 0;
    while (getline(in, buffer, Car::END)) {
        counter++;
        in.get();
        auto [car, ok] = Car::from_bytes(buffer.c_str(), (int) buffer.size());
        if (!ok) {
            cout << "An unexpected error occurred while parsing. aborting...";
            break;
        }

        cout << "Entity #" << counter << "\n"
             << "Brand:       " << car->brand() << "\n"
             << "Licence:     " << car->licence() << "\n"
             << "Owner:       " << car->owner() << "\n"
             << "Is hijacked: " << (car->hijacked() ? "true" : "false")
             << "\n\n";
    }
    return 0;
}