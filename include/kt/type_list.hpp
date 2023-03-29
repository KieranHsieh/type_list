/*
MIT License

Copyright (c) 2023 Kieran Hsieh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef KT_TYPE_LIST_HPP
#define KT_TYPE_LIST_HPP

#include <cstddef>
#include <limits>
#include <utility>

namespace kt {
    /**
     * @brief Removes const, volatile, and reference qualifiers from a type T
     *
     * @tparam T The type
     */
    template<typename T>
    struct remove_cvref {
        using type = std::remove_cv_t<std::remove_reference_t<T>>; /**< The output type */
    };

    /**
     * @relates remove_cvref
     *
     * @brief Shorthand alias for remove_cvref
     */
    template<typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;

    /**
     * @brief Generic type carrier
     *
     * @tparam T The type wrapped by the tag
     */
    template<typename T>
    struct tag_t {
        using type = T; /**< The wrapped type */

        /**
         * @brief Constructs the tag_t
         */
        constexpr explicit tag_t() = default;
    };

    /**
     * @relates tag_t
     *
     * @brief Global instantiation of a tag_t<T>
     */
    template<typename T>
    constexpr tag_t<T> tag = tag_t<T>();

    /**
     * @brief Extracts the wrapped type from a tag_t structure
     *
     * @tparam T The tag_t to extract the type from
     */
    template<typename T>
    struct from_tag;

    /**
     * @cond TURN_OFF_DOXYGEN
     */
    template<typename T>
    struct from_tag<tag_t<T>> {
        using type = T;
    };
    /**
     * @endcond
     */

    /**
     * @relates from_tag
     *
     * @brief Shorthand alias for from_tag<T>
     */
    template<typename T>
    using from_tag_t = typename from_tag<T>::type;

    /**
     * @brief Template wrapper for templated metafunctions
     *
     * @tparam F The unspecialized metafunction
     */
    template<template<typename> class F>
    struct func_t {
        /**
         * @brief Constructs the func_t
         */
        constexpr explicit func_t() = default;
    };

    /**
     * @relates func_t
     *
     * @brief Global instantiation of func_t<F>
     */
    template<template<typename> class F>
    constexpr func_t<F> func = func_t<F>();

    /**
     * @brief Template wrapper for an unsigned integer index
     *
     * @tparam I The index
     */
    template<size_t I>
    struct index_t {
        static constexpr size_t value = I; /**< The index */

        /**
         * @brief Constructs the index_t
         */
        constexpr explicit index_t() = default;
    };

    /**
     * @relates index_t
     *
     * @brief Global instantiation of index_t
     */
    template<size_t I>
    constexpr index_t<I> index = index_t<I>();

    /**
     * @brief Used to denote an invalid index
     *
     */
    static constexpr size_t npos = std::numeric_limits<size_t>::max();

    template<typename... TS>
    struct type_list_t;

    /**
     * @cond TURN_OFF_DOXYGEN
     */
    namespace detail {
        template<typename T>
        constexpr T declfwd();

        template<size_t I, template<typename> class F>
        struct indexed_func_t {
            constexpr explicit indexed_func_t() = default;
        };

        template<size_t S, size_t E, template<typename> class F>
        struct ranged_func_t {
            constexpr explicit ranged_func_t() = default;
        };

        template<size_t S, size_t E>
        struct range_t {
            constexpr explicit range_t() = default;
        };

        struct ignore_t {
            constexpr explicit ignore_t() = default;
        };

        template<typename... TS>
        struct type_list_first_type;

        template<typename T, typename... TS>
        struct type_list_first_type<T, TS...> {
            using type = T;
        };

        template<typename... TS>
        using type_list_first_type_t = typename type_list_first_type<TS...>::type;

        template<size_t I, typename = std::make_index_sequence<I>>
        struct type_list_element_impl;

        template<size_t I, size_t... IGNORED>
        struct type_list_element_impl<I, std::index_sequence<IGNORED...>> {
            template<typename T>
            static constexpr T fnc(decltype((void*) IGNORED)..., T*, ...);
        };

        template<size_t I, typename... TS>
        struct type_list_element {
            using type = typename decltype(type_list_element_impl<I>::fnc(static_cast<tag_t<TS>*>(0)...))::type;
        };

        template<size_t I, typename... TS>
        using type_list_element_t = typename type_list_element<I, TS...>::type;

        struct type_list_index_of_tag { };
        struct type_list_count_of_tag { };
        struct type_list_remove_tag { };
        struct type_list_remove_index_tag { };
        struct type_list_apply_index_tag { };
        struct type_list_apply_range_tag { };
        struct type_list_filter_tag { };
        struct type_list_filter_range_tag { };

        template<typename T, typename TL, typename TAG, typename = std::make_index_sequence<TL().size()>>
        struct type_list_with_iseq;

        template<typename T, typename TL>
        constexpr size_t type_list_index_of_v = TL().contains(tag<T>) ? type_list_with_iseq<T, TL, type_list_index_of_tag>::value : npos;

        template<typename T, typename TL>
        constexpr size_t type_list_count_of_v = type_list_with_iseq<T, TL, type_list_count_of_tag>::value;

        template<typename T, typename TL>
        using type_list_remove_t = typename type_list_with_iseq<T, TL, type_list_remove_tag>::type;

        template<size_t S, size_t E, typename TL>
        using type_list_remove_index_t = typename type_list_with_iseq<range_t<S, E>, TL, type_list_remove_index_tag>::type;

        template<size_t I, template<typename> class F, typename TL>
        using type_list_apply_index_t = typename type_list_with_iseq<indexed_func_t<I, F>, TL, type_list_apply_index_tag>::type;

        template<size_t S, size_t E, template<typename> class F, typename TL>
        using type_list_apply_range_t = typename type_list_with_iseq<ranged_func_t<S, E, F>, TL, type_list_apply_range_tag>::type;

        template<template<typename> class F, typename TL>
        using type_list_filter_t = typename type_list_with_iseq<func_t<F>, TL, type_list_filter_tag>::type;

        template<size_t S, size_t E, template<typename> class F, typename TL>
        using type_list_filter_range_t = typename type_list_with_iseq<ranged_func_t<S, E, F>, TL, type_list_filter_range_tag>::type;
    }
    /**
     * @endcond
     */

    /**
     * @brief A metaprogramming construct representing a list of types
     *
     * @tparam TS The types in the type list
     */
    template<typename... TS>
    struct type_list_t {
    private:
        template<size_t I>
        static constexpr bool index_in_range(index_t<I> /* index */) {
            return I >= 0 && I < sizeof...(TS);
        }

    public:
        /**
         * @brief Constructs the type_list_t
         */
        constexpr explicit type_list_t() = default;

        /**
         * @brief Returns a tag_t holding the first type in the type list.
         *
         * @note Calling this function give a compiler error if the type list is empty
         */
        constexpr auto front() const { return tag_t<detail::type_list_first_type_t<TS...>>(); }

        /**
         * @brief Returns a tag_t holding the last type in the type list.
         *
         * @note Calling this function give a compiler error if the type list is empty
         */
        constexpr auto back() const { return tag_t<decltype((detail::declfwd<TS>(), ...))>(); }

        /**
         * @brief Returns a tag_t holding the ith element of the type list, where i is a 0-based index.
         *
         * @note Calling this function give a compiler error if the given index is valid for the calling type list
         */
        template<size_t I>
        constexpr auto at(index_t<I> /* index */) const {
            static_assert(index_in_range(index<I>), "Cannot access out of range index of type list");
            return tag_t<detail::type_list_element_t<I, TS...>>();
        }

        /**
         * @brief Returns the number of elements in the type list
         */
        constexpr size_t size() const { return sizeof...(TS); }

        /**
         * @brief Applies a metafunction to each type in the type list
         *
         * @details The metafunction is applied to each type in the type list as ```typename F<T>::type```
         *
         * @tparam F The metafunction to apply
         *
         * @return A new type list, with the metafunction applied to each element of the old type list
         */
        template<template<typename> class F>
        constexpr auto apply(func_t<F> /* functor */) const {
            return type_list_t<typename F<TS>::type...>();
        }

        /**
         * @brief Applies a metafunction to the type at index I of the type list
         *
         * @details The metafunction is applied to the type as ```typename F<T>::type```
         *
         * @tparam I The index
         * @tparam F The metafunction to apply
         *
         * @return A new type list, with the metafunction applied to the element at index I
         *
         * @note Calling this function will give a compile error if the given index is out of the range of the type list
         */
        template<size_t I, template<typename> class F>
        constexpr auto apply(index_t<I> /* index */, func_t<F> /* functor */) const {
            static_assert(index_in_range(index<I>), "Cannot apply metafunction to out of range index of type list");
            return detail::type_list_apply_index_t<I, F, type_list_t<TS...>>();
        }

        /**
         * @brief Applies a metafunction to an index range in the type list
         *
         * @details The metafunction is applied to each type as ```typename F<T>::type``` in the range [start, end)
         *
         * @tparam S The starting index to apply to
         * @tparam E One past the last index
         * @tparam F The metafunction to apply
         *
         * @return A new type list, with the metafunction applied to the given range of types
         *
         * @note Calling this function will give a compile error if either the starting or ending indices result in applying the
         * metafunction to an out of range type.
         */
        template<size_t S, size_t E, template<typename> class F>
        constexpr auto apply_range(index_t<S> /* start */, index_t<E> /* end */, func_t<F> /* functor */) const {
            static_assert(index_in_range(index<S>) && index_in_range(index<E - 1>), "Cannot remove out of range index from type list");
            if constexpr(E <= S) {
                return *this;
            }
            else {
                return detail::type_list_apply_range_t<S, E, F, type_list_t<TS...>>();
            }
        }

        /**
         * @brief Filters out all types where the given metafunction returns false
         *
         * @details The metafunction is applied to each type as ```F<T>::value```
         *
         * @tparam F The metafunction used to filter
         *
         * @return A new type list with all elements in which the given metafunction returned true
         */
        template<template<typename> class F>
        constexpr auto filter(func_t<F> /* functor */) const {
            return detail::type_list_filter_t<F, type_list_t<TS...>>();
        }

        /**
         * @brief Filters out all types in the given index range where the given metafunction returns false
         *
         * @details The metafunction is applied to each type as ```F<T>::value``` in the range [start, end)
         *
         * @tparam S The starting index to start filtering
         * @tparam E One past the last index
         * @tparam F The metafunction to use
         *
         * @return A new type list with all elements in which the given metafunction returned true
         *
         * @note Calling this function will give a compile error if either the starting or ending indices result in applying the
         * metafunction to an out of range type.
         */
        template<size_t S, size_t E, template<typename> class F>
        constexpr auto filter_range(index_t<S> /* start */, index_t<E> /* end */, func_t<F> /* functor */) const {
            static_assert(index_in_range(index<S>) && index_in_range(index<E - 1>), "Cannot remove out of range index from type list");
            if constexpr(E <= S) {
                return *this;
            }
            else {
                return detail::type_list_filter_range_t<S, E, F, type_list_t<TS...>>();
            }
        }

        /**
         * @brief Appends a type to the end of the type list
         *
         * @tparam T The type to append to the end of the type list
         *
         * @return A new type list with the provided type appended
         */
        template<typename T>
        constexpr auto append(tag_t<T> /* type */) const {
            if constexpr(std::is_same_v<remove_cvref_t<T>, detail::ignore_t>) {
                return type_list_t<TS...>();
            }
            else {
                return type_list_t<TS..., T>();
            }
        }

        /**
         * @brief Appends the types from another type list to the end of the calling type list
         *
         * @tparam OTS The types in the other type list
         *
         * @return A new type list with the types from the other type list appended
         */
        template<typename... OTS>
        constexpr auto append(type_list_t<OTS...> /* type_list */) const {
            return (type_list_t<TS...>() + ... + tag<OTS>);
        }

        /**
         * @brief Prepends a type to the end of the type list
         *
         * @tparam T The type to prepend to the end of the type list
         *
         * @return A new type list with the provided type prepended
         */
        template<typename T>
        constexpr auto prepend(tag_t<T> /* type */) const {
            if constexpr(std::is_same_v<remove_cvref_t<T>, detail::ignore_t>) {
                return type_list_t<TS...>();
            }
            else {
                return type_list_t<T, TS...>();
            }
        }

        /**
         * @brief Prepends the types from another type list to the end of the calling type list
         *
         * @tparam OTS The types in the other type list
         *
         * @return A new type list with the types from the other type list prepended
         */
        template<typename... OTS>
        constexpr auto prepend(type_list_t<OTS...> /* type_list */) const {
            return (type_list_t<OTS...>() + ... + tag<TS>);
        }

        /**
         * @brief Removes all occurences of the given type from the type list
         *
         * @tparam T The type to remove
         *
         * @return A new type list with all instances of the given type removed
         */
        template<typename T>
        constexpr auto remove(tag_t<T> /* type */) const {
            return detail::type_list_remove_t<T, type_list_t<TS...>>();
        }

        /**
         * @brief Removes a range of types from the type list
         *
         * @tparam S The starting index
         * @tparam E The ending index
         *
         * @return A new type list with the given range of types removed
         *
         * @note Calling this function will give a compile error if the range [S, E) results in removing at an out of bounds index
         */
        template<size_t S, size_t E = S + 1>
        constexpr auto remove(index_t<S> /* start */, index_t<E> /* end */ = index_t<E>()) const {
            static_assert(index_in_range(index<S>) && index_in_range(index<E - 1>), "Cannot remove out of range index from type list");
            if constexpr(E <= S) {
                return *this;
            }
            else {
                return detail::type_list_remove_index_t<S, E, type_list_t<TS...>>();
            }
        }

        /**
         * @brief Returns the index of a given type in the type list
         *
         * @tparam T The type to look up
         *
         * @note This will give a compile error if the provided type is not unique in the type list (count_of(tag<T>) >= 2)
         */
        template<typename T>
        constexpr size_t index_of(tag_t<T> /* type */) const {
            static_assert(detail::type_list_count_of_v<T, type_list_t<TS...>> < 2, "Cannot find index of non-unique type in type list");
            return detail::type_list_index_of_v<T, type_list_t<TS...>>;
        }

        /**
         * @brief Returns the number of times a given type occurs in the type list
         *
         * @tparam T The type to count occurences of
         */
        template<typename T>
        constexpr size_t count_of(tag_t<T> /* type */) const {
            return detail::type_list_count_of_v<T, type_list_t<TS...>>;
        }

        /**
         * @brief Returns if the type list contains a given type
         *
         * @tparam T The type to check for
         */
        template<typename T>
        constexpr bool contains(tag_t<T> /* type */) const {
            return (std::is_same_v<T, TS> || ... || false);
        }

        /**
         * @brief Returns if the type list contains no elements
         *
         */
        constexpr bool empty() const { return sizeof...(TS) == 0; }

        /**
         * @brief Returns if the given metafunction evaluates to true for all types in the type list
         *
         * @details The metafunction is aplied to each type in the type list as ```F<T>::value```.
         * For empty type lists, this will always return true
         *
         * @tparam F The metafunction
         */
        template<template<typename> class F>
        constexpr bool all_of(func_t<F> /* functor */) const {
            return (F<TS>::value && ... && true);
        }

        /**
         * @brief Returns if the given metafunction evaluates to true for at least one type in the type list
         *
         * @details The metafunction is applied to each type in the type list as ```F<T>::value```.
         * For empty type lists, this will always return false
         *
         * @tparam F The metafunction
         */
        template<template<typename> class F>
        constexpr bool one_of(func_t<F> /* functor */) const {
            return (F<TS>::value || ... || false);
        }

        /**
         * @brief Appends the given type to the end of the type list
         *
         * @tparam T The type to append
         * @param elem A tag wrapping the type to be appended
         * @return constexpr auto A new type list with the type appended to the end
         */
        template<typename T>
        constexpr auto operator+(tag_t<T> elem) const {
            if constexpr(std::is_same_v<remove_cvref_t<T>, detail::ignore_t>) {
                return type_list_t<TS...>();
            }
            else {
                return type_list_t<TS..., T>();
            }
        }

        /**
         * @brief Appends the types in another type list to the end of the type list
         *
         * @tparam OTS The types in the other type list
         * @param other The other type list
         * @return constexpr auto A new type list with the types from the other type list appended to the end
         */
        template<typename... OTS>
        constexpr auto operator+(type_list_t<OTS...> other) const {
            return (type_list_t<TS...>() + ... + tag<OTS>);
        }

        /**
         * @brief Returns if all types in the calling type list are also in the other type list at the same indices
         *
         * @tparam OT The types in the other type list
         * @param other The other type list
         */
        template<typename... OT>
        constexpr bool operator==(const type_list_t<OT...>& other) const {
            if constexpr(sizeof...(TS) == sizeof...(OT)) {
                return (std::is_same_v<TS, OT> && ... && true);
            }
            else {
                return false;
            }
        }

        /**
         * @brief Returns if any type list in the calling type list is not in the other type list, or if any of the indices for each type in
         * both type lists is not the same
         *
         * @tparam OT The types in the other type list
         * @param other The other type list
         */
        template<typename... OT>
        constexpr bool operator!=(const type_list_t<OT...>& other) const {
            return !(*this == other);
        }
    };
    /**
     * @cond TURN_OFF_DOXYGEN
     */
    namespace detail {

        // type_list_t::index_of implementation
        template<typename T, typename... TS, size_t... IS>
        struct type_list_with_iseq<T, type_list_t<TS...>, type_list_index_of_tag, std::index_sequence<IS...>> {
            static constexpr size_t value = ((std::is_same_v<TS, T> ? IS : 0) + ... + 0);
        };

        // type_list_t::count_of implementation
        template<typename T, typename... TS, size_t... IS>
        struct type_list_with_iseq<T, type_list_t<TS...>, type_list_count_of_tag, std::index_sequence<IS...>> {
            static constexpr size_t value = ((std::is_same_v<TS, T> ? 1 : 0) + ... + 0);
        };

        // type_list_t::remove(type) implementation
        template<typename T, typename... TS, size_t... IS>
        struct type_list_with_iseq<T, type_list_t<TS...>, type_list_remove_tag, std::index_sequence<IS...>> {
            using type = decltype((type_list_t<>() + ... + std::conditional_t<std::is_same_v<T, TS>, tag_t<ignore_t>, tag_t<TS>>()));
        };

        // type_list_t::remove(index) implementation
        template<size_t S, size_t E, typename... TS, size_t... IS>
        struct type_list_with_iseq<range_t<S, E>, type_list_t<TS...>, type_list_remove_index_tag, std::index_sequence<IS...>> {
            using type = decltype((type_list_t<>() + ... + std::conditional_t<(IS >= S && IS < E), tag_t<ignore_t>, tag_t<TS>>()));
        };

        template<size_t I, template<typename> class F, typename... TS, size_t... IS>
        struct type_list_with_iseq<indexed_func_t<I, F>, type_list_t<TS...>, type_list_apply_index_tag, std::index_sequence<IS...>> {
            using type = decltype((type_list_t<>() + ... + std::conditional_t<I == IS, tag_t<typename F<TS>::type>, tag_t<TS>>()));
        };

        template<size_t S, size_t E, template<typename> class F, typename... TS, size_t... IS>
        struct type_list_with_iseq<ranged_func_t<S, E, F>, type_list_t<TS...>, type_list_apply_range_tag, std::index_sequence<IS...>> {
            // clang-format off
            using type = decltype((type_list_t<>() + ... + std::conditional_t<(IS >= S && IS < E), tag_t<typename F<TS>::type>, tag_t<TS>>()));
            // clang-format on
        };

        template<template<typename> class F, typename... TS, size_t... IS>
        struct type_list_with_iseq<func_t<F>, type_list_t<TS...>, type_list_filter_tag, std::index_sequence<IS...>> {
            using type = decltype((type_list_t<>() + ... + std::conditional_t<F<TS>::value, tag_t<TS>, tag_t<ignore_t>>()));
        };

        template<size_t S, size_t E, template<typename> class F, typename... TS, size_t... IS>
        struct type_list_with_iseq<ranged_func_t<S, E, F>, type_list_t<TS...>, type_list_filter_range_tag, std::index_sequence<IS...>> {
            using type = decltype((type_list_t<>() + ... +
                                   typename std::conditional<F<TS>::value,
                                                             tag_t<TS>,
                                                             std::conditional_t<(IS >= S && IS < E), tag_t<ignore_t>, tag_t<TS>>>::type()));
        };
    }
    /**
     * @endcond
     */

    /**
     * @relates type_list_t
     *
     * @brief Global instantiation of a type_list_t
     */
    template<typename... TS>
    constexpr type_list_t<TS...> type_list = type_list_t<TS...>();
}

#endif
