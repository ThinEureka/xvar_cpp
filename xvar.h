/*!
 * \file AsysValue.h
 * \date 9-15-2024 
 * 
 * https://github.com/ThinEureka/xvar_cpp
 * \author cs (04nycs@gmail.com)
 */

#include <functional>
#include <memory>
// #include <iostream>
#include <assert.h>
#include <tuple>
#include <map>

class xvar_base {
#ifdef XVAR_TEST_LIFETIME 
    public:
        static int s_totalCount;

    public:
        xvar_base(){
            s_totalCount++;
        }
#endif

    public:
        std::weak_ptr<xvar_base> w_this(){
            s_this();
            return _w_this;
        }

        std::shared_ptr<xvar_base> s_this(){
            auto s_this = _w_this.lock();
            if (s_this) {
                return s_this;
            }

            s_this = std::shared_ptr<xvar_base>(this);
            _w_this = s_this;

            return s_this;
        }

        void addSink(xvar_base* sink) {
            auto it = _sinks.find(sink);
            if (it == _sinks.end()){
                _sinks.insert({sink, sink});
                sink->_sources.insert({this, s_this()});
            }
        }

        void removeSink(xvar_base* sink) {
            _sinks.erase(_sinks.find(sink));
            sink->_sources.erase(sink->_sources.find(this));
        }

    public:
        virtual ~xvar_base() {
#ifdef XVAR_TEST_LIFETIME 
            s_totalCount--;
#endif 
            while (!_sources.empty()){
                auto it = _sources.begin();
                auto source = it->first;
                source->removeSink(this);
            }
        }

    protected:
        void validate() {
            if (!_isDirty) return;

            _isDirty = false;
            for (auto it = _sources.begin(); it != _sources.end(); ++it) {
                auto ptr = it->first;
                ptr->validate();
            }

            evaluate();
        }

        void setDirty(){
            if (_isDirty) return;

            _isDirty = true;
            for (auto it = _sinks.begin(); it != _sinks.end(); ++it) {
                auto ptr = it->first;
                ptr->setDirty();
            }
        }


    protected:
        virtual void evaluate() = 0;

    protected:
        bool _isDirty = true;
        std::map<xvar_base*, xvar_base*> _sinks;
        std::map<xvar_base*, std::shared_ptr<xvar_base>> _sources;
        std::weak_ptr<xvar_base> _w_this;
};

template <typename T>
class xvar_value : public xvar_base {
    public:
        virtual const T& operator () (){ 
            if (!xvar_base::_isDirty) {
                return _value;
            }
            validate();
            return _value;
        }

        T _value;
};

template <typename T> class xvar;

template <typename T>
class xvar_f0 : public xvar_value<T> {
    public:
        static std::shared_ptr<xvar_base> create(const T& v) {
            xvar_f0* x = new xvar_f0();
            x->xvar_value<T>::_value = v;
            // x->xvar_base::_isDirty = true;
            auto s_x = x->s_this();
            return s_x;
        }

        void setValue(const T& v) {
            xvar_value<T>::_value = v;
            xvar_value<T>::setDirty();
        }

    protected:
        void evaluate() override {
            //do nothing
        }
};

template <typename T, typename ...Sn>
class xvar_fn : public xvar_value<T> {
    public:
        static std::shared_ptr<xvar_base> create(const std::function<T(Sn...)>& f, xvar_value<Sn>*... args) {
            xvar_fn* x = new xvar_fn();
            x->_f = f;
            x->_sn = std::tuple(args...);
            // x->_isDirty = true;
            auto s_x = x->s_this();
            (args->addSink(x),...);

            return s_x;
        }

        static xvar<T> link(const std::function<T(Sn...)>& f, xvar<Sn>... args) {
            xvar_fn* x = new xvar_fn();
            x->_f = f;
            x->_sn = std::tuple(args.p()...);
            // x->_isDirty = true;
            auto s_x = x->s_this();
            (args.p()->addSink(x),...);

            return xvar<T>(s_x);
        }

        static std::shared_ptr<xvar_base> create(const std::function<T(Sn...)>& f, const std::tuple<xvar_value<Sn>*...>& sn) {
            xvar_fn* x = new xvar_fn();
            x->_f = f;
            x->_sn = sn;
            // x->_isDirty = true;
            auto s_x = x->s_this();
            // (args->addSink(x),...);
            tupleAddSkink(x, x->_sn, std::index_sequence_for<Sn...>());

            return s_x;
        }

        template<std::size_t... Is>
        static void tupleAddSkink(xvar_fn* x, std::tuple<xvar_value<Sn>*...>& tuple, std::index_sequence<Is...>) {
            (std::get<Is>(tuple)->addSink(x), ...);
        }

    protected:
        void evaluate() override { 
            tupleEvaluate(_sn, std::index_sequence_for<Sn...>());
        }

        template<std::size_t... Is>
        void tupleEvaluate(std::tuple<xvar_value<Sn>*...>& tuple, std::index_sequence<Is...>) {
            this->xvar_value<T>::_value = _f(std::get<Is>(tuple)->_value...);
        }

    private:
        std::function<T(Sn...)> _f;
        std::tuple<xvar_value<Sn>*...> _sn;
};


template<typename T>
class xvar {
    public:
        typedef T ValueType;
        typedef xvar_value<T> VariableType;

        xvar_value<T>* p() {
            xvar_base* rawPtr = _p.get();
            xvar_value<T>* rawPtrT = static_cast<xvar_value<T>*>(rawPtr);
            return rawPtrT;
        }

