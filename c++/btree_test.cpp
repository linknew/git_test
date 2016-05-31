#include <iomanip>
#include <iostream>
#include "btree.h"
#include "btree_node.h"
#include "btree_explorer.h"

#include "rbtree_node.h"

using std::string ;

int main(void)
{
    try{
        btree<int> bt;
        btree_node<int>* p = NULL ;

        bt.insert(new btree_node<int>(7)) ;
        bt.insert(new btree_node<int>(12)) ;
        bt.insert(new btree_node<int>(4)) ;
        bt.insert(new btree_node<int>(15)) ;
        bt.insert(new btree_node<int>(9)) ;
        bt.insert(new btree_node<int>(6)) ;
        bt.insert(new btree_node<int>(32)) ;
        bt.insert(new btree_node<int>(17)) ;

        btree_explorer<int> e ;
        task_print<int> tp(bt.get_root());
        task_searching<int> ts(bt.get_root(),btree_guide<int>::ORTT_SEARCH);

        e.assign_task(tp) ;
        e.cross_tree() ;

        e.assign_task(ts) ;

        ts.set_tree(bt.get_root()) ;
        ts.set_keyword(7) ;
        p = e.cross_tree() ;
        cout << p->data << endl ;
        bt.remove(p) ;

        tp.set_tree(bt.get_root()) ;
        tp.set_ort(btree_guide<int>::ORTT_PREORDER) ;
        e.assign_task(tp);
        e.cross_tree();

    }catch(int &err){
        cout << "caught an error!! [" << err << "]" << endl ;
    }catch(...){
        cout << "caught an unknown error!!" << endl ;
    } 

    return 0 ;
}
