//
// Created by kosti on 10/20/2018.
//

#ifndef UNTITLED_KEY_H
#define UNTITLED_KEY_H
#include <iostream>
#include <vector>

struct Key {
    int hash_val;

    bool operator==(const Key& lhs) const
    {
        return (lhs.hash_val==hash_val);
    }


};

namespace std {
    template<>
    struct hash<Key> {
        size_t operator()(const Key &k) const {
            return k.hash_val;
        }
    };
};


#endif //UNTITLED_KEY_H
