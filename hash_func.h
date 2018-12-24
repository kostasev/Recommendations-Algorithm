//
// Created by kosti on 10/21/2018.
//

#ifndef UNTITLED_HASH_FUNC_H
#define UNTITLED_HASH_FUNC_H
#include<iostream>
#include <vector>
#include <random>
#include "Key.h"
#include "data_point.h"
#include "Value.h"

class hash_func {
private:
    std::string func;
    std::vector<std::vector<double>> v;
    std::vector<double> t;
public:
    hash_func(int,int,std::string);
    ~hash_func();
    void clean();
    value_point<double> hash_value(data_point<double>&,int&,int,int,std::vector<int>);
};


#endif //UNTITLED_HASH_FUNC_H
