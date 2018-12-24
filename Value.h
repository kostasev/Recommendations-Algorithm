//
// Created by kosti on 10/25/2018.
//

#ifndef UNTITLED_VALUE_H
#define UNTITLED_VALUE_H
#include <iostream>
#include <vector>
#include "./data_point.h"


template <typename T>
struct value_point {
    std::vector<T>  point;
    data_point<T> *p;
};

#endif //UNTITLED_VALUE_H
