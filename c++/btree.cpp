#include <iostream>

using std::cout ;
using std::endl ;

#if 1
template<class T>
struct node{
    inline node(T t) : data(t){} 
    inline ~node(){ cout << "delete " << data << endl ; }
    struct node *parent ;
    struct node *left ;
    struct node *right ;
    T      data ;
} ;

template<class T>
class btree{
    public:
        btree(): _root(NULL), _first(NULL), _last(NULL) { }
        virtual ~btree() ;

        #define has_child(node)  (node && (node->left || node->right))
        #define has_parent(node) (node && (node->parent))
        #define has_left(node)   (node && node->left)
        #define has_right(node)  (node && node->right)
        #define is_left(node)    (node \
                                  && node->parent \
                                  && node->parent->left \
                                  && node->parent->left == node)
        #define is_right(node)   (node \
                                  && node->parent \
                                  && node->parent->right \
                                  && node->parent->right == node)
        enum childid{ IAMROOT, ASLEFT, ASRIGHT } ;

        inline node<T>* get_rightmost(node<T> *t)
        { 
            while(t->right)
                t = t->right ;
            return t ;
        }

        inline node<T>* get_leftmost(node<T> *t)
        {
            while(t->left) 
                t = t->left ; 
            return t ;
        }

        bool has_relationship(node<T> *parent, node<T> *child) ;
        node<T>* lost_child(node<T> *parent, childid) ; //return the pool wanderer
        node<T>* leave_parent(node<T> *child) ; // return the worried parents
        void adopt_child(node<T> *parent, node<T> *child, childid) ;
        virtual void remove(node<T>* t) ;
        void insertroot(node<T> *root) ;
        node<T>* getroot(void) ;
        childid get_childid(node<T> *child) ;
        node<T>* get_next(node<T> *cur) ;

    private:
        node<T> *_root ;
        node<T> *_first ;
        node<T> *_last ;
} ;

template<class T>
btree<T>::~btree()
{
    node<T> *b = NULL ;
    node<T> *t = NULL ;

    // free allocate memory
    b = get_leftmost(_root) ;

    while(b)
    {
        t = b ;
        b = get_next(b) ;

        leave_parent(t) ;
        delete t ;
    }

    // no need reset members

    return ;
}

template<class T>
void btree<T>::adopt_child( node<T> *parent, node<T> *child, childid id)
{
    if(!parent || !child || child->parent == child) 
        throw -1 ;

    if(ASLEFT == id && parent->left == NULL)
        parent->left = child ;
    else if(ASRIGHT == id && parent->right == NULL)
        parent->right = child ;
    else
        throw -2 ;

    child->parent = parent ;

    return ;
}

template<class T>
bool btree<T>::has_relationship(node<T> *parent, node<T> *child)
{
    if(!parent || !child)
        throw -3 ;

    return ((child->parent == parent) && 
            (parent->left == child || parent->right == child)) ;
}

template<class T>
node<T>* btree<T>::get_next(node<T>* cur)
{
    if(!cur)
        return NULL ;

    if(has_right(cur)){
        cur = cur->right ;
        cur = get_leftmost(cur) ;
        return cur ;
    }else if(is_left(cur)){
        return cur->parent ;
    }else{
        while(is_right(cur))
            cur = cur->parent ;
        return cur->parent ;
    }

    return NULL;
}

template<class T>
typename btree<T>::childid btree<T>::get_childid(node<T>* node)
{
    if(!node)
        throw -4 ;

    if(!node->parent) 
        return IAMROOT ;
    
    return (node->parent->left == node) ? ASLEFT : ASRIGHT ;
}

template<class T>
node<T>* btree<T>::leave_parent(node<T> *child)
{
    node<T> *parent = NULL ;

    if(!has_parent(child))
        return NULL ;

    parent = child->parent ;
    lost_child(parent,get_childid(child)) ;
    child->parent = NULL ;

    return parent ;
}

template<class T>
node<T>* btree<T>::lost_child(node<T> *parent, childid id)
{
    node<T> *child = NULL ;

    if(!parent)
        return NULL ;

    switch(id){
    case ASLEFT:
        child = parent->left ;
        parent->left = NULL ;
        break ;
    case ASRIGHT:
        child = parent->right ;
        parent->right = NULL ;
        break ;
    default:
        throw -5 ;
        break ;
    }

    return child ;
}

template<class T>
node<T>* btree<T>::getroot(void)
{
    return _root ;
}

template<class T>
void btree<T>::insertroot(node<T> *root)
{
    if(_root || !root)
        throw -6 ;

    _root = root ;
    return ;
}

template<class T>
void btree<T>::remove(node<T> *cur)
{
    node<T> *wanderer = NULL ;

    if (!cur)
        return ;

    // cur node has no child
    if(!has_child(cur)){
        leave_parent(cur) ;
        delete cur ;
        return ;
    }

    // merge children into one branch
    if (cur->left && cur->right){
        wanderer = lost_child(cur,ASRIGHT) ;
        adopt_child(get_rightmost(cur->left),wanderer,ASRIGHT) ;
    }

    // now, there is only one branch. release cur node from tree
    wanderer = lost_child(cur, has_left(cur) ? ASLEFT : ASRIGHT) ;

    if(cur->parent)
        adopt_child(cur->parent, wanderer, get_childid(cur)) ;
    else
        _root = wanderer ;

    // all done, delete cur
    delete cur ;

    return ;
}
#endif

int main(void)
{
    try{
        node<int> *p = NULL ;
        node<int> *q = NULL ;

        btree<int>  bt ;
        p = new node<int>(7) ;
        bt.insertroot(p) ;
        q = new node<int>(8) ;
        bt.adopt_child(p,q,btree<int>::ASRIGHT) ;
        q = new node<int>(6) ;
        bt.adopt_child(p,q,btree<int>::ASLEFT) ;

    }catch(int &err){
        cout << err << endl ;
    }

    return 0 ;
}
