#include <iomanip>
#include <iostream>
#include "btree_node.h"
#include "btree_guide.h"
#include "btree.h"

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
        
        explorer<int> e ;
        btree_guide<int> g ;
        print_task<int> tp(bt.get_root());
        searching_task<int> ts(bt.get_root(),btree_guide<int>::ORTT_SEARCH,12);

        e.assign_guide(g);
        e.assign_task(ts);

        cout << e.cross_tree()->data << endl ;


    }catch(int &err){
        cout << err << endl ;
    }catch(...){
        cout << "caught you!!" << endl ;
    } 

    return 0 ;
}
