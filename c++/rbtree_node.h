#ifndef __RBTREE_NODE_H__
#define __RBTREE_NODE_H__

#include <iostream>
#include "btree.h"

using std::cout;
using std::endl;

template<class T>
struct rbtree_node : public btree_node<T>{

    enum COLOR{
        RED ,
        BLACK
    } ;

    rbtree_node(T d) : btree_node<T>(d),color(RED){}

    unsigned char color ;
    
} ;

#endif
