//
// Created by Burenin Artem on 05.10.2023.
//
#include "binary.hpp"


void binary::marshall(std::ostream& out, CarRecord* car) {
    out.write(reinterpret_cast<char*>(car), sizeof(CarRecord));
}

void binary::unmarshall(std::istream& in, CarRecord* car) {
    in.read(reinterpret_cast<char*>(car), sizeof(CarRecord));
}

void text::marshall(std::ostream& out, CarRecord* car) {
    out.write(car->licence_plate, std::strlen(car->licence_plate));
    out << "\n";
    out.write(car->model, std::strlen(car->model));
    out << "\n";
    out.write(car->owner, std::strlen(car->owner));
    out << "\n";
    out << car->is_hijacked;
    out << "\n";
}

void text::unmarshall(std::istream& in, CarRecord* car) {
    in.getline(car->licence_plate, sizeof(car->licence_plate));
    in.getline(car->model, sizeof(car->model));
    in.getline(car->owner, sizeof(car->owner));
    in >> car->is_hijacked;
    in.ignore();
}

void utils::text_to_binary(std::istream& in, std::ostream& out) {
    while (in) {
        CarRecord car;
        text::unmarshall(in, &car);
        binary::marshall(out, &car);
    }
}

void utils::binary_to_text(std::istream& in, std::ostream& out) {
    while (in) {
        CarRecord car;
        binary::unmarshall(in, &car);
        if (in.good())
            text::marshall(out, &car);
    }
}

void utils::list_hijacked(std::istream& in, std::ostream& out) {
    while (in) {
        CarRecord car;
        binary::unmarshall(in, &car);
        if (car.is_hijacked) {
            text::marshall(out, &car);
        }
    }
}

bool utils::is_hijacked(std::istream& in, const char* target) {
    while (in) {
        CarRecord car;
        binary::unmarshall(in, &car);
        if (strcmp(car.licence_plate, target) == 0) {
            return car.is_hijacked;
        }
    }
    return false;
}


void utils::remove_by_index(std::iostream& file, int index) {
    CarRecord last;
    file.seekg(-(int) sizeof(CarRecord), std::ios_base::end);
    binary::unmarshall(file, &last);
    file.seekp((int) sizeof(CarRecord) * index);
    binary::marshall(file, &last);
}

void utils::get_by_index(std::istream& file, CarRecord* record, int index) {
    file.clear();
    int offset = index * (int) sizeof(CarRecord);
    if (!file.seekg(offset), std::ios::beg) {
        memset(record, 0, sizeof(CarRecord));
        return;
    }
    binary::unmarshall(file, record);
    file.clear();
}