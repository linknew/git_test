#ifndef __RBTREE_H_
#define __RBTREE_H_

#include <iostream>
#include "rbtree_node.h"
#include "btree_guide.h"
#include "btree.h"

using std::cout ;
using std::cerr ;
using std::endl ;
using std::setw ;

template<class T>
class rbtree : public btree<T>{

    public:

        rbtree() : btree()
        { 
#if _DBG
            cerr << "construction rbtree:" << this << endl;
#endif
        }

        void remove(btree_node<T> *t) ;
        void insert(btree_node<T> *t) ;

        static inline void set_color(rbtree_node<T> *t, unsigned char color)
        {
            if(!t)
                throw_err(ERR_INV_ARG) ;

            t->color = color ;
        }

        static inline bool is_red(const rbtree_node<T> *t)
        {
            return (t && (t->color == RED)) ;
        }

    protected:
} ;

template<class T>
void rbtree<T>::remove(btree_node<T> *cur)
{
    //btree_node<T> *wanderer = NULL ;
    btree_node<T>* pa = NULL ;
    btree_node<T>* ch = NULL ;
    childid id = ASLEFT ;

    if (!cur)
        return ;

    // merge children into one branch
    if (has_left_child(cur) && has_right_child(cur)){
        ch = lost_child(cur,ASRIGHT) ;
        adopt_child(get_rightmost(get_left_child(cur)), ch, ASRIGHT) ;
    }

    // now, there is only one branch.

    // cut off the 'cur' node from chain
    id = get_childid(cur) ;
    pa = leave_parent(cur) ;
    ch = lost_child(cur, has_left_child(cur) ? ASLEFT : ASRIGHT) ;

    // re-chain
    adopt_child(pa,ch,id) ;

    // don't forget update _proot
    if(!pa)
        _proot = ch ;

    // all done, delete the 'cur'
    delete cur ;

    return ;
}

template<class T>
void rbtree<T>::insert(btree_node<T>* cur)
{
    int state = 0 ;
    node<T>* p = NULL ;

    if(!cur)
        return ;

    btree_explorer<T> e ;
    task_searching<T> t(_proot,btree_guide<T>::ORTT_SEARCH,cur->data) ;

    e.assign_task(t) ;
    p = e.cross_tree() ;

    if(!p)
        adopt_child(NULL,
                    static_cast<btree_node<T>*>(cur),
                    IAMROOT);
    else
        adopt_child(static_cast<btree_node<T>*>(p),
                    static_cast<btree_node<T>*>(cur),
                    ((*cur) < (*p)) ? ASLEFT : ASRIGHT) ;

    return ;
}

#endif
