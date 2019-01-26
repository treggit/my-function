//
// Created by Anton Shelepov on 2019-01-19.
//

#include "function.h"
#include <functional>
#include <iostream>

struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_+i << '\n'; }
    int num_;
};

void print_num(int i)
{
    std::cout << i << '\n';
}

struct PrintNum {
    void operator()(int i) const
    {
        std::cout << i << '\n';
    }
};

int main()
{
    // store a free function
    my_function::function<void(int)> f_display = print_num;
    f_display(-9);

//    // store a lambda
//    my_function::function<void()> f_display_42 = []() { print_num(42); };
//    f_display_42();
//
//    // store the result of a call to std::bind
//    my_function::function<void()> f_display_31337 = std::bind(print_num, 31337);
//    f_display_31337();
//
//    // store a call to a function object
//    my_function::function<void(int)> f_display_obj = PrintNum();
//    f_display_obj(18);
}
