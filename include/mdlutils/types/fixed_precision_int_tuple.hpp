//
// Created by marandil on 22.08.18.
//

#ifndef MDLUTILS_TYPES_FIXED_PRECISION_INT_TUPLE_HPP
#define MDLUTILS_TYPES_FIXED_PRECISION_INT_TUPLE_HPP

#include <cstddef>

#include <iomanip>
#include <ostream>
#include <type_traits>


namespace mdl
{
    /* Define the template class */
    template<typename HighType, typename LowType,
         bool IsSigned = std::is_signed<HighType>::value,
         size_t Alignment = sizeof(HighType) + sizeof(LowType) >
    class alignas(Alignment) fixed_precision_int_tuple
    {
    public:
        typedef HighType   high_type;
        typedef LowType    low_type;

        constexpr static bool is_signed = IsSigned;
        constexpr static size_t alignment = Alignment;

        typedef fixed_precision_int_tuple<high_type, low_type, is_signed, alignment>
                           this_type;

    protected:
        low_type   _lo;
        high_type  _hi;

        constexpr static size_t _low_bit_size = sizeof(low_type) * 8;
        constexpr static size_t _high_bit_size = sizeof(high_type) * 8;

    public:
        constexpr fixed_precision_int_tuple () : _lo(0), _hi(0) { }
        constexpr fixed_precision_int_tuple (low_type value) : _lo(value), _hi(0) { }
        constexpr fixed_precision_int_tuple (high_type high, low_type low) : _lo(low), _hi(high) { }

        template <typename Int,
                  class = std::enable_if_t<std::is_integral<Int>::value
                            && std::is_convertible<Int, low_type>::value
                            && sizeof(Int) <= sizeof(low_type) >
                  >
        constexpr fixed_precision_int_tuple (Int value) : _lo(value), _hi(0) { }

        constexpr explicit operator low_type () const
        {
            return this->_lo;
        }
        template <typename Int,
                  class = std::enable_if_t<std::is_integral<Int>::value
                            && std::is_convertible<low_type, Int>::value
                            && sizeof(Int) <= sizeof(low_type) >
                  >
        constexpr explicit operator Int () const
        {
            return Int(this->_lo);
        }

        constexpr explicit operator bool() const { return bool(this->_lo) || bool(this->_hi); }

        /* Logical operators */
        constexpr bool operator ! () const { return !bool(*this); }
        constexpr this_type operator ~ () const { return this_type(~this->_hi, ~this->_lo); }
        constexpr this_type& operator |= (const this_type& rhs) { this->_lo |= rhs._lo; this->_hi |= rhs._hi; return *this; }
        constexpr this_type& operator &= (const this_type& rhs) { this->_lo &= rhs._lo; this->_hi &= rhs._hi; return *this; }
        constexpr this_type& operator ^= (const this_type& rhs) { this->_lo ^= rhs._lo; this->_hi ^= rhs._hi; return *this; }

        constexpr this_type& operator <<= (int amount)
        {
            // Special case, since shifts of >= _low_bit_size may be undefined
            if (amount < int(this_type::_low_bit_size))
            {
                // Special case for negative values
                if (amount < 0)
                    return (*this) >>= (-amount);

                if (amount != 0)
                {
                    this->_hi = (this->_hi << amount) | (this->_lo >> (this_type::_low_bit_size - amount));
                    this->_lo = this->_lo << amount;
                }
            }
            else
            {
                this->_hi = this->_lo << (amount - this_type::_low_bit_size);
                this->_lo = 0;
            }
            return *this;
        }

        constexpr this_type& operator >>= (int amount)
        {
            // Special case, since shifts of >= _low_bit_size may be undefined
            if (amount < int(this_type::_low_bit_size))
            {
                // Special case for negative values
                if (amount < 0)
                    return (*this) >>= (-amount);

                if (amount != 0)
                {
                    this->_lo = (this->_lo >> amount) | (this->_hi << (this_type::_low_bit_size - amount));
                    this->_hi = this->_hi >> amount;
                }
            }
            else
            {
                this->_lo = this->_hi >> (amount - this_type::_low_bit_size);
                this->_hi = 0;
            }
            return *this;
        }

        /* Arithmetic operators */
        constexpr this_type& operator += (const this_type& rhs)
        {
            low_type new_lo = this->_lo + rhs._lo;
            high_type new_hi = this->_hi + rhs._hi;
            if(new_lo < this->_lo) // Carry detection
                new_hi++;

            this->_lo = new_lo;
            this->_hi = new_hi;

            return *this;
        }

        constexpr this_type& operator -= (const this_type& rhs)
        {
            low_type new_lo = this->_lo - rhs._lo;
            high_type new_hi = this->_hi - rhs._hi;
            if(this->_lo < rhs._lo) // Borrow detection
                new_hi--;

            this->_lo = new_lo;
            this->_hi = new_hi;

            return *this;
        }

