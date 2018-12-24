//
// Created by kosti on 10/20/2018.
//

#include "Hash_table.h"
#include "Value.h"
#include "utilities.h"
#include "constants.h"

Hash_table::Hash_table( int buck, int dimension, int k, std::string func){
    this->k=k;
    this->d=dimension;
    this->hash_tb.rehash(buck);
    this->hash_tb.max_load_factor(10000);
    //if (func == "euclidean") {
        hfunc = new hash_func(dimension, k,func);
    //}
}
Hash_table::~Hash_table() {
    this->hash_tb.clear();
    this->hfunc->clean();
    this->hfunc=NULL;
}

void Hash_table::add_item(data_point<double>& k,int tb_size,std::vector<int> r){
    Key f;
    value_point<double> point = this->hfunc->hash_value(k,f.hash_val,this->k,tb_size,r);
    this->hash_tb.insert({f,point});
}

Key Hash_table::query_item(data_point<double>& k,int tb_size,std::vector<int> r){
    Key f;
    this->hfunc->hash_value(k,f.hash_val,this->k,tb_size,r);
    return f;
}

void Hash_table::get_bucket(data_point<double>& z,Key k, std::map<std::string,value_point<double>>& mp,std::vector<int> r){
    Key f;
    auto range=this->hash_tb.equal_range(k);
    value_point<double> point = this->hfunc->hash_value(z,f.hash_val,this->k,2500,r);
    for(auto it = range.first; it != range.second ; it++ ){
        if (vectors_eq(it->second.point , point.point)){
            mp.insert({it->second.p->name,it->second});
        }
    }
}

void Hash_table::print_stats(){
    std::cout << "Number of buckets: " << hash_tb.bucket_count() << std::endl;
    std::cout << "Number of  Max buckets: " << hash_tb.max_bucket_count() << std::endl;
    std::cout << "Hash table size: " << hash_tb.size() << std::endl;
    for (unsigned i=0; i<hash_tb.bucket_count(); i++) {
        std::cout << "bucket #" << i << " has " << hash_tb.bucket_size(i) << " elements.\n";
    }
}