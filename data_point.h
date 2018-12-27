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
    int sum;
};

struct tweet {
    int user_id;
    int tweet_id;
    std::vector<std::string> tokens;
};

struct recom{
    double value;
    int coin;
};

#endif //PROJECT2_DATA_POINT_H
