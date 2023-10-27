//
// Created by Burenin Artem on 05.10.2023.
//
#include "utils.hpp"


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


void utils::remove_record(const std::string& file, const char* target) {
    std::vector<CarRecord> records;
    process_file(file, std::ios::in | std::ios::out | std::ios::binary, [&records, target](std::fstream& inout) {

        while (inout) {
            CarRecord car;
            binary::unmarshall(inout, &car);
            if (inout.good())
                records.push_back(car);
        }
        auto pos = std::find_if(records.begin(), records.end(), [=](const CarRecord& record) {
            return strcmp(record.licence_plate, target) == 0;
        });
        if (pos == records.end())
            return;

        *pos = records.back();
        records.pop_back();

    });
    process_file(file, std::ios::out | std::ios::binary, [&records](std::ostream& out) {
        for (auto& record: records) {
            binary::marshall(out, &record);
        }
    });
}

