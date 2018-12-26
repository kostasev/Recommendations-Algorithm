//
// Created by kevangel on 22/11/18.
//

#include "cluster.h"


cluster::cluster(data_point<double> d) {
    set_centroid(d);
}

void cluster::set_centroid(data_point<double> cen){
    this->centroid=cen;
}

data_point<double> cluster::get_centroid(){
    return this->centroid;
}

void cluster::add_item(data_point<double> item){
    this->clitems.push_back(item);
}

vector<data_point<double>> cluster::get_items(){
    return this->clitems;
}

void cluster::empty_clitems() {
    this->clitems.clear();
}
void cluster::print_centroid() {
    cout << "Item: " << centroid.name << endl;
    for (int i = 0 ; i< centroid.point.size(); i++){
        cout << centroid.point[i] << " " ;
    }
    cout <<endl;
}

void cluster::print_cluster() {
    cout << "Centroid: " << this->centroid.name << endl;
    cout << "Cluster Size: " << this->clitems.size() << endl;
    for (int i = 0 ; i< this->clitems.size() ; i++){
        cout << this->clitems[i].name << " " ;
    }
    cout <<endl;
}

int cluster::check_equal(cluster xx) {
    if(this->clitems.size()!=xx.clitems.size()) return 0;
    vector<data_point<double>> zz=xx.get_items();
    for (int i=0; i< this->clitems.size();i++){
        if (this->clitems[i].point!=zz[i].point){
            return 0;
        }
    }
    return 1;
}