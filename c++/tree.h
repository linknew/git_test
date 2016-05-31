#ifndef __TREE_H_
#define __TREE_H_

#include <iostream>

#ifndef _DBG
#define _DBG 0
#endif

#if _DBG
#define throw_err(err_no)                                   \
{                                                           \
    cerr << "**ALERT" << endl                               \
         << "[" << __FILE__ << ":" << __LINE__  << "] "     \
         << "throw an error (" << err_no << ")"             \
         << endl ;                                          \
    throw err_no ;                                          \
}
#else
#define throw_err(err_no)                                   \
{                                                           \
    throw err_no ;                                          \
}
#endif





using std::endl;
using std::cerr;

template<class T>
struct node{
    node(T value) : data(value) 
    {
#if _DBG
        cerr << "construction node:" << this << endl ;
#endif
    }
    virtual ~node()
    { 
#if _DBG
        cerr << "destruction node:" << data << endl ;
#endif
    }

    virtual bool operator < (const node&) = 0 ;
    virtual bool operator > (const node&) = 0 ;
    virtual bool operator <=(const node&) = 0 ;
    virtual bool operator >=(const node&) = 0 ;
    virtual bool operator !=(const node&) = 0 ;

    T  data;
};

template<class T> class explorer ;

template<class T>
class guide{
    friend class explorer<T> ;

    public:
        guide() : _ort_type(0) 
        { 
#if _DBG
            cerr << "construction guide:" << this << endl;
#endif
        }
        guide(int ott) : _ort_type(ott) 
        { 
#if _DBG
            cerr << "construction guide:" << this << endl;
#endif
        }
        virtual ~guide()
        { 
#if _DBG
            cerr << "destruction guide:" << this << endl ;
#endif
        }

        virtual node<T>* get_start(const node<T>* ptree_top) const = 0;
        virtual node<T>* get_end(const node<T>* ptree_top) const = 0;
        virtual node<T>* get_next(const node<T>* pcur) const = 0;

    protected:
        inline void set_ort(int type){ _ort_type = type; }
        inline int get_ort(void)const{ return _ort_type; }
        inline void set_keyword(T kw){ _keyword = kw; }
        inline T& get_keyword(void){ return _keyword; }
        int     _ort_type;
        T       _keyword;    // used when _ort_type is ORTT_SEARCH
} ;

template<class T>
class task{
    public:
        task(node<T>* tree, int ort, const T& kw)
            :_state(TTS_NOT_START),_tree(tree),_ort(ort),_keyword(kw)
        { 
#if _DBG
            cerr << "construction task:" << this << endl;
#endif
        }
        virtual ~task()
        { 
#if _DBG
            cerr << "destruction task:" << this << endl;
#endif
        }

        // the state fo tree-task
        enum state{
            TTS_NO_TASK = -1,
            TTS_NOT_START,
            TTS_OK,
            TTS_FAIL
        };

        virtual int do_the_job(node<T>*) = 0;   // return task state
        inline node<T>* get_tree(void){ return _tree; }
        inline void set_tree(node<T>* t){ _tree = t; }
        inline int get_ort(void){ return _ort; }
        inline void set_ort(int ort){ _ort = ort; }
        inline T& get_keyword(void){ return _keyword; }
        inline void set_keyword(T kw){ _keyword = kw; }
        inline int get_state(void){ return _state;}
        inline void reset_state(void){ _state = TTS_NOT_START; }

    protected:
        node<T>*    _tree;
        int         _ort;
        T           _keyword;
        int         _state;     // save the return value of 'do_the_job'
} ;

template<class T>
class explorer{

    public:
        explorer() : _guide(0),_task(0),_last_step(0)
        { 
#if _DBG
            cerr << "construction explorer:" << this << endl;
#endif
        }

        explorer(guide<T>& g) : _guide(&g),_task(0),_last_step(0)
        { 
#if _DBG
            cerr << "construction explorer:" << this << endl;
#endif
        }

        virtual ~explorer()
        { 
#if _DBG
            cerr << "destruction explorer:" << this << endl;
#endif
        }
    
        inline void assign_guide(guide<T>& g){ _guide = &g; }
        inline void assign_task(task<T>& t){ _task = &t; _last_step = NULL; }
        inline node<T>* get_last_step(void)
        {
            return (task<T>::TTS_OK == get_task_state() ? _last_step : NULL );
        }
        inline int  get_task_state(void)
        {
            _task ? _task->get_state() : task<T>::TTS_NO_TASK;
        }
        node<T>* cross_tree(void) // start to traversal the tree.
        {
            node<T>*    cur = NULL ;
            node<T>*    end = NULL ;
            node<T>*    working_node = NULL ;

            if(!_task || !_guide || !_task->get_tree())
                return NULL ;

            _task->reset_state() ;  // reset state when begin
            _guide->set_ort(_task->get_ort()) ;  // set orient type
            _guide->set_keyword(_task->get_keyword()) ; // set keyword for ORTT_SEARCH
            cur = _guide->get_start(_task->get_tree()) ; // get the 'start'
            end = _guide->get_end(_task->get_tree()) ; // get the 'end'
            end = _guide->get_next(end) ;  // refine the 'end'

            /* in this loop, we will repeat the 'job' to complete the task
               we should get the next step first before starting the 'job'
               Because...
               the 'job' may remove cur node(or something else),
               and 'get_next' depends on current node */
            while(cur && (cur != end)){
                _last_step = cur ;  // record it!!
                working_node = cur ;
                cur = _guide->get_next(cur) ; // get the next node before the job starting
                if(task<T>::TTS_OK != _task->do_the_job(working_node))
                    break ;
            }

            return get_last_step() ;
        }

    protected:
        guide<T>*   _guide;
        task<T>*    _task ;
        node<T>*    _last_step; // before take a new step, the current 
                                // step will be recorded as _last_step
} ;

#endif
