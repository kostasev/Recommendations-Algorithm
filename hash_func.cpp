//
// Created by kosti on 10/21/2018.
//

#include "hash_func.h"
#include <climits>
#include <random>
#include "constants.h"
#include "data_point.h"


hash_func::hash_func(int dimension, int k,std::string func){
    std::random_device rd; // assume unsigned int is 32 bits
    std::mt19937_64 generator(rd()); // seeded with 256 bits of entropy from random_device
    std::normal_distribution<double>         n_distribution(0.0,1.0);
    std::uniform_real_distribution<double>   uint_distW(0.0,(float) const_lsh::w);
    std::vector<double> temp(dimension);
    this->func=func;
    for(int i=0 ; i < k ; i++ ) {
        this->t.push_back(uint_distW(generator));
        for(int j=0 ; j < dimension ; j++ ){
            temp[j]=n_distribution(generator);
        }
        v.push_back(temp);
    }
}

hash_func::~hash_func(){
    for (int i=0;i<this->v.size();i++)
        this->v[i].clear();
    this->t.clear();
    this->v.clear();
}

void hash_func::clean(){
    for (int i=0;i<this->v.size();i++)
        this->v[i].clear();
    this->t.clear();
    this->v.clear();
}

value_point<double> hash_func::hash_value(data_point<double>& k,int &f,int size,int table_size,std::vector<int> r){
    value_point<double> point;
    if (this->func=="euclidean"){
        long int sum=0;
        double h=0.0;
        for(int j=0; j<size; j++){
            for (int i=0; i<k.point.size(); i++){
                h+=this->v[j][i]*k.point[i];
            }
            h=h+this->t[j];
            h=h/(double)const_lsh::w;
            point.point.push_back(int(h));
            sum+=int(h)*r[j];
            h=0.0;
        }
        f=int((((sum%INT_MAX-5)%table_size)+table_size)%table_size);
        point.p=&k;
        return point;
    }else {
        long int sum=0;
        double h=0.0;
        for(int j=0; j<size; j++){
            for (int i=0; i<k.point.size(); i++){
                h+=this->v[j][i]*k.point[i];
            }
            int coin=0;
            if (h>=0){
                coin=1;
                point.point.push_back(1);
            }
            else{
                point.point.push_back(0);
            }
            sum+= (int) pow(2.0,double(size-1-j))*coin;
            h=0.0;
        }
        f=sum;
        point.p=&k;
        return point;
    }

}
