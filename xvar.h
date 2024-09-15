
#include <functional>
#include <memory>

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

        void addSink(xvar_base* sink) {
            _sinks.push_back(sink->w_this());
            sink->_sources.push_back(s_this());
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
class xvar : public xvar_base {
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
class xvar_const : public xvar<T> {
    public:
        xvar_const(const T v) {
            xvar<T>::_value = v;
            xvar_base::_isDirty = true;
        }

    protected:
        void evaluate() override {
            //do nothing
        }
};

template <typename T, typename S1>
class xvar_f1 : public xvar<T> {
    public:
        xvar_f1(const std::function<T(S1)>& f, xvar<S1>* s1) {
            _f = f;
            _s1 = s1;
            s1->addSink(this);
            xvar_base::_isDirty = true;
        }

    protected:
        virtual void validate() override {
            if (_s1->_isDirty) {
                _s1->validate();
            }
            xvar<T>::_value = _f(_s1->_value);
        }

    private:
        std::function<T(S1)> _f;
        xvar<S1>* _s1;
};

template <typename T, typename S1, typename S2>
class xvar_f2 : public xvar<T> {
    public:
        xvar_f2(const std::function<T(S1, S2)>& f, xvar<S1>* s1, xvar<S2>* s2) {
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
            xvar<T>::_value = _f(_s1->_value, _s2->value);
        }

    private:
        std::function<T(S1, S2)> _f;
        xvar<S1>* _s1;
        xvar<S2>* _s2;
};

template<typename T>
class xvar_ptr {
    public:
        xvar_ptr(std::shared_ptr<xvar_base> p) {
            _p = p;
        }

        virtual const T& operator () (){ 
            xvar_base* rawPtr = _p.get();
            xvar<T>* rawPtrT = static_cast<xvar<T>*>(rawPtr);
            return (*rawPtrT)();
        }


    private:
        std::shared_ptr<xvar_base> _p;
};

// #define X_CONST(type, name, value) std::shared_ptr<xvar_const<type>> name = (new xvar_const<type>((value)))->s_this()
// #define X_CONST(type, name, value) std::shared_ptr<xvar_const<type>> name;// = (new xvar_const<type>((value)))->s_this()
// #define X_CONST(type, name, value) std::shared_ptr<xvar_const<type>> name = std::static_pointer_cast<xvar_const<type>>((new xvar_const<type>((value)))->s_this());
//
//
 #define X_CONST(type, name, value) xvar_ptr<type> name = (new xvar_const<type>((value)))->s_this();



