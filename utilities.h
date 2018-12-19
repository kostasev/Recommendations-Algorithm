//
// Created by kosti on 11/20/2018.
//

#ifndef PROJECT2_UTILITIES_H
#define PROJECT2_UTILITIES_H

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <chrono>
#include <random>
#include <unistd.h>
#include "data_point.h"

using namespace std;

void get_data_lengths(string input,int& lines, int& d);
void feed_data_set(string input, data_point<double> *pPoint,int d,int num);
int num_columns(string line);

#endif //PROJECT2_UTILITIES_H
