//
// Created by Burenin Artem on 19.10.2023.
//

#ifndef CONTROLLER
#define CONTROLLER

#include <string>
#include <fstream>
#include <binary.hpp>
#include <hash_map.hpp>

class Controller {
public:

    explicit Controller(std::iostream* file) : file(file) {
        index_all();
    }

public:

    CarRecord get(const std::string& licence) {
        CarRecord result{};

        if (!map.contains(licence)) {
            return result;
        }
        auto index = map[licence];
        utils::get_by_index(*file, &result, index);
        return result;
    }

    bool erase(const std::string& licence) {
        if (!map.contains(licence)) {
            return false;
        }

        auto index = map[licence];
        utils::remove_by_index(*file, index);
        map.erase(licence);
        return true;
    }

    void index_all() {
        map = burenotti::hash_map::HashMap<std::string, int>{};

        file->clear();
        int index = 0;
        CarRecord record;
        while (*file) {
            binary::unmarshall(*file, &record);
            std::string licence{record.licence_plate};
            if (licence.empty()) {
                break;
            }
            map[licence] = index;
            index++;
        }
        file->clear();
    }

protected:

    Controller() = default;

protected:
    burenotti::hash_map::HashMap<std::string, int> map;
    std::iostream* file = nullptr;
};

#endif //CONTROLLER
