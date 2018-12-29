//
// Created by kostasev on 10/15/2018.
//

#ifndef UNTITLED_CONSTANTS_H
#define UNTITLED_CONSTANTS_H

#include <iostream>

namespace consts {
    const std::string input_file="./data_files/in.csv" ;
    const std::string coins="./data_files/coins_queries.csv" ;
    const std::string voc="./data_files/vader_lexicon.csv" ;
    const std::string tweets="./data_files/tweets_dataset_small.csv" ;
    const std::string clust_50="./clusters/000_50.txt" ;
    const double z =0.0015915494309;// 0.3141592;
}

namespace const_lsh {
    const int k = 4;
    const int L = 6;
    const int w = 350;
    const double R = 400.0;
    const int table_size = 4;
    const std::string def_func = "cosine";
}

#endif //UNTITLED_CONSTANTS_H
