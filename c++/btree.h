#ifndef __btree_h__
#define __btree_h__

#define ERR_INV_ARG (-1)
#define ERR_ADOPTION_FAIL   (-2)
#define UNKNOWN_TRAVERSAL_CMD (-3)

#include <iostream>
#include "btree_node.h"
#include "btree_guide.h"

using std::cout ;
using std::cerr ;
using std::endl ;
using std::setw ;

template<class T> class btree_node ;
template<class T> class btree_guide ;
template<class T> class btree_explorer ;

template<class T>
class task_print : public task<T>{
    public:
        task_print(node<T>* tree,
                   int ort = btree_guide<T>::ORTT_INORDER,
                   const T& kw = T(0)) 
            : task<T>(tree,ort,kw){}

        int do_the_job(node<T>* node)
        {
            btree_node<T>* t = static_cast<btree_node<T>*>(node) ;

            if(!t){
                cout << "Do you want to print a 'NULL' node !!?" << endl ;
                return task<T>::TTS_OK ;
            }
            cout << "--" << setw(4) << t->data << ": " ;
            
            (t->pparent) ? cout << "P(" << setw(4) << t->pparent->data << ") " 
                        : cout << "P(   *) " ;

            (t->pleft) ? cout << "L(" << setw(4) << t->pleft->data << ") " 
                        : cout << "L(   *) " ;

            (t->pright) ? cout << "R(" << setw(4) << t->pright->data << ") " 
                        : cout << "R(   *) " ;

            cout << endl ;
            this->task<T>::_state = task<T>::TTS_OK ;
            return this->task<T>::_state ;
        }
} ;

template<class T>
class task_searching : public task<T>{
    public:
        task_searching(node<T>* tree,
                   int ort = btree_guide<T>::ORTT_SEARCH,
                   const T& kw = T(0)) 
            : task<T>(tree,ort,kw){}

        int do_the_job(node<T>* node)
        {
            /* do nothing~~~ only travel~~ happy */
            this->task<T>::_state = task<T>::TTS_OK ;
            return this->task<T>::_state ;
        }
} ;

template<class T>
class task_destruction : public task<T>{
    public:
        task_destruction(node<T>* tree,
                         int ort = btree_guide<T>::ORTT_POSTORDER,
                         const T &kw = T(0))
            : task<T>(tree,ort,kw){}

        int do_the_job(node<T>* node)
        {
            btree_node<T>* t = static_cast<btree_node<T>*>(node) ;

            if(!t){
                return task<T>::TTS_NOT_START ;
            }

            if(btree<T>::has_child(t))
                throw_err(-1) ;

            if(btree<T>::has_parent(t))
                btree<T>::is_left_child(t) ? btree<T>::get_parent(t)->pleft = NULL
                                           : btree<T>::get_parent(t)->pright = NULL;

            t->pparent = NULL ;

            delete t ;
            this->task<T>::_state = task<T>::TTS_OK ;
            return this->task<T>::_state ;
        }
} ;

template<class T>
class btree{

    public:

        btree(): _proot(NULL)
        { 
#if _DBG
            cerr << "construction btree:" << this << endl;
#endif
        }
        virtual ~btree() ;

        enum childid{ IAMROOT, ASLEFT, ASRIGHT } ;

        static inline bool is_left_child(const btree_node<T> *t) ;
        static inline bool is_right_child(const btree_node<T> *t) ;
        static inline bool has_parent(const btree_node<T> *t) ;
        static inline bool has_child(const btree_node<T> *t) ;
        static inline bool has_left_child(const btree_node<T> *t) ;
        static inline bool has_right_child(const btree_node<T> *t) ;
        static inline bool has_brother(const btree_node<T> *t) ;
        static inline bool has_left_brother(const btree_node<T> *t) ;
        static inline bool has_right_brother(const btree_node<T> *t) ;
        static inline btree_node<T>* get_parent(const btree_node<T> *t) ;
        static inline btree_node<T>* get_left_child(const btree_node<T> *t) ;
        static inline btree_node<T>* get_right_child(const btree_node<T> *t) ;
        static inline btree_node<T>* get_left_brother(const btree_node<T> *t) ;
        static inline btree_node<T>* get_right_brother(const btree_node<T> *t) ;
        static inline btree_node<T>* get_leftmost(const btree_node<T> *t) ;
        static inline btree_node<T>* get_rightmost(const btree_node<T> *t) ;
        static inline childid get_childid(const btree_node<T> *child) ;

