#ifndef __BTREE_NODE_H__
#define __BTREE_NODE_H__

#include <iostream>
#include "node.h"

using std::cout;
using std::endl;

template<class T>
struct btree_node : public node<T>{

    btree_node(T d) : node<T>::node(d),pleft(NULL),pright(NULL),pparent(NULL){} 
    bool operator< (const node<T>&) ;
    bool operator> (const node<T>&) ;
    bool operator<=(const node<T>&) ;
    bool operator>=(const node<T>&) ;
    bool operator!=(const node<T>&) ;

    struct btree_node*  pleft;
    struct btree_node*  pright;
    struct btree_node*  pparent;
};

template<class T>
bool
btree_node<T>::operator<(const node<T>& t)
{
    return (this->data < t.data) ;
}

template<class T>
bool
btree_node<T>::operator>(const node<T>& t)
{
    return (this->data > t.data) ;
}

template<class T>
bool
btree_node<T>::operator<=(const node<T>& t)
{
    return (this->data <= t.data) ;
}

template<class T>
bool
btree_node<T>::operator>=(const node<T>& t)
{
    return (this->data >= t.data) ;
}

template<class T>
bool
btree_node<T>::operator!=(const node<T>& t)
{
    return (this->data != t.data) ;
}

#endif