        xvar(std::shared_ptr<xvar_base> p) {
            _p = p;
        }

        xvar(const T& v){
            _p = xvar_f0<T>::create(v);
        }

        virtual const T& operator () (){ 
            xvar_base* rawPtr = _p.get();
            xvar_value<T>* rawPtrT = dynamic_cast<xvar_value<T>*>(rawPtr);
            if (rawPtrT) {
                return (*rawPtrT)();
            }

            return (*rawPtrT)();
        }

        void setValue(const T& v) {
            xvar_base* rawPtr = _p.get();
            xvar_f0<T>* rawPtrT = static_cast<xvar_f0<T>*>(rawPtr);
            assert(rawPtrT);
            if (rawPtrT) {
                rawPtrT->setValue(v);
            }
        }

        // template <typename S1>
        // static xvar f1(const std::function<T(S1)>& f, xvar<S1> s1) {
            // return xvar(xvar_f1<T, S1>::create(f, s1.p()));
        // }
//
        // template <typename ...Sn>
        // static void fn(const std::function<T(Sn...)>& f){
            // return xvar(xvar_fn<T, Sn...>::create(f, args...));
            // return xvar();
        // }
//
    private:
        std::shared_ptr<xvar_base> _p;
};

template<typename... Sn>
class xvar_tuple{
    public:
        xvar_tuple(xvar<Sn>... args) : _sn(args.p()...)
        {
        }

        // template <typename T>
        // xvar<T> operator >> (const std::function<T(Sn...)>& f){
            // return xvar<T>(xvar_fn<T, Sn...>::create(f, _sn));
        // }

        template <typename T>
        xvar<T> link(const std::function<T(Sn...)>& f){
            return xvar<T>(xvar_fn<T, Sn...>::create(f, _sn));
        }
//
    private:
        std::tuple<xvar_value<Sn>*...> _sn;
};

template<typename... Sn>
auto x_tuple(xvar<Sn>... sn)-> xvar_tuple<Sn...> {
    return xvar_tuple<Sn...>(sn...);
}

template<typename T, typename... Sn>
class xvar_form{
    public:
        xvar_form(xvar<Sn>... args) : _sn(args.p()...)
        {
        }

        xvar<T> operator >> (const std::function<T(Sn...)>& f){
            return xvar<T>(xvar_fn<T, Sn...>::create(f, _sn));
        }

        // xvar<T> link(const std::function<T(Sn...)>& f){
            // return xvar<T>(xvar_fn<T, Sn...>::create(f, _sn));
        // }
    private:
        std::tuple<xvar_value<Sn>*...> _sn;
};


template<typename T, typename ...Sn>
auto x_form(xvar<Sn>...sn)->xvar_form<T, Sn...> {
    return xvar_form<T, Sn...>(sn...);
}
#define x_define_unary_op(op) \
template<typename S1>\
auto operator op (xvar<S1> s1) -> xvar<decltype(op s1())> {\
    typedef decltype(op s1()) T;\
    return xvar<T>(xvar_fn<T, S1>::create([](S1 s1)-> T {return op s1;}, s1.p()));\
}

#define x_define_binary_op(op) \
template<typename S1, typename S2>\
auto operator op (xvar<S1> s1, xvar<S2> s2) -> xvar<decltype(s1() op s2())> {\
    typedef decltype(s1() op s2()) T;\
    return xvar<T>(xvar_fn<T, S1, S2>::create([](S1 s1, S2 s2)-> T {return s1 op s2;}, s1.p(), s2.p()));\
}\
\
template<typename S1, typename S2>\
auto operator op (xvar<S1> s1, const S2& s2) -> xvar<decltype(s1() op s2)> {\
    typedef decltype(s1() op s2) T;\
    return xvar<T>(xvar_fn<T, S1, S2>::create([](S1 s1, S2 s2)-> T {return s1 op s2;}, s1.p(), xvar<S2>(s2).p()));\
}\
\
template<typename S1, typename S2>\
auto operator op (const S1& s1, xvar<S2> s2) -> xvar<decltype(s1 op s2())> {\
    typedef decltype(s1 op s2()) T;\
    return xvar<T>(xvar_fn<T, S1, S2>::create([](S1 s1, S2 s2)-> T {return s1 op s2;}, xvar<S1>(s1).p(), s2.p()));\
}

//Arithmetic operators
x_define_binary_op(+)
x_define_binary_op(-)
x_define_binary_op(*)
x_define_binary_op(/)
x_define_binary_op(%)

x_define_unary_op(-)

//Relational and comparison operators
x_define_binary_op(==)
x_define_binary_op(!=)
x_define_binary_op(<)
x_define_binary_op(>)
x_define_binary_op(<=)
x_define_binary_op(>=)

//Logical operators 
x_define_unary_op(!)
x_define_binary_op(&&)
x_define_binary_op(||)

//Bitwise operators
x_define_binary_op(&)
x_define_binary_op(|)
x_define_binary_op(<<)
x_define_binary_op(>>)


#define x_f0(T, value) xvar<T>(xvar_f0<T>::create((value)))

#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...)                                    \
          __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                         \
          macro(a1)                                                     \
      __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() ,FOR_EACH_HELPER

#define AUTO(x) auto x

#define x_fn(T, exp, ...) x_form<T>(__VA_ARGS__) >> \
    [=](FOR_EACH(AUTO, __VA_ARGS__)) \
        {return exp; }
