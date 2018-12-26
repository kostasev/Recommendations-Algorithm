//
// Created by kevangel on 22/11/18.
//

#ifndef PROJECT2_CLUSTER_H
#define PROJECT2_CLUSTER_H
#include <vector>
#include "data_point.h"
using namespace std;
class cluster {
private:
    data_point<double> centroid;
    vector<data_point<double>> clitems;
public:
    cluster(data_point<double>);
    void set_centroid(data_point<double>);
    data_point<double> get_centroid();
    void add_item(data_point<double>);
    vector<data_point<double>> get_items();
    void empty_clitems();
    void print_centroid();
    void print_cluster();
    int check_equal(cluster);
};


#endif //PROJECT2_CLUSTER_H
