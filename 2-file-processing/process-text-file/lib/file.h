//
// Created by Burenin Artem on 28.09.2023.
//

#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <vector>


void display_content(const std::string& file_path);

int count(const std::string& file_path);

int number_at_pos(const std::string& file_path, int pos, bool& ok);

void append_number(const std::string& file_path, int value);

bool multiply_by_sum_of_first_and_last(const std::string& file_path);

#endif //FILE_H
