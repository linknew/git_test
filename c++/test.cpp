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

namespace test5_1_space{
    template<class T> T min(T,T) ;  // ok, declare a template
    template<class U> U min(U,U) ;  // ok, same with upline

    
    template<class V>               // define the declared template
    V min(V x, V y)
    {
        return x ;
    }

    template<class T1, class T2, class T3>
    T1 func(T2 x, T3 y)
    {
        T1  ret = static_cast<T1>(0);
        return ret ;
    }
}

namespace test5_2_space{

    template<class T>
    inline void func(T t)   // if want use keyword "inline" or "extern",
                            // the key words must be placed after 
                            //  template arguments
    {
        int k = 0 ;
        typename T::name i ; // use "typename" to indicate T::name is a TYPE
    }
}

//test5, about template
void test5(void)
{
    using namespace test5_1_space ;
    using namespace test5_2_space ;

    cout << min(3,4) << endl ;  //implicitly specify
                                //use actual argument to 
                                //dedue the template argument tpye
                                //this is called 
                                //"template argument deduction"

    cout << min<int>(3,4) << endl ; // explicitly specify

    cout << func<int,char,char>('a','b') << endl ; // explicitly specify
                            // cannot deduce the return type automatically

    cout << func<int>('a','b') << endl ;    // explicitly specify
                            // only return type cannot be deduced
                            // so, we can ellipsis the others.
    return ;
}

namespace test6_space{
    class excpt_a{
        public:
            void err_info(int line_num)
            {
                cout << "catch excpt_a at line: " << line_num << endl;
                return ;
            }
        private:
    } ;
}

//test6, about exception
void test6(void)
{
    using namespace test6_space;
    
    // throw and catch
    // continue to execute statement following by 'catch' if 
    // catched successful
    {
        try{
            throw excpt_a();
            return ;
        }catch(excpt_a &err){
            err.err_info(__LINE__);
        }

        cout << __LINE__ << "  continue" << endl ;
    }

    // thow -> no catch
    // return to caller directly if failing on catch
    {
        try{
            try{
                throw excpt_a() ;
            }catch(int &err){
            }
            cout << __LINE__ << "  continue" << endl ;  // skip this
        }catch(excpt_a &err){
            err.err_info(__LINE__);
        }
        cout << __LINE__ << "  continue" << endl ;
    }

    // thow -> catched -> rethow
    {
        try{
            try{
                cout << "thow a excpt here" << endl ;
                throw excpt_a() ;
            }catch(excpt_a &err){
                err.err_info(__LINE__);
                cout << "rethow it again" << endl;
                throw ;
            }
            cout << "this line will not be printed" << endl ;
        }catch(excpt_a &err){
            err.err_info(__LINE__);
        }
        cout << __LINE__ << "  continue" << endl ;
    }
}

int main(void)
{
    //test1();
    //test2();
    //test3() ;
    //test4();
    //test5();
    test6();

    return 0 ;
}
