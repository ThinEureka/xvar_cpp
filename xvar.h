
#include <functional>
#include <memory>
#include <iostream>
#include <assert.h>

class xvar_base {
    public:
        std::weak_ptr<xvar_base> w_this(){
            return s_this();
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
            _sinks.push_back(sink->w_this());
            sink->_sources.push_back(s_this());
        }

    protected:
        void validate() {
            if (!_isDirty) return;

            _isDirty = false;
            for (auto it = _sources.begin(); it != _sources.end(); ++it) {
                auto s_ptr = *it;
                s_ptr->validate();
            }

            evaluate();
        }

        void setDirty(){
            if (_isDirty) return;

            _isDirty = true;
            for (auto it = _sinks.begin(); it != _sinks.end();) {
                auto w_ptr = *it;
                std::shared_ptr<xvar_base> s_ptr = w_ptr.lock();
                if (s_ptr) {
                    s_ptr->setDirty();
                    it++;
                } else {
                    auto oldIt = it++;
                    _sinks.erase(oldIt);
                }
            }
        }


    protected:
        virtual void evaluate() = 0;

    protected:
        bool _isDirty;
        std::vector<std::weak_ptr<xvar_base>> _sinks;
        std::vector<std::shared_ptr<xvar_base>> _sources;
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

template <typename T>
class xvar_f0 : public xvar_value<T> {
    public:
        static std::shared_ptr<xvar_base> create(const T v) {
            xvar_f0* x = new xvar_f0();
            x->xvar_value<T>::_value = v;
            x->xvar_base::_isDirty = true;
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

template <typename T, typename S1>
class xvar_f1 : public xvar_value<T> {
    public:
        static std::shared_ptr<xvar_base> create(const std::function<T(S1)>& f, xvar_value<S1>* s1) {
            xvar_f1* x = new xvar_f1();
            x->_f = f;
            x->_s1 = s1;
            x->_isDirty = true;
            auto s_x = x->s_this();
            s1->addSink(x);
            return s_x;
        }

    protected:
        void evaluate() override {
            xvar_value<T>::_value = _f(_s1->_value);
        }

    private:
        std::function<T(S1)> _f;
        xvar_value<S1>* _s1;
};

template <typename T, typename S1, typename S2>
class xvar_f2 : public xvar_value<T> {
    public:
        xvar_f2(const std::function<T(S1, S2)>& f, xvar_value<S1>* s1, xvar_value<S2>* s2) {
            _f = f;
            _s1 = s1;
            _s2 = s2;
            s1->addSink(this);
            xvar_base::_isDirty = true;
        }

    protected:
        virtual void validate() override {
            if (_s1->_isDirty) _s1->validate();
            if (_s2->_isDirty) _s2->validate();
            xvar_value<T>::_value = _f(_s1->_value, _s2->value);
        }

    private:
        std::function<T(S1, S2)> _f;
        xvar_value<S1>* _s1;
        xvar_value<S2>* _s2;
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


    private:
        std::shared_ptr<xvar_base> _p;
};

#define x_f0(type, value) xvar_f0<type>::create((value));
#define x_f1(T, x, exp) xvar_f1<T, decltype(x)::ValueType>::create([=](decltype(x)::ValueType x)-> T {return (exp);}, x.p())



