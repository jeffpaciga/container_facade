// container_facade: derive from container_facade, provide a few primatives, and your class inherits a full range interface.
//
// Unlike ranges::view_facade, container_facade is intended for ranges that own their elements. The difference is
// that the mutability of the elements owned by a container_facade-derived class is determined by the constness
// of the container, whereas for views the mutability of the elements is determined by the container being viewed.
//
// This file is almost entirely a direct copy of range-v3's view_facade.hpp, and shares the same licence:
// https://github.com/ericniebler/range-v3/blob/master/LICENSE.txt

#ifndef INCLUDED_CONTAINER_FACADE_HPP
#define INCLUDED_CONTAINER_FACADE_HPP

#include <utility>
#include <type_traits>
#include <meta/meta.hpp>
#include <range/v3/view_interface.hpp>
#include <range/v3/utility/concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/basic_iterator.hpp>

template<typename Derived, ranges::cardinality Cardinality = ranges::finite>
struct container_facade
  : ranges::view_interface<Derived, Cardinality>
{
private:
    template<typename D>
    using begin_cursor_t =
        decltype(std::declval<D &>().begin_cursor());

    template<typename D>
    using end_cursor_t =
        decltype(std::declval<D &>().end_cursor());

    template<typename D>
    using facade_iterator_t =
        ranges::basic_iterator<begin_cursor_t<D>, end_cursor_t<D>>;

    template<typename D>
    using facade_sentinel_t =
        meta::if_<
            ranges::Same<begin_cursor_t<D>, end_cursor_t<D>>,
            ranges::basic_iterator<begin_cursor_t<D>, end_cursor_t<D>>,
            ranges::basic_sentinel<end_cursor_t<D>>>;

protected:
    friend ranges::range_access;
    using ranges::view_interface<Derived, Cardinality>::derived;

    Derived begin_cursor() const
    {
        return derived();
    }
    ranges::default_sentinel end_cursor() const
    {
        return {};
    }
public:
    template<typename D = Derived, CONCEPT_REQUIRES_(ranges::Same<D, Derived>())>
    facade_iterator_t<D> begin()
    {
        return derived().begin_cursor();
    }
    template<typename D = Derived, CONCEPT_REQUIRES_(ranges::Same<D, Derived>())>
    facade_iterator_t<D const> begin() const
    {
        return derived().begin_cursor();
    }
    template<typename D = Derived, CONCEPT_REQUIRES_(ranges::Same<D, Derived>())>
    facade_sentinel_t<D> end()
    {
        return derived().end_cursor();
    }
    template<typename D = Derived, CONCEPT_REQUIRES_(ranges::Same<D, Derived>())>
    facade_sentinel_t<D const> end() const
    {
        return derived().end_cursor();
    }
};

#endif
