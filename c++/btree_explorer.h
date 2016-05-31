#ifndef __BTREE_EXPLORER_H__
#define __BTREE_EXPLORER_H__

#include "tree.h"
#include "btree_guide.h"

template<class T> class btree_node ;
template<class T> class btree_guide ;
template<class T> class btree ;

template<class T>
class btree_explorer : public explorer<T>{

    public:
        btree_explorer() 
            : explorer<T>(*(new btree_guide<T>())) { }

        ~btree_explorer(){ delete this->_guide; }
    
        inline btree_node<T>* get_last_step(void)
        {
            return static_cast<btree_node<T>*>(explorer<T>::get_last_step()) ;
        }

        inline btree_node<T>* cross_tree(void)
        {
            return static_cast<btree_node<T>*>(explorer<T>::cross_tree()) ;
        }
    protected:
} ;

#endif
