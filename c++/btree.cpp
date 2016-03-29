#include <iostream>

using std::cout ;
using std::endl ;

template<class T>
struct node{
    inline node(T t) : data(t),parent(NULL),left(NULL),right(NULL){} 
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

#define ERR_INV_ARG  (-1)
#define ERR_ADOPTION_FAIL   (-2)

        inline bool is_left_child(node<T>* t) const ;
        inline bool is_right_child(node<T>* t) const ;
        inline bool is_root(node<T>* t) const ;
        inline bool has_child(node<T>* t) const ;
        inline bool has_parent(node<T>* t) const ;
        inline bool has_left_child(node<T>* t) const ;
        inline bool has_right_child(node<T>* t) const ;
        inline bool has_brother(node<T>* t) const ;
        inline bool has_left_brother(node<T>* t) const ;
        inline bool has_right_brother(node<T>* t) const ;
        inline node<T> * get_root(void) const ;
        inline node<T> * get_parent(node<T> *t) const ;
        inline node<T> * get_left_child(node<T> *t) const ;
        inline node<T> * get_right_child(node<T> *t) const ;
        inline node<T> * get_left_brother(node<T> *t) const ;
        inline node<T> * get_right_brother(node<T> *t) const ;
        inline node<T> * get_rightmost(node<T> *t) ;
        inline node<T> * get_leftmost(node<T> *t) ;
        enum childid{ IAMROOT, ASLEFT, ASRIGHT } ;
        enum traversal_type{ PREORDER, INORDER, POSTORDER } ;

        node<T>* lost_child(node<T> *parent, childid id) ; //return the pool wanderer
        node<T>* leave_parent(node<T> *child) ; // return the worried parents
        void adopt_child(node<T> *parent, node<T> *child, childid id) ;
        void traversal(node<T>* tree_top, traversal_type tt = INORDER, void (*pf)(const node<T>*) = print) ;
        virtual void remove(node<T>* t) ;
        static void print(const node<T> *t) ;
        childid get_childid(node<T> *child) ;
        node<T>* get_first(node<T> *tree_top, traversal_type tt) ;
        node<T>* get_last(node<T> *tree_top, traversal_type tt) ;
        node<T>* get_next(node<T> *cur, traversal_type tt) ;

    private:
        node<T> *_root ;
        node<T> *_first ;
        node<T> *_last ;
} ;

template<class T>
inline bool btree<T>::is_left_child(node<T>* t) const
{
    return (t 
            && t->parent 
            && t->parent->left 
            && t->parent->left == t) ;
}

template<class T>
inline bool btree<T>::is_right_child(node<T>* t) const
{
    return (t 
            && t->parent 
            && t->parent->right 
            && t->parent->right == t) ;
}

template<class T>
inline bool btree<T>::is_root(node<T>* t) const
{
    return  (t && t == _root) ;
}

template<class T>
inline bool btree<T>::has_child(node<T>* t) const
{
    return  (t && (t->left || t->right)) ;
}

template<class T>
inline bool btree<T>::has_parent(node<T>* t) const
{
    return  (t && (t->parent)) ;
}

template<class T>
inline bool btree<T>::has_left_child(node<T>* t) const
{
    return  (t && t->left) ;
}

template<class T>
inline bool btree<T>::has_right_child(node<T>* t) const
{
    return  (t && t->right) ;
}

template<class T>
inline bool btree<T>::has_brother(node<T>* t) const
{
    return  (t  
             && t->parent 
             && t->parent->left
             && t->parent->right) ;
}

template<class T>
inline bool btree<T>::has_left_brother(node<T>* t) const
{
    return  (t  
             && t->parent 
             && t->parent->left) ;
}

template<class T>
inline bool btree<T>::has_right_brother(node<T>* t) const
{
    return (t 
            && t->parent 
            && t->parent->right) ;
}

template<class T>
inline node<T>* btree<T>::get_rightmost(node<T> *t)
{ 
    while(has_right_child(t))
        t = get_right_child(t) ;
    return t ;
}

template<class T>
inline node<T>* btree<T>::get_leftmost(node<T> *t)
{
    while(has_left_child(t)) 
        t = get_left_child(t) ;
    return t ;
}

template<class T>
inline node<T> * btree<T>::get_root(void) const
{
    return  _root ;
}

template<class T>
inline node<T> * btree<T>::get_left_child(node<T> *t) const
{
    return  (t ? t->left : NULL) ;
}

template<class T>
inline node<T> * btree<T>::get_right_child(node<T> *t) const
{
    return  (t ? t->right : NULL) ;
}

template<class T>
inline node<T> * btree<T>::get_parent(node<T> *t) const
{
    return  (t ? t->parent : NULL) ;
}

template<class T>
void btree<T>::traversal(node<T> *tree_top, traversal_type tt, void (*pf)(const node<T>*))
{
    node<T> *iterator = NULL ;
    node<T> *start = NULL ;
    node<T> *end = NULL ;

    if(!tree_top){
        cout << "It is a empty tree" << endl ;
        return ;
    }

    start = get_first(tree_top,tt) ;
    end = get_last(tree_top,tt) ;
    iterator = start ;

    while(iterator != end){
        if(pf)
            pf(iterator);
        iterator = get_next(iterator,tt) ;
    }

    if(pf)
        pf(end) ;
    return ;
}

template<class T>
btree<T>::~btree()
{
    node<T> *d = NULL ;
    node<T> *n = NULL ;

    d = get_first(_root,POSTORDER) ;
    n = get_next(d,POSTORDER) ;

    while(d){
        remove(d) ;
        d = n ;
        n = get_next(n,POSTORDER) ;
    }

    return ;
}