        // these 2 functions access non-static member (_proot)
        // cannot defined with 'static'
        inline bool is_root(const btree_node<T> *t) const ;
        inline btree_node<T>* get_root(void) const ;

        void adopt_child(btree_node<T> *parent, btree_node<T> *child, childid id) ;

        //return the pool wanderer
        btree_node<T>* lost_child(btree_node<T> *parent, childid id) ; 

        // return the worried parents
        btree_node<T>* leave_parent(btree_node<T> *child) ; 

        virtual void remove(btree_node<T> *t) ;
        virtual void insert(btree_node<T> *t) ;

    protected:
        btree_node<T>       *_proot ;
} ;

template<class T>
bool inline btree<T>::is_left_child(const btree_node<T> *t)
{
    return (t 
            && t->pparent 
            && t->pparent->pleft 
            && t->pparent->pleft == t) ;
}

template<class T>
inline bool btree<T>::is_right_child(const btree_node<T> *t)
{
    return (t 
            && t->pparent 
            && t->pparent->pright 
            && t->pparent->pright == t) ;
}

template<class T>
inline bool btree<T>::is_root(const btree_node<T> *t) const
{
    return  (t && t == _proot) ;
}

template<class T>
inline bool btree<T>::has_child(const btree_node<T> *t)
{
    return  (t && (t->pleft || t->pright)) ;
}

template<class T>
inline bool btree<T>::has_parent(const btree_node<T> *t)
{
    return  (t && (t->pparent)) ;
}

template<class T>
inline bool btree<T>::has_left_child(const btree_node<T> *t)
{
    return  (t && t->pleft) ;
}

template<class T>
inline bool btree<T>::has_right_child(const btree_node<T> *t)
{
    return  (t && t->pright) ;
}

template<class T>
inline bool btree<T>::has_brother(const btree_node<T> *t)
{
    return  (t  
             && t->pparent 
             && t->pparent->pleft
             && t->pparent->pright) ;
}

template<class T>
inline bool btree<T>::has_left_brother(const btree_node<T> *t)
{
    return  (t  
             && t->pparent 
             && t->pparent->pleft) ;
}

template<class T>
inline bool btree<T>::has_right_brother(const btree_node<T> *t)
{
    return (t 
            && t->pparent 
            && t->pparent->pright) ;
}

template<class T>
inline btree_node<T>* btree<T>::get_rightmost(const btree_node<T> *t)
{ 
    while(has_right_child(t))
        t = get_right_child(t) ;
    return const_cast<btree_node<T>*>(t) ;
}

template<class T>
inline btree_node<T>* btree<T>::get_leftmost(const btree_node<T> *t)
{
    while(has_left_child(t)) 
        t = get_left_child(t) ;
    return const_cast<btree_node<T>*>(t) ;
}

template<class T>
inline btree_node<T>* btree<T>::get_root(void) const
{
    return  _proot ;
}

template<class T>
inline btree_node<T>* btree<T>::get_left_child(const btree_node<T> *t)
{
    return  (t ? t->pleft : NULL) ;
}

template<class T>
inline btree_node<T>* btree<T>::get_right_child(const btree_node<T> *t)
{
    return  (t ? t->pright : NULL) ;
}

template<class T>
inline btree_node<T>* btree<T>::get_left_brother(const btree_node<T> *t)
{
    if(is_left_child(t))
        throw_err(-1) ;

    return (has_parent(t) ? get_left_child(get_parent(t)) : NULL) ;
}

