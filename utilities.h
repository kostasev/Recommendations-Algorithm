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
#include <map>
#include <sstream>
#include <unistd.h>
#include "data_point.h"
#include "constants.h"
#include "Hash_table.h"

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
void cluster_feels(map<int,tweet> raw_tweets, map<int,data_point<double>>& feels,vector<string>* coinz, map<string,float> voc);
void print_feels(map<int,data_point<double>> feels);
bool vectors_eq(std::vector<double> v1, std::vector<double> v2);
void user_feels(map<int,tweet> raw_tweets, map<int,data_point<double>>& feels,vector<string>* coinz, map<string,float> voc);
void random_vector(vector<int> &r,int k );
int create_tables(vector<Hash_table> &tables,string metric,int num_lines,int dim);
void feed_tables(vector<Hash_table> &tables,data_point<double> *data_set,int table_size,int num_lines,vector<int> r);
double cosine_similarity(vector<double> p1,vector<double> p2);
vector<recom> rec_nn(map<string,value_point<double>> bucks, data_point<double> point);
void print_recom(vector<recom> recomm,data_point<double> data,int coins,vector<string>* cn);
vector<recom> rec_nn_cluster(data_point<double> cen, data_point<double> point);
#endif //PROJECT2_UTILITIES_H
