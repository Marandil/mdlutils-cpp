//
// Created by marandil on 04.09.15.
//

#ifndef MDLUTILS_CPP_GETSET_ACCESSOR_HPP
#define MDLUTILS_CPP_GETSET_ACCESSOR_HPP

#include <functional>

namespace mdl
{

    template<typename T, typename Setter=std::function<void(const T &)>>
    class set_accessor
    {
    protected:
        Setter setter;

    public:
        set_accessor(Setter set) : setter{set} { };

        set_accessor<T, Setter> &operator=(const T &value)
        {
            setter(value);
            return *this;
        }
    };

    template<typename T, typename Getter=std::function<T(void)>>
    class get_accessor
    {
    protected:
        Getter getter;

    public:
        get_accessor(Getter get) : getter{get} { };

        operator T() const
        {
            return std::move(getter());
        }
    };

    /* This class of accessors only provide access to a object of type T via set(const T&) and T get() functions.
     * To provide access with T& get, see <refgetset_accessor>.
     */
    template<typename T, typename Getter=std::function<T()>, typename Setter=std::function<void(const T &)> >
    class getset_accessor : public get_accessor<T, Getter>, public set_accessor<T, Setter>
    {
    protected:
        Getter getter;
        Setter setter;

    public:
        getset_accessor(Getter get, Setter set) : get_accessor<T, Getter>(get), set_accessor<T, Setter>(set) { }

        getset_accessor<T, Getter, Setter> &operator=(const T &value)
        {
            set_accessor<T, Setter>::operator=(value);
            return *this;
        };
    };

}

#endif //MDLUTILS_CPP_GETSET_ACCESSOR_HPP
