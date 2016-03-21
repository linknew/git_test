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

// test1,about const reference
void test1(void)
{
    int i = 3 ;
    const int j = 4 ;
    test1_1::func(j) ;  // ok
    test1_1::func(i) ;  // ok
    test1_2::func(j) ;  // ok
    test1_2::func(i) ;  // ok
    //test1_3::func(j) ;  // fail, can use const_cast to convert. test1_3::func(const_cast<int&>(j)) 
    test1_3::func(i) ;  // ok

    return ;
}

//test2, about function pointer
void test2(void)
{
    void func_1(int,int) ;      // declare a function
    void (*prt_1)(int,int) ;    // declare a pointer, point to a function with 2 arguments (int, int)
    typedef void (*q)(int,int) ;
    q func_2 (int,int) ;        // declare a function, has 2 arguments, and it return a function_pointer
    q (*prt_2)(int,int);        // declare a pointer, point to a function which with 2 arguments and its return type is a function_pointer

    return ;
}

int main(void)
{
    //test1();
    test2();

    return 0 ;
}
