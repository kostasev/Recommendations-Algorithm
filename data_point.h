//
// Created by kosti on 11/20/2018.
//

#ifndef PROJECT2_DATA_POINT_H
#define PROJECT2_DATA_POINT_H
#include <iostream>
#include <vector>

template <typename T>
struct data_point {
    std::vector<T>  point;
    std::string     name;
};

#endif //PROJECT2_DATA_POINT_H
