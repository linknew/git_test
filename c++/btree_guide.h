#ifndef __BTREE_GUID_H__
#define __BTREE_GUID_H__

#include <iostream>
#include "btree_node.h"
#include "btree.h"

using std::cout ;
using std::endl ;

template<class T>
class btree_guide : public guide<T>{

    public:
        enum ort_type{         // orient type
            ORTT_PREORDER,     // traversal with pre-order
            ORTT_INORDER,      // traversal with in-order
            ORTT_POSTORDER,    // traversal with post-order
            ORTT_SEARCH        // for searching a node
        } ;

        btree_guide() : guide<T>(ORTT_INORDER) { } 
        node<T>* get_start(const node<T>* ptree_top) const;
        node<T>* get_end(const node<T>* ptree_top) const;
        node<T>* get_next(const node<T>* pcur) const;

    protected:
} ;

template<class T>
node<T>*
btree_guide<T>::get_start(const node<T>* ptree_top)const
{
    const node<T> *ret = NULL ;
    const btree_node<T>* t = NULL ;
    const btree_node<T>* _t = NULL ;

    t = static_cast<const btree_node<T>*>(ptree_top) ;

    switch(this->_ort_type)
    {
        case ORTT_PREORDER:
            ret = ptree_top ;
            break ;
        case ORTT_INORDER:
            ret = btree<T>::get_leftmost(t) ;
            break ;
        case ORTT_POSTORDER:
            while((_t = btree<T>::get_left_child(t)) 
                  || (_t = btree<T>::get_right_child(t)))
                t = _t ;
            ret = t ;
            break ;
        case ORTT_SEARCH:
            ret = ptree_top ;
            break ;
        default:
            throw ERR_INV_ARG ;
    }

    return const_cast<node<T>*>(ret) ;
}

template<class T>
node<T>*
btree_guide<T>::get_end(const node<T>* ptree_top)const
{
    const node<T> *ret = NULL ;
    const btree_node<T>* _t = NULL ;
    const btree_node<T>* t = static_cast<const btree_node<T>*>(ptree_top) ;

    switch(this->_ort_type)
    {
        case ORTT_PREORDER:
            while((_t = btree<T>::get_right_child(t)) 
                  || (_t = btree<T>::get_left_child(t)))
                t = _t ;
            ret = t ;
            break ;
        case ORTT_INORDER:
            ret = btree<T>::get_rightmost(t) ;
            break ;
        case ORTT_POSTORDER:
            ret = ptree_top ;
            break ;
        case ORTT_SEARCH:
            ret = NULL ;
            break ;
        default:
            throw ERR_INV_ARG ;
    }

    return const_cast<node<T>*>(ret) ;
}

template<class T>
node<T>*
btree_guide<T>::get_next(const node<T>* pcur)const
{
    const btree_node<T>* cur = static_cast<const btree_node<T>*>(pcur) ;
    const btree_node<T>* t = NULL ;

    if(!cur)
        return NULL ;

    switch(this->_ort_type){
        case ORTT_PREORDER:
            if(btree<T>::has_left_child(cur))
                return btree<T>::get_left_child(cur) ;

            if(btree<T>::has_right_child(cur))
                return btree<T>::get_right_child(cur) ;

            while(btree<T>::is_right_child(cur))
                cur = btree<T>::get_parent(cur) ;

            if(btree<T>::has_parent(cur))
                return btree<T>::get_right_brother(cur) ;

            return NULL ;

        case ORTT_INORDER:
            if(btree<T>::has_right_child(cur)){
                return btree<T>::get_leftmost(btree<T>::get_right_child(cur)) ;
            }
            
            if(btree<T>::is_left_child(cur))
                return btree<T>::get_parent(cur) ;

            while(btree<T>::is_right_child(cur))
                cur = btree<T>::get_parent(cur) ;
            return btree<T>::get_parent(cur) ;

        case ORTT_POSTORDER:
            if(btree<T>::is_left_child(cur) 
                && btree<T>::has_right_brother(cur)){
                cur = btree<T>::get_right_brother(cur) ;
                while((t = btree<T>::get_left_child(cur))
                      || (t = btree<T>::get_right_child(cur)))
                    cur = t ;
                return const_cast<btree_node<T>*>(cur) ;
            }

            return btree<T>::get_parent(cur) ;

        case ORTT_SEARCH:
            if(this->_keyword < cur->data)
                return btree<T>::get_left_child(cur) ;

            if(this->_keyword > cur->data)
                return btree<T>::get_right_child(cur) ;

            return NULL ; // foud you!, break the searching

        default:
            throw ERR_INV_ARG ;
    }

    return NULL;
}

#endif