template<class T>
inline btree_node<T>* btree<T>::get_right_brother(const btree_node<T> *t)
{
    if(is_right_child(t))
        throw_err(-1) ;

    return (has_parent(t) ? get_right_child(get_parent(t)) : NULL) ;
}


template<class T>
inline btree_node<T>* btree<T>::get_parent(const btree_node<T> *t)
{
    return  (t ? t->pparent : NULL) ;
}

template<class T>
btree<T>::~btree()
{
    btree_explorer<T> e ;
    task_destruction<T> t(_proot);

#if _DBG
    cerr << "destruction btree:" << this << endl;
#endif
    e.assign_task(t) ;
    e.cross_tree() ;

    return ;
}

template<class T>
void btree<T>::adopt_child( btree_node<T> *parent, btree_node<T> *child, childid id)
{
    if(!child || (id != ASLEFT && id != ASRIGHT && id != IAMROOT))
        throw_err(ERR_INV_ARG) ;

    if(has_parent(child))
        throw_err(ERR_ADOPTION_FAIL) ;

    /* is first btree_node<T>, set as root */
    if(!_proot && !parent && IAMROOT == id){
        child->pleft = NULL ;
        child->pright = NULL ;
        child->pparent = NULL ;
        _proot = child ;
        return ;
    }
        
    if(ASLEFT == id && !has_left_child(parent))
        parent->pleft = child ;
    else if(ASRIGHT == id && !has_right_child(parent))
        parent->pright = child ;
    else
        throw_err(ERR_ADOPTION_FAIL) ;

    child->pparent = parent ;

    return ;
}

template<class T>
typename btree<T>::childid btree<T>::get_childid(const btree_node<T> *t)
{
    if(!t)
        throw_err(-1) ;

    if(!has_parent(t)) 
        return IAMROOT ;
    
    return (is_left_child(t)) ? ASLEFT : ASRIGHT ;
}

template<class T>
btree_node<T>* btree<T>::leave_parent(btree_node<T> *child)
{
    btree_node<T> *parent = NULL ;

    if(!has_parent(child))
        return NULL ;

    parent = get_parent(child) ;
    lost_child(parent,get_childid(child)) ;

    return parent ;
}

template<class T>
btree_node<T>* btree<T>::lost_child(btree_node<T> *parent, childid id)
{
    btree_node<T> *child = NULL ;

    if(!parent)
        return NULL ;

    switch(id){
    case ASLEFT:
        child = get_left_child(parent) ;
        parent->pleft = NULL ;
        break ;
    case ASRIGHT:
        child = get_right_child(parent) ;
        parent->pright = NULL ;
        break ;
    default:
        throw_err(-1) ;
        break ;
    }

    if(child)
        child->pparent = NULL ;

    return child ;
}

template<class T>
void btree<T>::remove(btree_node<T> *cur)
{
    btree_node<T> *wanderer = NULL ;

    if (!cur)
        return ;

    // cur btree_node<T> has no child
    if(!has_child(cur)){
        if(NULL == leave_parent(cur))   // no parent
            _proot = NULL ;
        delete cur ;
        return ;
    }

    // merge children into one branch
    if (has_left_child(cur) && has_right_child(cur)){
        wanderer = lost_child(cur,ASRIGHT) ;
        adopt_child(get_rightmost(get_left_child(cur)),wanderer,ASRIGHT) ;
    }

    // now, there is only one branch. release cur btree_node<T> from tree
    wanderer = lost_child(cur, has_left_child(cur) ? ASLEFT : ASRIGHT) ;

    if(has_parent(cur))
        adopt_child(get_parent(cur), wanderer, get_childid(cur)) ;
    else
        _proot = wanderer ;

    // all done, delete cur
    delete cur ;

    return ;
}

template<class T>
void btree<T>::insert(btree_node<T>* cur)
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
