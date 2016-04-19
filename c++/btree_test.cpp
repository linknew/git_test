#include <iomanip>
#include <iostream>
#include "btree.h"
#include "btree_node.h"
#include "btree_explorer.h"

using std::string ;

int main(void)
{
    try{
        btree<int> bt;
        node<int>* p = NULL ;

        bt.insert(new btree_node<int>(7)) ;
        bt.insert(new btree_node<int>(4)) ;
        bt.insert(new btree_node<int>(15)) ;
        bt.insert(new btree_node<int>(9)) ;
        bt.insert(new btree_node<int>(6)) ;
        bt.insert(new btree_node<int>(32)) ;
        bt.insert(new btree_node<int>(17)) ;
        
        btree_explorer<int> e ;
        task_searching<int> ts(bt.get_root(),btree_guide<int>::ORTT_SEARCH,12);

        e.assign_task(ts);
        cout << e.cross_tree()->data << endl ;

        task_print<int> tp(bt.get_root(),btree_guide<int>::ORTT_PREORDER);
        e.assign_task(tp);
        e.cross_tree() ;

    }catch(int &err){
        cout << err << endl ;
    }catch(...){
        cout << "caught you!!" << endl ;
    } 

    return 0 ;
}
