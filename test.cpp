#include <iostream>

using std::cout;
using std::endl;

namespace test1_1 {
    void func(int &)
    {
        cout << __PRETTY_FUNCTION__ << endl ;
        return ;
    }

    void func(const int &)
    {
        cout << __PRETTY_FUNCTION__ << endl ;
        return ;
    }
}

namespace test1_2 {
    void func(const int &)
    {
        cout << __PRETTY_FUNCTION__ << endl ;
        return ;
    }
}

namespace test1_3 {
    void func(int &)
    {
        cout << __PRETTY_FUNCTION__ << endl ;
        return ;
    }
}

int main(void)
{
    // test1 
    {
        int i = 3 ;
        const int j = 4 ;
        test1_1::func(j) ;  // ok
        test1_1::func(i) ;  // ok
        test1_2::func(j) ;  // ok
        test1_2::func(i) ;  // ok
        //test1_3::func(j) ;  // fail
        test1_3::func(i) ;  // ok

    }
    return 0 ;
}
