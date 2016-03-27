#include <iostream>

using std::cout ;
using std::endl ;

#if 1
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

        bool is_left_child(node<T>* t) const
        {
            return (t 
                    && t->parent 
                    && t->parent->left 
                    && t->parent->left == t) ;
        }

        bool is_right_child(node<T>* t) const
        {
            return (t 
                    && t->parent 
                    && t->parent->right 
                    && t->parent->right == t) ;
        }

        bool is_root(node<T>* t) const
        {
            return  (t && t == _root) ;
        }

        bool has_child(node<T>* t) const
        {
            return  (t && (t->left || t->right)) ;
        }

        bool has_parent(node<T>* t) const
        {
            return  (t && (t->parent)) ;
        }

        bool has_left_child(node<T>* t) const
        {
            return  (t && t->left) ;
        }

        bool has_right_child(node<T>* t) const
        {
            return  (t && t->right) ;
        }

        bool has_left_brother(node<T>* t) const
        {
            return  (t  
                     && t->parent 
                     && t->parent->left) ;
        }

        bool has_right_brother(node<T>* t) const
        {
            return (t 
                    && t->parent 
                    && t->parent->right) ;
        }

        node<T> * get_root(void) const
        {
            return  _root ;
        }

        node<T> * get_left_child(node<T> *t) const
        {
            return  (t ? t->left : NULL) ;
        }

        node<T> * get_right_child(node<T> *t) const
        {
            return  (t ? t->right : NULL) ;
        }

        node<T> * get_parent(node<T> *t) const
        {
            return  (t ? t->parent : NULL) ;
        }

        enum childid{ IAMROOT, ASLEFT, ASRIGHT } ;
        enum order{ PREORDER, INORDER, POSTORDER } ;

        inline node<T>* get_rightmost(node<T> *t)
        { 
            while(has_right_child(t))
                t = get_right_child(t) ;
            return t ;
        }

        inline node<T>* get_leftmost(node<T> *t)
        {
            while(has_left_child(t)) 
                t = get_left_child(t) ;
            return t ;
        }

        bool has_relationship(node<T> *parent, node<T> *child) ;
        node<T>* lost_child(node<T> *parent, childid) ; //return the pool wanderer
        node<T>* leave_parent(node<T> *child) ; // return the worried parents
        void adopt_child(node<T> *parent, node<T> *child, childid) ;
        void traversal(node<T>* tree_top, order);
        virtual void remove(node<T>* t) ;
        void insertroot(node<T> *root) ;
        void print(node<T> *t) const ;
        childid get_childid(node<T> *child) ;
        node<T>* get_next(node<T> *cur, order) ;

    private:
        node<T> *_root ;
        node<T> *_first ;
        node<T> *_last ;
} ;

template<class T>
void btree<T>::traversal(node<T> *t, order ot)
{
    node<T> *iterator = NULL ;

    if(!t){
        cout << "It is a empty tree" << endl ;
        return ;
    }

    PREORDER == ot ? iterator = t
                   : iterator = get_leftmost(t) ;

    while(iterator != get_parent(t)){
        print(iterator);
        iterator = get_next(iterator,ot) ;
    }

    return ;
}

template<class T>
btree<T>::~btree()
{
    node<T> *b = NULL ;
    node<T> *t = NULL ;

    // free allocate memory
    b = get_leftmost(_root) ;

    while(b)
    {
        t = b ; print(t) ;
        b = get_next(b,POSTORDER) ;
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
node<T>* btree<T>::get_next(node<T>* cur, btree<T>::order ot)
{
    if(!cur)
        return NULL ;

    switch(ot){
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
void btree<T>::insertroot(node<T> *root)
{
    if(_root || !root)
        throw -6 ;

    _root = root ;
    return ;
}

template<class T>
void btree<T>::print(node<T>* t) const
{
    if(!t){
        cout << "Do you want to print a 'NULL' node !!?" << endl ;
        return ;
    }

    cout << "--data=" << t->data << endl ;

    if(t->parent){
        is_left_child(t) ? cout << "  left of " << t->parent->data << endl
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
    wanderer = lost_child(cur, has_left_child(cur) ? ASLEFT : ASRIGHT) ;

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

        cout << "PREORDER" << endl ;
        bt.traversal(bt.get_root(),btree<int>::PREORDER) ;
        cout << "INORDER" << endl ;
        bt.traversal(bt.get_root(),btree<int>::INORDER) ;
        cout << "POSTORDER" << endl ;
        bt.traversal(bt.get_root(),btree<int>::POSTORDER) ;

    }catch(int &err){
        cout << err << endl ;
    }

    return 0 ;
}