        constexpr this_type& operator *= (const this_type& rhs)
        {
            if(!this_type::is_signed)
            {
                constexpr size_t half_low_bit_size = this_type::_low_bit_size / 2;
                constexpr low_type lo_lo_mask = (low_type(1) << half_low_bit_size) - 1;
                low_type lhs_lo_lo = this->_lo & lo_lo_mask;
                low_type lhs_lo_hi = this->_lo >> half_low_bit_size;
                low_type rhs_lo_lo = rhs._lo & lo_lo_mask;
                low_type rhs_lo_hi = rhs._lo >> half_low_bit_size;

                this->_hi = this->_lo * rhs._hi
                          + this->_hi * rhs._lo
                          + lhs_lo_hi * rhs_lo_hi;

                this->_lo = lhs_lo_lo * rhs_lo_lo;

                this_type mid = this_type(lhs_lo_lo * rhs_lo_hi) + this_type(lhs_lo_hi * rhs_lo_lo);

                *this += (mid << half_low_bit_size);
            }
            else
            {
                static_assert(!this_type::is_signed, "Signed multiplication not implemented yet!");
            }
            return *this;
        }


        /* Comparison operators */
        constexpr bool operator == (const this_type& rhs) const
        { return this->_lo == rhs._lo && this->_hi == rhs._hi; }

        constexpr bool operator != (const this_type& rhs) const
        { return this->_lo != rhs._lo && this->_hi != rhs._hi; }

        constexpr bool operator < (const this_type& rhs) const
        { return this->_hi < rhs._hi || (this->_hi == rhs._hi && this->_lo < rhs._lo); }

        constexpr bool operator > (const this_type& rhs) const
        { return rhs < (*this); }

        constexpr bool operator <= (const this_type& rhs) const
        { return !(rhs < (*this)); }

        constexpr bool operator >= (const this_type& rhs) const
        { return !((*this) < rhs); }

    };

    /* Simple operators */
    #define GRAFPE_DECLARE_SIMPLE_OP(op) \
    template<typename H, typename L, bool S, size_t A> \
    constexpr inline fixed_precision_int_tuple<H,L,S,A> operator op (fixed_precision_int_tuple<H,L,S,A> lhs, fixed_precision_int_tuple<H,L,S,A> rhs) { return (lhs op##= rhs); }
    GRAFPE_DECLARE_SIMPLE_OP(+)
    GRAFPE_DECLARE_SIMPLE_OP(-)
    GRAFPE_DECLARE_SIMPLE_OP(*)
    GRAFPE_DECLARE_SIMPLE_OP(|)
    GRAFPE_DECLARE_SIMPLE_OP(&)
    GRAFPE_DECLARE_SIMPLE_OP(^)
    #undef GRAFPE_DECLARE_SIMPLE_OP

    template<typename H, typename L, bool S, size_t A>
    constexpr inline fixed_precision_int_tuple<H,L,S,A> operator << (fixed_precision_int_tuple<H,L,S,A> lhs, int amount) { return (lhs <<= amount); }
    template<typename H, typename L, bool S, size_t A>
    constexpr inline fixed_precision_int_tuple<H,L,S,A> operator >> (fixed_precision_int_tuple<H,L,S,A> lhs, int amount) { return (lhs >>= amount); }


    /* Simple operators with other integer types - int-rhs case*/
    #define GRAFPE_DECLARE_CONVERSION_OP(op) \
    template<typename H, typename L, bool S, size_t A, typename T, class = std::enable_if_t<std::is_convertible<T, fixed_precision_int_tuple<H,L,S,A>>::value> >\
    constexpr inline fixed_precision_int_tuple<H,L,S,A> operator op (fixed_precision_int_tuple<H,L,S,A> lhs, T rhs) { return (lhs op##= rhs); }
    GRAFPE_DECLARE_CONVERSION_OP(+)
    GRAFPE_DECLARE_CONVERSION_OP(-)
    GRAFPE_DECLARE_CONVERSION_OP(*)
    GRAFPE_DECLARE_CONVERSION_OP(|)
    GRAFPE_DECLARE_CONVERSION_OP(&)
    GRAFPE_DECLARE_CONVERSION_OP(^)
    #undef GRAFPE_DECLARE_CONVERSION_OP

    /* Simple operators with other integer types - int-lhs case */
    #define GRAFPE_DECLARE_CONVERSION_OP(op) \
    template<typename H, typename L, bool S, size_t A, typename T, class = std::enable_if_t<std::is_convertible<T, fixed_precision_int_tuple<H,L,S,A>>::value> >\
    constexpr inline fixed_precision_int_tuple<H,L,S,A> operator op (T lhs, fixed_precision_int_tuple<H,L,S,A> rhs) { return (fixed_precision_int_tuple<H,L,S,A>(lhs) op##= rhs); }
    GRAFPE_DECLARE_CONVERSION_OP(+)
    GRAFPE_DECLARE_CONVERSION_OP(-)
    GRAFPE_DECLARE_CONVERSION_OP(*)
    GRAFPE_DECLARE_CONVERSION_OP(|)
    GRAFPE_DECLARE_CONVERSION_OP(&)
    GRAFPE_DECLARE_CONVERSION_OP(^)
    #undef GRAFPE_DECLARE_CONVERSION_OP
}

#endif // MDLUTILS_TYPES_FIXED_PRECISION_INT_TUPLE_HPP
