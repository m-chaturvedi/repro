// @ref http://stackoverflow.com/questions/32282705/a-failure-to-instantiate-function-templates-due-to-universal-forward-reference

#include <utility>
#include <iostream>

using std::cout;
using std::endl;

template<typename T>
struct is_nested : std::false_type {
    using outer_type = void;
    using inner_type = void;
};

template<template <typename> class T, typename A>
struct is_nested<T<A>> : std::true_type {
    template<typename B>
    using outer_type = T<B>;
    using inner_type = A;
};

//
// It **seems** that the templated type T<A> should
// behave the same as an bare type T with respect to
// universal references, but this is not the case.
//
template <typename T_A,
    typename Result =
    typename std::enable_if<
        is_nested<typename std::remove_reference<T_A>::type>::value,
        is_nested<typename std::remove_reference<T_A>::type> // Return info so that we can extract it
        >::type>
decltype(auto) f (T_A&& t)
{
    // Blech, but necessary
    using A = typename Result::inner_type;
    // // Cannot use template aliases directly :(
    // template<typename B>
    // using T = Result::outer_type<B>;

    cout << "Default inner: " << A() << endl;

    // Reinstantiate the class of type double
    // using T_double = typename Result::template outer_type<double>;
    // T_double c { .bar = 0.5 };
    // cout << "T_double: " << c.bar << endl;
    // Permit da forwarding
    return std::forward<T_A>(t);
}

template <typename A>
struct foo
{
    A bar;
};

int main() {
    struct foo<int>        x1 { .bar = 1 };
//     struct foo<int> const  x2 { .bar = 1 };
//     struct foo<int> &      x3 = x1;
//     struct foo<int> const& x4 = x2;

//     // all calls to `f` **fail** to compile due
//     // to **unsuccessful** binding of T&& to the required types
    auto r1 = f (x1);
//     auto r2 = f (x2);
//     auto r3 = f (x3);
//     auto r4 = f (x4);
//     auto r5 = f (foo<int> {1}); // only rvalue works
}