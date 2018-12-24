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
#include <map>
#include <sstream>
#include "constants.h"

using namespace std;

void get_data_lengths(string input,int& lines, int& d);
void feed_data_set(string input, data_point<double> *pPoint,int d,int num);
int num_columns(string line);
void feed_coins(string file, vector<string>* coins );
void feed_voc(string file, map<string,float>& voc );
void add_vectors(vector<double>& v1,vector<double> v2);
void feed_tweets(string file, map<int,tweet>& tweets );
void mean_vec(vector<double> vec,int sum);
void mean_all(map<int,data_point<double>>& feels);
bool is_mentioned(vector<string> v1,vector<string> v2);
vector<double> calc_feeling(vector<string> tweet ,map<string,float> voc ,vector<string> * coins);
void user_feels(map<int,tweet> raw_tweets, map<int,data_point<double>>& feels,vector<string>* coinz, map<string,float> voc);
void cluster_feels(map<int,tweet> raw_tweets, map<int,data_point<double>>& feels,vector<string>* coinz, map<string,float> voc);

void print_feels(map<int,data_point<double>> feels);
#endif //PROJECT2_UTILITIES_H
