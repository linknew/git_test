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
    test1_1::func(j) ;      // ok
    test1_1::func(i) ;      // ok
    test1_2::func(j) ;      // ok
    test1_2::func(i) ;      // ok
    //test1_3::func(j) ;    // fail, can use const_cast to convert. 
                            // test1_3::func(const_cast<int&>(j)) 
    test1_3::func(i) ;      // ok

    return ;
}

//test2, about function pointer
void test2(void)
{
    void func_1(int,int) ;      // declare a function
    void (*prt_1)(int,int) ;    // declare a pointer, point to a function 
                                // with 2 arguments (int, int)
    typedef void (*q)(int,int) ;
    q func_2 (int,int) ;        // declare a function, has 2 arguments and 
                                // return a function_pointer
    q (*prt_2)(int,int);        // declare a pointer, point to a function 
                                // which with 2 arguments and 
                                // the return type is a function_pointer
    return ;
}

namespace test3_1_space{
    void func(int){ return ; }
    void func(char*){ return ; }
}

//test3, about overload
void test3(void)
{
    // overload should be in same domain
    {
        extern void func(int,int);
        extern void func(char,double*);

        {
            double* a ;
            extern void func(int) ;

            // func(3) ;        // ok
            // func(1,2) ;      // error, func(int,int) was hide
            // func('a',&a) ;   // error, func(char,double*) was hide
        }
    }

    // overload example
    {
        extern void func(int) ;
        extern void func(int,int) ;
        //extern void func(int,int=3) ; // ambiguous with func(int)
    }

    // another example
    {
        extern void func(int,int);
        using test3_1_space::func;

        char* s;
        func(1) ;       // ok, test3_1_space::func(int)
        func(s) ;       // ok, test3_1_space::func(char*)
        //func(1,2) ;   // ok, func(int,int)
    }

    // more example
    {
        using test3_1_space::func;
        //extern void func(int) ; // conflict with test3_1_space::func(int)
    }

    // extern "c" vs overload
    {
        extern void func(int) ;
        extern void func(int,int);  // ok, overload first one
        //extern "C" void func(int,int,int) ; // ok, overload again
        //extern "C" void func(int,int,int,int) ; // NOT OK
                            // c++ will expand the overload functions
                            // the function name will be renamed.
                            // but will not expand "C" style function name 
    }

    // function pointer vs overload
    {
        extern void func(char*);
        extern void func(unsigned int);
        //void (*pf1)(unsigned int) = func ; // ok
        //void (*pf2)(int) = func ; // fail, argument not match
        //int  (*pf3)(unsigned int) = func ;  // fail, return_type not match
        //void (*pf4)(char) = func ; // is it ok ???
    }

    return ;
}

//test4, about macro
void test4(void)
{
    // a trap!!! for macro
#define max(a,b)    (a) > (b) ? (a) : (b)

    using std::cout;
    using std::endl;
    
    int x=5;
    int y=4;
    int z=0;

    z=max(x++,y) ;  // x++ will be executed twice once expand
                    // template can fix this issue

    cout << x << endl ;

    return ;
}



int main(void)
{
    //test1();
    //test2();
    //test3() ;
    test4();

    return 0 ;
}