template<class T>
void btree<T>::adopt_child( node<T> *parent, node<T> *child, childid id)
{
    if(!child || (id != ASLEFT && id != ASRIGHT && id != IAMROOT))
        throw ERR_INV_ARG ;

    if(child->parent)
        throw ERR_ADOPTION_FAIL ;

    if(!_root && !parent && IAMROOT == id){
        child->left = NULL ;
        child->right = NULL ;
        child->parent = NULL ;
        _root = child ;
        return ;
    }
        
    if(ASLEFT == id && parent->left == NULL)
        parent->left = child ;
    else if(ASRIGHT == id && parent->right == NULL)
        parent->right = child ;
    else
        throw ERR_ADOPTION_FAIL ;

    child->parent = parent ;

    return ;
}

template<class T>
node<T>* btree<T>::get_first(node<T>* tree_top, traversal_type tt)
{
    node<T> *ret = tree_top ;
    node<T> *t = NULL ;

    switch(tt)
    {
        case PREORDER:
            ret = tree_top ;
            break ;
        case INORDER:
            ret = get_leftmost(tree_top) ;
            break ;
        case POSTORDER:
            while((t = get_left_child(ret)) || (t = get_right_child(ret)))
                ret = t ;
            break ;
        default:
            throw ERR_INV_ARG ;
    }

    return ret ;
}

template<class T>
node<T>* btree<T>::get_last(node<T> *tree_top, traversal_type tt)
{
    node<T> *ret = tree_top ;
    node<T> *t = NULL ;

    switch(tt)
    {
        case PREORDER:
            while((t = get_right_child(ret)) || (t = get_left_child(ret)))
                ret = t ;
            break ;
        case INORDER:
            ret = get_rightmost(tree_top) ;
            break ;
        case POSTORDER:
            ret = tree_top ;
            break ;
        default:
            throw ERR_INV_ARG ;
    }

    return ret ;
}

template<class T>
node<T>* btree<T>::get_next(node<T>* cur, traversal_type tt)
{
    if(!cur)
        return NULL ;

    switch(tt){
        case PREORDER:
            if(has_left_child(cur))
                return cur->left ;

            if(has_right_child(cur))
                return cur->right ;

            while(is_right_child(cur))
                cur = cur->parent ;

            if(cur->parent)
                return cur->parent->right ;

            return NULL ;

        case INORDER:
            if(has_right_child(cur)){
                cur = cur->right ;
                cur = get_leftmost(cur) ;
                return cur ;
            }
            
            if(is_left_child(cur))
                return cur->parent ;

            while(is_right_child(cur))
                cur = cur->parent ;
            return cur->parent ;

        case POSTORDER:
            if(is_root(cur))
                return NULL ;

            if(is_right_child(cur))
                return cur->parent ;

            if(!has_right_brother(cur))
                return cur->parent ;

            cur = cur->parent->right ;
            cur = get_leftmost(cur) ;
            return cur ;

        default:
            throw -1 ;
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

    if(child)
        child->parent = NULL ;

    return child ;
}

template<class T>
void btree<T>::print(const node<T>* t)
{
    if(!t){
        cout << "Do you want to print a 'NULL' node !!?" << endl ;
        return ;
    }

    cout << "--data=" << t->data << endl ;

    if(t->parent){
        (t->parent->left && t->parent->left == t) ? cout << "  left of " << t->parent->data << endl
                                                  : cout << "  right of " << t->parent->data << endl ;
    }else{
        cout << "  parent = NULL" << endl ;
    }

    cout << "  left_child = " ;
    if(t->left)
        cout << t->left->data << endl ;
    else
        cout << "NULL" << endl ;

    cout << "  right_child = " ;
    if(t->right)
        cout << t->right->data << endl ;
    else
        cout << "NULL" << endl ;

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
        if(NULL == leave_parent(cur))   // no parent
            _root = NULL ;
        delete cur ;
        return ;
    }

    // merge children into one branch
    if (cur->left && cur->right){
        wanderer = lost_child(cur,ASRIGHT) ;
        adopt_child(get_rightmost(cur->left),wanderer,ASRIGHT) ;
    }

    // now, there is only one branch. release cur node from tree
    wanderer = lost_child(cur, has_left_child(cur) ? ASLEFT : ASRIGHT) ;

    if(cur->parent)
        adopt_child(cur->parent, wanderer, get_childid(cur)) ;
    else
        _root = wanderer ;

    // all done, delete cur
    delete cur ;

    return ;
}

void func(const node<int>* p)
{
    cout << p->data << "," ;
    return ;
}

int main(void)
{
    using ::btree ;

    try{
        node<int> *p = NULL ;
        node<int> *q = NULL ;

        btree<int>  bt ;
        p = new node<int>(7) ;
        bt.adopt_child(NULL,p,btree<int>::IAMROOT) ;
        q = new node<int>(8) ;
        bt.adopt_child(p,q,btree<int>::ASRIGHT) ;
        q = new node<int>(6) ;
        bt.adopt_child(p,q,btree<int>::ASLEFT) ;

        cout << "\n**PREORDER" << endl ;
        bt.traversal(bt.get_root(),btree<int>::PREORDER,func) ;

        cout << "\n**INORDER" << endl ;
        bt.traversal(bt.get_root(),btree<int>::INORDER,func) ;

        cout << "\n**POSTORDER" << endl ;
        bt.traversal(bt.get_root(),btree<int>::POSTORDER,func) ;

        bt.remove(bt.get_root()) ;
        cout << "\n---------------" << endl ;
        bt.traversal(bt.get_root(),btree<int>::POSTORDER,func) ;
        cout << endl << "==============="  << endl ;
        
    }catch(int &err){
        cout << err << endl ;
    }

    return 0 ;
}
