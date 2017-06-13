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
                            // ATTENTION PLEASE, 
                            // extern "C" cannot used in function inside.
                            // here we just assume, it is not in a function.
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
void test6(void) throw(int,test6_space::excpt_a)
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
            throw ;
        }
        cout << __LINE__ << "  continue" << endl ;
    }
}

namespace test7_1_space{
    class a{
        public:
        a() {} ;                    // ok
        //a() const ;               // fail. not allow const construction
        //a(int=3,int=2,int=1){} ;  // fail. ambiguous with a()
    } ;

    class b{
        public:
            b(int i):_i(i),_j(i*2) {}  // use initial list to initialize members
        private:
            int _i ;
            int _j ;
    };

    class c{
        public:
            c(int i):_i(i),_j(_i){}     // must use initial list to initial const or reference member
                                        // because const or reference must be initialized when defined.
        private:
            const int  _i ;
            const int& _j ;
    };

    class d{
        public:
            //d(int i):_k(i),_j(_k+1),_i(_j+1) {} // there 2 warnings!!!
                                                // the order of initial list is not 
                                                // the true order the member initialsed.
                                                // normally, the initialize order is depend on the order of members
            d(int i):_k(i),_j(i+2),_i(i+3) {}   // ok.
                                                
        private:
            int _i ;
            int _j ;
            int _k ;
    };
}

// test7, about construction function
void test7(void)
{
    using namespace test7_1_space;

    a oa;
    //a ob(oa);
    return ;
}

namespace test8_space{

    class a ;                                       // declare a class

    class A{
        //int i = 1 ;                               // error, cannot initialed in class type. (no object)
        const static int k = 3 ;                    // ok, (no need object)
    };

    // for describing inline function
    class a{
        public:
            int func_1(void){ return 0; }           // ok, defined inside in a class, is a inline function
            inline int func_2(void) {return 0; }    // ok, explicitly define a inline function
            inline int func_3(void) ;               // declare a inline function.
    };

    int a::func_3(void)                             // define the inline function, the "inline" keyword is ellipsised
    {
        return 0 ;
    }

    // for friend
    class b{
        friend void func_friend(void) ;             // the tags public/protected/private will not effect friend declare.
        public:
        protected:
        private:
            void _func_prv(void){ cout << "hello, i am a private function in class b" << endl ;}
    } ;

    void func_friend(void)
    {
        b ob;
        
        cout << "i am func_friend, i will invoke class_b's private function" << endl;
        ob._func_prv() ;
        return ;
    }

    // for const object
    class c{
        public:
            c(){;}
            //c() const {;}                             // const construction is not allowed

            void func(void) const                       // const function
            { 
                //_i = 3 ;                              // will change object data, is not allowed in const function
                //*_p = 3 ;                             // it's ok as this operation does not change the object data.
                                                        // the value of "_p" is not change.
                                                        // there is BUG !!
                                                        // if the construction like this:  c() : _p(&_i) {;}
                                                        // then *_p = 3 will change the value of _i.
                cout << __PRETTY_FUNCTION__ << endl  ;
            }

            void func(void)                             // non-const function
            { 
                cout << __PRETTY_FUNCTION__ << endl; 
            }

        private:
            int _i ;
            int* _p ;
    };

}

// test8, about class
void test8(void)
{
    using namespace test8_space ;

    func_friend() ;

    const c oc1;
    c oc2 ;
    oc1.func();             // invoke const function as the object is const one
    oc2.func();             // invoke non-const function as the object is non-const one

    //oc1.func
    return ;
}

namespace test9_space{
    class base{
        public:
            void func(int)
            {
                cout << __PRETTY_FUNCTION__ << endl ;
            }
            void func(int,int)
            {
                cout << __PRETTY_FUNCTION__ << endl ;
            }
    } ;

    class derive : public base {
        public:
            void func(int, int)
            {
                cout << __PRETTY_FUNCTION__ << endl ;
            }
    } ;
} 

// virtual vs. inherit
void test9(void)
{
    using namespace test9_space;

    base b ;
    derive d ;
    base* pb = &b ;
    base* pd = &d ;

    b.func(1);
    b.func(1,1);

    //d.func(1) ;
    d.func(1,1) ;

    pb->func(1) ;
    pb->func(1,1) ;

    pd->func(1);
    pd->func(1,1);
}

namespace test10_space{
    class b{
        public:
            int i ;
    };

    class d : public b{
        public:
            int i ;
    };
}

