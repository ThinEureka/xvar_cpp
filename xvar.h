
#include <functional>
#include <memory>

class xvar_base {
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
                    auto oldId = it++;
                    _sinks.erase(oldId);
                }
            }
        }

        void addSink(xvar_base* sink) {
            _sinks.push_back(sink->w_this());
            sink->_sources.push_back(s_this());
        }

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
            if (xvar_base::_isDirty) {
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
        xvar_const(T v) {
            _value = v;
            xvar_base::_isDirty = true;
        }

    protected:
        void evaluate() override {
            //do nothing
        }

    protected:
        T _value;
};

template <typename T, typename P1>
class xvar_f1 : public xvar<T> {
    public:
        xvar_f1(const std::function<T(P1)>& f, xvar<P1>* p1) {
            _f = f;
            _p1 = p1;
        }

    protected:
        virtual void validate() override {
            xvar<T>::_value = _f(_p1);
        }

    private:
        std::function<T(P1)> _f;
        P1 _p1;
};


