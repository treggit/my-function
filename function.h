//
// Created by Anton Shelepov on 2019-01-19.
//

#ifndef MY_FUNCTION_FUNCTION_H
#define MY_FUNCTION_FUNCTION_H


#include <cstddef>
#include <memory>

namespace my_function {

    template <typename>
    struct function;

    template <typename R, typename ...Args>
    class function<R(Args...)> {

    public:

        function() noexcept : ptr(nullptr), is_small(false) {};

        function(std::nullptr_t) noexcept : ptr(nullptr), is_small(false) {};

        function(function const& other) {
            if (other.is_small) {
                is_small = true;
                auto c = reinterpret_cast<concept const*>(&other.buffer);
                c->copy_to_buf(&buffer);
            } else {
                is_small = false;
                new (&buffer) std::unique_ptr<concept>(other.ptr->copy());
            }
        };

        function(function&& other) noexcept {
            move_function(std::move(other));
        }

        template <typename F>
        function(F f) {
            if (sizeof(f) <= BUFFER_SIZE && std::is_nothrow_move_constructible<F>::value) {
                is_small = true;
                new (&buffer) model<F>(std::move(f));
            } else {
                is_small = false;
                new (&buffer) std::unique_ptr<concept>(std::make_unique<model<F>>(f));
            }
        }

        ~function() {
            clear_me();
        }

        function& operator=(const function& other) {
            function tmp(other);
            swap(tmp);
            return *this;
        }

        function& operator=(function&& other) noexcept {
            clear_me();
            move_function(std::move(other));
            return *this;
        }

        void swap(function& other) noexcept {
            function tmp(std::move(other));
            other = std::move(*this);
            this = std::move(tmp);
        }

        explicit operator bool() const noexcept {
            return is_small || (ptr != nullptr);
        }

        R operator()(Args&& ... a) const {
            return ptr->call(std::forward<Args>(a)...);
        }

    private:

        void clear_me() {
            if (is_small) {
                auto c = reinterpret_cast<concept*>(&buffer);
                c->~concept();
            } else {
                auto c = reinterpret_cast<std::unique_ptr<concept>*>(&buffer);
                c->~unique_ptr();
            }
            is_small = false;
        }

        void move_function(function&& other) {
            if (other.is_small) {
                is_small = true;
                auto c = reinterpret_cast<concept*>(&other.buffer);
                c->move_to_buf(&buffer);
                c->~concept();
                other.is_small = false;
                new (&other.buffer) std::unique_ptr<concept>(nullptr);
            } else {
                is_small = false;
                new (&buffer) std::unique_ptr<concept>(std::move(other.ptr));
            }
        }

        struct concept {
            virtual std::unique_ptr<concept> copy() const = 0;
            virtual R call(Args&& ... a) const = 0;
            virtual ~concept() = default;
            virtual void copy_to_buf(void* buf) const = 0;
            virtual void move_to_buf(void* buf) noexcept = 0;
        };

        template <typename F>
        struct model : concept {
            model(F f) : f(std::move(f)) {}

            std::unique_ptr<concept> copy() const override {
                return std::make_unique<model<F>>(f);
            }

            R call(Args&& ... a) const override {
                return f(std::forward<Args>(a)...);
            }

            void copy_to_buf(void* buf) const override {
                new (&buffer) model<F>(f);
            }

            void move_to_buf(void* buf) noexcept override  {
                new (&buffer) model<F>(std::move(f));
            }

            ~model() override = default;

            F f;
        };

        static const size_t BUFFER_SIZE = 64;

        union {
            std::aligned_storage_t<BUFFER_SIZE, alignof(size_t)> buffer;
            std::unique_ptr<concept> ptr;
        };

        bool is_small;
    };
}




#endif //MY_FUNCTION_FUNCTION_H