// pointer of base class to refer derive class vs. member(not function member) override
void test10(void)
{
    using namespace test10_space ;

    b* pb ;
    b ob ;
    d od ;

    od.b::i = 1 ;
    od.i = 2;

    pb = &od ;

    cout << od.i << endl ;
    cout << od.b::i << endl ;
    cout << pb->i << endl ;

    return ;
}

namespace test11_space{
        
    int permutation(char *buff, int begin, int end, int count )
    {
        int i = 0;
        int j = 0;
        int t = 0;

        if (!buff || count <0 || begin < 0 || end < 0)
            return -1 ;

        if (0 == count){
            for(j=0;j<=begin-1;j++)
                cout << buff[j] ;
            cout << endl ;
            return 0;
        }

        for (i = begin; i <= end; i++){
            t = buff[begin] ; buff[begin] = buff[i]; buff[i] = t ;
            if(-1 == permutation(buff, begin+1, end, count-1))
                return -1 ;
            t=buff[begin] ; buff[begin]= buff[i]; buff[i] = t ;
        }

        return 0;
    }

    int combination(char *buff, int number, int count)
    {
        int i = 0 ;

        if(number < count)
            return -1 ;

        for(i=0; i<number; i++){
            cout << buff[i];
            combination(buff+i+1, number-i-1, count-1);
            cout << endl ;
        }

        /* does not completed!! */
        /* cannot display the result */
        /* maybe, we need a tree to record the processing */

        return 0 ;
    }
}

// a implementation of permutation & combination
void test11(void)
{
    using namespace test11_space ;

    char A[100]={'a','b','c','d'} ;

    cout << endl << permutation(A,0,3,2) << endl ;
    cout << endl << combination(A,4,2) << endl ;

    return ;
}

namespace test12_space{

    inline void xchg(int *p, int *q)
    {
        int t=0;

        if(!p || !q)
            cout << "ERROR!!!" << endl ;

        t=*p; *p=*q; *q=t;

        return ;
    }
    
    // l - loading
    // n - number of items
    // w - weights of items
    // v - value of items
    // return the optimal values
    // this function will take a long time, refer to note_1 & note_2
    int f(int l, int n, int *w, int *v)
    {
        int i=0;
        int t=0;
        int m=0;

        if(l<=0||n<=0){
            return 0 ;
        }

        for(i=0;i<n;i++){
            if(l<w[i])
                continue;

            // note_1, this will change w & v
            xchg(&w[i],&w[n-1]);            
            xchg(&v[i],&v[n-1]);

            // not_2, the return value of f(...) depends on w & v
            // as w & v alwasy in changging, 
            // the result of previous f(...) cannot be reused.
            t=v[n-1]+f(l-w[n-1],n-1,w,v);   
                                            
            if(t>m)
                m=t;

            xchg(&w[i],&w[n-1]);
            xchg(&v[i],&v[n-1]);
        }

        return m ;
    }

    // exercise 16.2-2 of Introduction_of_algorithms
    int f_dynamic_programing(int l, int n, int* w, int *v, void* map)
    {
        int t = 0;
        int ret = 0;
        int (*p)[n] ;       // <-- support by C99!!!

        if(!w||!v||!map)
            return -1 ;

        p=(int(*)[n])map;

        if(p[l-1][n-1]!=-1)
            return p[l-1][n-1];

        if(l<=0||n<=0)
            return 0 ;

        // the n'th item is too heavy
        if(l<w[n-1])
            ret=f_dynamic_programing(l,n-1,w,v,map) ;

        // the n'th item can be put in the knapsack
        else{
            // the n'th item is in the knapsack.
            t=f_dynamic_programing(l-w[n-1],n-1,w,v,map)+v[n-1];
            ret=(t>ret)?t:ret;

            // the n'th item is not in the knapsack
            t=f_dynamic_programing(l,n-1,w,v,map);
            ret=(t>ret)?t:ret;
        }

        p[l-1][n-1]=ret;
        return ret ;
    }
}

// a implementation of 0-1 knapsack problem
void test12(void)
{
    using namespace test12_space;

    int w[]={10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,30};
    int v[]={10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,10,20,50,80,90};

    int i=0;
    int l=150;
    int n=sizeof(w)/sizeof(int);
    int map[l][n];

    for(i=0;i<l*n;i++)
        ((int*)map)[i]=-1;

    //cout << f(l,n,w,v) << endl ;      // <-- too slow!!!
    cout << f_dynamic_programing(l,n,w,v,map) << endl ;
    return ;
}

int main(void)
{
    test12();
    //test11();
    //test10();
    //test9();
    //test8();
    //test7();
    //test6();
    //test5();
    //test4();
    //test3() ;
    //test2();
    //test1();

    return 0 ;
}
