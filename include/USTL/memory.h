#pragma once

#include <memory>

namespace Ubpa::USTL {
    // Forward
    ////////////

    template<typename T, typename Deleter = std::default_delete<T>>
    class unique_object;
    template<typename T>
    class shared_object;
    template<typename T>
    class weak_object;

    // unique_object
    //////////////////

    template<typename T, typename Deleter>
    class unique_object {
        static_assert(!std::is_const_v<T>);

    public:
        using unique_pointer_type = std::unique_ptr<T, Deleter>;
        using pointer = typename unique_pointer_type::pointer;
        using pointer_to_const = std::add_pointer_t<const std::remove_pointer_t<pointer>>;
        using element_type = typename unique_pointer_type::element_type;
        using deleter_type = Deleter;

        // Constructor
        ////////////////

        constexpr unique_object() noexcept = default;
        constexpr unique_object(std::nullptr_t) noexcept {}
        explicit unique_object(pointer p) noexcept : ptr{ p } {}

        template<typename D = Deleter, std::enable_if_t<std::is_constructible_v<D, const D&>, int> = 0>
        unique_object(pointer p, const Deleter& d) noexcept : ptr{ p,d } {}
        template<typename D = Deleter,
            std::enable_if_t<std::conjunction_v<std::negation<std::is_reference<D>>, std::is_constructible<D, D>>, int> = 0>
        unique_object(pointer p, Deleter&& d) noexcept : ptr{ p,std::move(d) } {}
        template<typename D = Deleter,
            std::enable_if_t<std::conjunction_v<std::is_reference<D>, std::is_constructible<D, std::remove_reference_t<D>>>, int> = 0>
        unique_object(pointer p, std::remove_reference_t<Deleter>&& d) = delete;

        unique_object(unique_object&& obj) noexcept : ptr{ std::move(obj.ptr) } {}
        unique_object(unique_pointer_type&& ptr) noexcept : ptr{ std::move(ptr) } {}

        template<typename U, typename E>
        unique_object(unique_object<U, E>&& obj) noexcept : ptr{ std::move(obj.ptr) } {}
        template<typename U, typename E>
        unique_object(std::unique_ptr<U, E>&& ptr) noexcept : ptr{ std::move(ptr) } {}

        // array-version

        template<typename U,
            typename V = T, std::enable_if_t<std::is_array_v<V>, int> = 0>
        explicit unique_object(U p) noexcept : ptr{ p } {}

        template<typename U,
            typename D = Deleter, std::enable_if_t<std::is_constructible_v<D, const D&>, int> = 0,
            typename V = T, std::enable_if_t<std::is_array_v<V>, int> = 0>
        unique_object(U p, const Deleter& d) noexcept : ptr{ p,d } {}

        template<typename U,
            typename D = Deleter, std::enable_if_t<std::conjunction_v<std::negation<std::is_reference<D>>, std::is_constructible<D, D>>, int> = 0,
            typename V = T, std::enable_if_t<std::is_array_v<V>, int> = 0>
        unique_object(U p, Deleter&& d) noexcept : ptr{ p,std::move(d) } {}

        template<typename U,
            typename D = Deleter, std::enable_if_t<std::conjunction_v<std::is_reference<D>, std::is_constructible<D, std::remove_reference_t<D>>>, int> = 0,
            typename V = T, std::enable_if_t<std::is_array_v<V>, int> = 0>
        unique_object(U p, std::remove_reference_t<Deleter>&& d) = delete;

        // Assign
        ///////////

        unique_object& operator=(unique_object&& r) noexcept {
            ptr = std::move(r.ptr);
            return *this;
        }

        template<typename U, typename E>
        unique_object& operator=(unique_object<U, E>&& r) noexcept {
            ptr = std::move(r.ptr);
            return *this;
        }

        unique_object& operator=(unique_pointer_type&& r) noexcept {
            ptr = std::move(r);
            return *this;
        }

        template<typename U, typename E>
        unique_object& operator=(std::unique_ptr<U, E>&& r) noexcept {
            ptr = std::move(r);
            return *this;
        }

        unique_object& operator=(std::nullptr_t) noexcept {
            reset();
            return *this;
        }

        // Modifiers
        //////////////

        pointer          release() noexcept { return ptr.release(); }
        pointer_to_const release() const noexcept { return ptr.release(); }

        template<typename U = T, std::enable_if_t<!std::is_array_v<U>, int> = 0>
        void reset(pointer p = pointer{}) noexcept { ptr.reset(p); }
        template<typename U = T, std::enable_if_t<std::is_array_v<U>, int> = 0>
        void reset(U p) noexcept { ptr.reset(p); }
        template<typename U = T, std::enable_if_t<std::is_array_v<U>, int> = 0>
        void reset(std::nullptr_t p = nullptr) noexcept { ptr.reset(p); }

        void swap(unique_object& rhs) noexcept { ptr.swap(rhs.ptr); }
        void swap(unique_pointer_type& rhs) noexcept { ptr.swap(rhs); }

        // Observers
        //////////////

        pointer          get() noexcept { return ptr.get(); }
        pointer_to_const get() const noexcept { return ptr.get(); }

        Deleter&       get_deleter() noexcept { return ptr.get_deleter(); }
        const Deleter& get_deleter() const noexcept { return ptr.get_deleter(); }

        explicit operator bool() const noexcept { return static_cast<bool>(ptr); }

        template <typename U = T, std::enable_if_t<!std::disjunction_v<std::is_array<U>, std::is_void<U>>, int> = 0>
        U&       operator*() & noexcept { return *ptr; }
        template <typename U = T, std::enable_if_t<!std::disjunction_v<std::is_array<U>, std::is_void<U>>, int> = 0>
        U        operator*() && noexcept { return std::move(*ptr); }
        template <typename U = T, std::enable_if_t<!std::disjunction_v<std::is_array<U>, std::is_void<U>>, int> = 0>
        const U& operator*() const& noexcept { return *ptr; }

        template <typename U = T, typename Elem = element_type, std::enable_if_t<std::is_array_v<U>, int> = 0>
        pointer          operator->() noexcept { return ptr.operator->(); }
        template <typename U = T, typename Elem = element_type, std::enable_if_t<std::is_array_v<U>, int> = 0>
        pointer_to_const operator->() const noexcept { return ptr.operator->(); }

        template <typename U = T, typename Elem = element_type, std::enable_if_t<std::is_array_v<U>, int> = 0>
        Elem&       operator[](std::ptrdiff_t idx) noexcept { return ptr[idx]; }
        template <typename U = T, typename Elem = element_type, std::enable_if_t<std::is_array_v<U>, int> = 0>
        const Elem& operator[](std::ptrdiff_t idx) const noexcept { return ptr[idx]; }

    private:
        std::unique_ptr<T, Deleter> ptr;
    };

    // shared_object
    //////////////////

	template<typename T>
	class shared_object {
        static_assert(!std::is_const_v<T>);

    public:
        //using shared_object_type = shared_object<T>;
        using weak_object_type = weak_object<T>;
        using shared_pointer_type = std::shared_ptr<T>;
        using weak_pointer_type = std::weak_ptr<T>;
        using unique_pointer_type = std::unique_ptr<T>;
        using element_type = typename shared_pointer_type::element_type;

        // Constructor
        ////////////////

        constexpr shared_object() noexcept = default;
        constexpr shared_object(std::nullptr_t) noexcept {}
        template<typename U>
        explicit shared_object(U* ptr) : ptr{ ptr } {}
        template<typename U, typename Deleter>
        shared_object(U* ptr, Deleter d) : ptr{ ptr, std::move(d) } {}
        template<typename Deleter>
        shared_object(std::nullptr_t ptr, Deleter d) : ptr{ ptr, std::move(d) } {}
        template<typename U, typename Deleter, typename Alloc>
        shared_object(U* ptr, Deleter d, Alloc alloc) : ptr{ ptr, std::move(d), std::move(alloc) } {}
        template<typename Deleter, typename Alloc>
        shared_object(std::nullptr_t ptr, Deleter d, Alloc alloc) : ptr{ ptr, std::move(d), std::move(alloc) } {}

        template<typename U>
        explicit shared_object(const std::weak_ptr<U>& ptr) : ptr{ ptr } {}
        template<typename U>
        explicit shared_object(weak_object<U>& obj) : ptr{ obj.ptr } {}

        template<typename Y, typename Deleter>
        shared_object(std::unique_ptr<Y, Deleter>&& r) : ptr{ std::move(r) } {}
        template<typename Y, typename Deleter>
        shared_object(unique_object<Y, Deleter>&& obj) : ptr{ std::move(obj.ptr) } {}

        shared_object(const shared_pointer_type& ptr) noexcept : ptr{ ptr } {}
        shared_object(shared_pointer_type&& ptr) noexcept : ptr{ std::move(ptr) } {}
        template<typename U>
        shared_object(const std::shared_ptr<U>& ptr) noexcept : ptr{ ptr } {}
        template<typename U>
        shared_object(std::shared_ptr<U>&& ptr) noexcept : ptr{ std::move(ptr) } {}
        template<typename U>
        shared_object(const std::shared_ptr<U>& r, std::remove_extent_t<T>* ptr) noexcept : ptr{ r,ptr } {}

        shared_object(shared_object& obj) noexcept : ptr{ obj.ptr } {}
        shared_object(shared_object&& obj) noexcept : ptr{ std::move(obj.ptr) } {}
        template<typename U>
        shared_object(shared_object<U>& obj) noexcept : ptr{ obj.ptr } {}
        template<typename U>
        shared_object(shared_object<U>&& obj) noexcept : ptr{ std::move(obj.ptr) } {}
        template<typename U>
        shared_object(shared_object<U>& r, std::remove_extent_t<T>* ptr) noexcept : ptr{ r.ptr,ptr } {}

        // Assign
        ///////////

        shared_object& operator=(shared_object& rhs) noexcept {
            ptr = rhs.ptr;
            return *this;
        }

        template <typename U>
        shared_object& operator=(shared_object<U>& rhs) noexcept {
            ptr = rhs.ptr;
            return *this;
        }

        shared_object& operator=(shared_object&& rhs) noexcept {
            ptr = std::move(rhs.ptr);
            return *this;
        }

        template <typename U>
        shared_object& operator=(shared_object<U>&& rhs) noexcept {
            ptr = std::move(rhs.ptr);
            return *this;
        }

        template <typename U, typename Deleter>
        shared_object& operator=(unique_object<U, Deleter>&& rhs) {
            ptr = std::move(rhs.ptr);
            return *this;
        }

        shared_object& operator=(const shared_pointer_type& rhs) noexcept {
            ptr = rhs;
            return *this;
        }

        template <typename U>
        shared_object& operator=(const std::shared_ptr<U>& rhs) noexcept {
            ptr = rhs;
            return *this;
        }

        shared_object& operator=(shared_pointer_type&& rhs) noexcept {
            ptr = std::move(rhs);
            return *this;
        }

        template <typename U>
        shared_object& operator=(std::shared_ptr<U>&& rhs) noexcept {
            ptr = std::move(rhs);
            return *this;
        }

        template <typename U, typename Deleter>
        shared_object& operator=(std::unique_ptr<U, Deleter>&& rhs) {
            ptr = std::move(rhs);
            return *this;
        }

        shared_object& operator=(std::nullptr_t) noexcept {
            reset();
            return *this;
        }

        // Cast
        /////////

        shared_pointer_type&     cast_to_shared_ptr() & noexcept { return ptr; }
        shared_pointer_type      cast_to_shared_ptr() && noexcept { return std::move(ptr); }
        std::shared_ptr<const T> cast_to_shared_ptr() const& noexcept { return std::static_pointer_cast<const T>(ptr); }

        operator shared_pointer_type&    () & noexcept { return ptr; }
        operator shared_pointer_type     () && noexcept { return std::move(ptr); }
        operator std::shared_ptr<const T>() const& noexcept { return ptr; }

        // Modifiers
        //////////////

        void reset() noexcept { ptr.reset(); }
        template<typename U>
        void reset(U* ptrU) { ptr.reset(ptrU); }
        template<typename U, typename Deleter>
        void reset(U* ptrU, Deleter d) { ptr.reset(ptrU, std::move(d)); }
        template<typename U, typename Deleter, typename Alloc>
        void reset(U* ptrU, Deleter d, Alloc alloc) { ptr.reset(ptrU, std::move(d), std::move(alloc)); }

        void swap(shared_object& rhs) noexcept { return ptr.swap(rhs.ptr); }
        void swap(shared_pointer_type& rhs) noexcept { return ptr.swap(rhs); }

        // Observers
        //////////////

        T*       get() noexcept { return ptr.get(); }
        const T* get() const noexcept { return ptr.get(); }

        long use_count() const noexcept{ return ptr.use_count(); }

        template <typename U = T, std::enable_if_t<!std::disjunction_v<std::is_array<U>, std::is_void<U>>, int> = 0>
        U&       operator*() noexcept { return ptr.operator*(); }
        template <typename U = T, std::enable_if_t<!std::disjunction_v<std::is_array<U>, std::is_void<U>>, int> = 0>
        const U& operator*() const noexcept { return ptr.operator*(); }

        template <typename U = T, std::enable_if_t<!std::is_array_v<U>, int> = 0>
        U*       operator->() noexcept { return ptr.operator->(); }
        template <typename U = T, std::enable_if_t<!std::is_array_v<U>, int> = 0>
        const U* operator->() const noexcept { return ptr.operator->(); }

        template <typename U = T, typename Elem = std::remove_extent_t<T>, std::enable_if_t<std::is_array_v<U>, int> = 0>
        Elem&       operator[](std::ptrdiff_t idx) noexcept { return ptr[idx]; }
        template <typename U = T, typename Elem = std::remove_extent_t<T>, std::enable_if_t<std::is_array_v<U>, int> = 0>
        const Elem& operator[](std::ptrdiff_t idx) const noexcept { return ptr[idx]; }

        explicit operator bool() const noexcept { return static_cast<bool>(ptr); }

        template <typename U>
        bool owner_before(const shared_object<U>& rhs) const noexcept { return ptr.owner_before(rhs.ptr); }
        template <typename U>
        bool owner_before(const std::shared_ptr<U>& rhs) const noexcept { return ptr.owner_before(rhs); }
        template <typename U>
        bool owner_before(const weak_object<U>& rhs) const noexcept { return ptr.owner_before(rhs.ptr); }
        template <typename U>
        bool owner_before(const std::weak_ptr<U>& rhs) const noexcept { return ptr.owner_before(rhs); }

        template <typename U>
        bool owner_after(const shared_object<U>& rhs) const noexcept { return rhs.ptr.owner_before(ptr); }
        template <typename U>
        bool owner_after(const std::shared_ptr<U>& rhs) const noexcept { return rhs.owner_before(ptr); }
        template <typename U>
        bool owner_after(const weak_object<U>& rhs) const noexcept { return rhs.ptr.owner_before(ptr); }
        template <typename U>
        bool owner_after(const std::weak_ptr<U>& rhs) const noexcept { return rhs.owner_before(ptr); }

    private:
        shared_pointer_type ptr;
	};

    // weak_object
    //////////////////

    template<typename T>
    class weak_object {
        static_assert(!std::is_const_v<T>);

    public:
        using shared_object_type = shared_object<T>;
        //using weak_object_type = weak_object<T>;
        using shared_pointer_type = std::shared_ptr<T>;
        using weak_pointer_type = std::weak_ptr<T>;
        using unique_pointer_type = std::unique_ptr<T>;
        using element_type = typename weak_pointer_type::element_type;

        // Constructor
        ////////////////

        constexpr weak_object() noexcept = default;

        weak_object(weak_object& obj) noexcept : ptr{ obj.ptr } {}
        weak_object(weak_object&& obj) noexcept : ptr{ std::move(obj.ptr) } {}
        template<typename U>
        weak_object(weak_object<U>& obj) noexcept : ptr{ obj.ptr } {}
        template<typename U>
        weak_object(weak_object<U>&& obj) noexcept : ptr{ std::move(obj.ptr) } {}

        weak_object(const weak_pointer_type& ptr) noexcept : ptr{ ptr } {}
        weak_object(weak_pointer_type&& ptr) noexcept : ptr{ std::move(ptr) } {}
        template<typename U>
        weak_object(const std::weak_ptr<U>& ptr) noexcept : ptr{ ptr } {}
        template<typename U>
        weak_object(std::weak_ptr<U>&& ptr) noexcept : ptr{ std::move(ptr) } {}

        template<typename U>
        weak_object(shared_object<U>& obj) noexcept : ptr{ obj.ptr } {}
        template<typename U>
        weak_object(const std::shared_ptr<U>& ptr) noexcept : ptr{ ptr } {}

        // Assign
        ///////////

        weak_object& operator=(weak_object& rhs) noexcept {
            ptr = rhs.ptr;
            return *this;
        }

        template <typename U>
        weak_object& operator=(weak_object<U>& rhs) noexcept {
            ptr = rhs.ptr;
            return *this;
        }

        weak_object& operator=(weak_object&& rhs) noexcept {
            ptr = std::move(rhs.ptr);
            return *this;
        }

        template <typename U>
        weak_object& operator=(weak_object<U>&& rhs) noexcept {
            ptr = std::move(rhs.ptr);
            return *this;
        }

        template <typename U>
        weak_object& operator=(shared_object<U>& rhs) noexcept {
            ptr = rhs.ptr;
            return *this;
        }

        weak_object& operator=(const weak_pointer_type& rhs) noexcept {
            ptr = rhs;
            return *this;
        }

        template <typename U>
        weak_object& operator=(const std::weak_ptr<U>& rhs) noexcept {
            ptr = rhs;
            return *this;
        }

        weak_object& operator=(weak_pointer_type&& rhs) noexcept {
            ptr = std::move(rhs);
            return *this;
        }

        template <typename U>
        weak_object& operator=(std::weak_ptr<U>&& rhs) noexcept {
            ptr = std::move(rhs);
            return *this;
        }

        template <typename U>
        weak_object& operator=(const std::shared_ptr<U>& rhs) noexcept {
            ptr = rhs;
            return *this;
        }

        // Cast
        /////////

        weak_pointer_type&     cast_to_weak_ptr() & noexcept { return ptr; }
        weak_pointer_type      cast_to_weak_ptr() && noexcept { return std::move(ptr); }
        std::weak_ptr<const T> cast_to_weak_ptr() const& noexcept { return std::static_pointer_cast<const T>(ptr); }

        operator weak_pointer_type& () & noexcept { return ptr; }
        operator weak_pointer_type () && noexcept { return std::move(ptr); }
        operator std::weak_ptr<const T>() const& noexcept { return ptr; }

        // Modifiers
        //////////////

        void reset() noexcept { ptr.reset(); }

        void swap(weak_object& rhs) noexcept { return ptr.swap(rhs.ptr); }
        void swap(weak_pointer_type& rhs) noexcept { return ptr.swap(rhs); }

        // Observers
        //////////////

        long use_count() const noexcept { return ptr.use_count(); }

        bool expired() const noexcept { return ptr.expired(); }

        shared_object_type       lock() noexcept { return { ptr.lock() }; }
        const shared_object_type lock() const noexcept { return { ptr.lock() }; }
        shared_pointer_type      lock_to_shared_ptr() noexcept { return ptr.lock(); }
        std::shared_ptr<const T> lock_to_shared_ptr() const noexcept { return std::static_pointer_cast<const T>(ptr.lock()); }

        template <typename U>
        bool owner_before(const shared_object<U>& rhs) const noexcept { return ptr.owner_before(rhs.ptr); }
        template <typename U>
        bool owner_before(const std::shared_ptr<U>& rhs) const noexcept { return ptr.owner_before(rhs); }
        template <typename U>
        bool owner_before(const weak_object<U>& rhs) const noexcept { return ptr.owner_before(rhs.ptr); }
        template <typename U>
        bool owner_before(const std::weak_ptr<U>& rhs) const noexcept { return ptr.owner_before(rhs); }

        template <typename U>
        bool owner_after(const shared_object<U>& rhs) const noexcept { return rhs.ptr.owner_before(ptr); }
        template <typename U>
        bool owner_after(const std::shared_ptr<U>& rhs) const noexcept { return rhs.owner_before(ptr); }
        template <typename U>
        bool owner_after(const weak_object<U>& rhs) const noexcept { return rhs.ptr.owner_before(ptr); }
        template <typename U>
        bool owner_after(const std::weak_ptr<U>& rhs) const noexcept { return rhs.owner_before(ptr); }

    private:
        weak_pointer_type ptr;
    };

    // make object
    ////////////////

    template <typename T, class... Args>
    shared_object<T> make_shared_object(Args&&... args) {
        return { std::make_shared<T>(std::forward<Args>(args)...) };
    }

    template <typename T, std::enable_if_t<std::is_array_v<T>, int> = 0>
    shared_object<T> make_shared_object(std::size_t size) {
        return { std::make_shared<T>(size) };
    }

    template <typename T, class Alloc, class... Args>
    shared_object<T> allocate_shared_object(const Alloc& alloc, Args&&... args) {
        return { std::allocate_shared<T>(alloc, std::forward<Args>(args)...) };
    }

    template <typename T, class... Args>
    unique_object<T> make_unique_object(Args&&... args) {
        return { std::make_unique<T>(std::forward<Args>(args)...) };
    }

    template <typename T, std::enable_if_t<std::is_array_v<T>, int> = 0>
    unique_object<T> make_unique_object(std::size_t size) {
        return { std::make_unique<T>(size) };
    }

    // cast
    /////////

    template<typename Ty1, typename Ty2>
    shared_object<Ty1> static_object_cast(shared_object<Ty2>&& other) noexcept {
        return { std::static_pointer_cast<Ty1>(std::move(other).cast_to_shared_ptr()) };
    }

    template<typename Ty1, typename Ty2>
    shared_object<Ty1> static_object_cast(shared_object<Ty2>& other) noexcept {
        return { std::static_pointer_cast<Ty1>(other.cast_to_shared_ptr()) };
    }

    template<typename Ty1, typename Ty2>
    const shared_object<Ty1> static_object_cast(const shared_object<Ty2>& other) noexcept {
        return { static_object_cast<Ty1>(const_cast<shared_object<Ty2>&>(other)) };
    }

    template<typename Ty1, typename Ty2>
    shared_object<Ty1> dynamic_object_cast(shared_object<Ty2>&& other) noexcept {
        return { std::dynamic_pointer_cast<Ty1>(std::move(other.cast_to_shared_ptr())) };
    }

    template<typename Ty1, typename Ty2>
    shared_object<Ty1> dynamic_object_cast(shared_object<Ty2>& other) noexcept {
        return { std::dynamic_pointer_cast<Ty1>(other.cast_to_shared_ptr()) };
    }

    template<typename Ty1, typename Ty2>
    const shared_object<Ty1> dynamic_object_cast(const shared_object<Ty2>& other) noexcept {
        return { dynamic_object_cast<Ty1>(const_cast<shared_object<Ty2>&>(other)) };
    }

    template<typename Ty1, typename Ty2>
    shared_object<Ty1> reinterpret_object_cast(shared_object<Ty2>&& other) noexcept {
        return { std::reinterpret_pointer_cast<Ty1>(std::move(other.cast_to_shared_ptr())) };
    }

    template<typename Ty1, typename Ty2>
    shared_object<Ty1> reinterpret_object_cast(shared_object<Ty2>& other) noexcept {
        return { std::reinterpret_pointer_cast<Ty1>(other.cast_to_shared_ptr()) };
    }

    template<typename Ty1, typename Ty2>
    const shared_object<Ty1> reinterpret_object_cast(const shared_object<Ty2>& other) noexcept {
        return { reinterpret_object_cast<Ty1>(const_cast<shared_object<Ty2>&>(other)) };
    }

    // Deduction Guides
    /////////////////////

    template<typename T>
    shared_object(weak_object<T>)->shared_object<T>;
    template<typename T>
    shared_object(std::weak_ptr<T>)->shared_object<T>;
    template<typename T, typename Deleter>
    shared_object(unique_object<T, Deleter>)->shared_object<T>;
    template<typename T, typename Deleter>
    shared_object(std::unique_ptr<T, Deleter>)->shared_object<T>;

    template<typename T>
    weak_object(shared_object<T>)->weak_object<T>;
    template<typename T>
    weak_object(std::shared_ptr<T>)->weak_object<T>;
}

// Hash
/////////

template<typename T>
struct std::hash<Ubpa::USTL::shared_object<T>> {
    constexpr std::size_t operator()(const Ubpa::USTL::shared_object<T>& obj) noexcept {
        return std::hash<typename std::shared_ptr<T>::element_type*>()(obj.get());
    }
};

template<typename T, typename Deleter>
struct std::hash<Ubpa::USTL::unique_object<T, Deleter>> {
    constexpr std::size_t operator()(const Ubpa::USTL::unique_object<T, Deleter>& obj) noexcept {
        return std::hash<typename std::unique_ptr<T, Deleter>::pointer>()(obj.get());
    }
};

// Compare
////////////

template<typename Ty1, typename Ty2>
bool operator==(const Ubpa::USTL::shared_object<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() == right.get();
}

template<typename Ty1, typename Ty2>
bool operator!=(const Ubpa::USTL::shared_object<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() != right.get();
}

template<typename Ty1, typename Ty2>
bool operator<(const Ubpa::USTL::shared_object<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() < right.get();
}

template<typename Ty1, typename Ty2>
bool operator>=(const Ubpa::USTL::shared_object<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() >= right.get();
}

template<typename Ty1, typename Ty2>
bool operator>(const Ubpa::USTL::shared_object<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() > right.get();
}

template<typename Ty1, typename Ty2>
bool operator<=(const Ubpa::USTL::shared_object<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() <= right.get();
}

template<typename Ty1, typename Ty2>
bool operator==(const std::shared_ptr<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() == right.get();
}

template<typename Ty1, typename Ty2>
bool operator!=(const std::shared_ptr<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() != right.get();
}

template<typename Ty1, typename Ty2>
bool operator<(const std::shared_ptr<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() < right.get();
}

template<typename Ty1, typename Ty2>
bool operator>=(const std::shared_ptr<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() >= right.get();
}

template<typename Ty1, typename Ty2>
bool operator>(const std::shared_ptr<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() > right.get();
}

template<typename Ty1, typename Ty2>
bool operator<=(const std::shared_ptr<Ty1>& left, const Ubpa::USTL::shared_object<Ty2>& right) noexcept {
    return left.get() <= right.get();
}

template<typename Ty1, typename Ty2>
bool operator==(const Ubpa::USTL::shared_object<Ty1>& left, const std::shared_ptr<Ty2>& right) noexcept {
    return left.get() == right.get();
}

template<typename Ty1, typename Ty2>
bool operator!=(const Ubpa::USTL::shared_object<Ty1>& left, const std::shared_ptr<Ty2>& right) noexcept {
    return left.get() != right.get();
}

template<typename Ty1, typename Ty2>
bool operator<(const Ubpa::USTL::shared_object<Ty1>& left, const std::shared_ptr<Ty2>& right) noexcept {
    return left.get() < right.get();
}

template<typename Ty1, typename Ty2>
bool operator>=(const Ubpa::USTL::shared_object<Ty1>& left, const std::shared_ptr<Ty2>& right) noexcept {
    return left.get() >= right.get();
}

template<typename Ty1, typename Ty2>
bool operator>(const Ubpa::USTL::shared_object<Ty1>& left, const std::shared_ptr<Ty2>& right) noexcept {
    return left.get() > right.get();
}

template<typename Ty1, typename Ty2>
bool operator<=(const Ubpa::USTL::shared_object<Ty1>& left, const std::shared_ptr<Ty2>& right) noexcept {
    return left.get() <= right.get();
}

template <typename T>
bool operator==(const Ubpa::USTL::shared_object<T>& left, std::nullptr_t) noexcept {
    return left.get() == nullptr;
}

template <typename T>
bool operator==(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return nullptr == right.get();
}

template <typename T>
bool operator!=(const Ubpa::USTL::shared_object<T>& left, std::nullptr_t) noexcept {
    return left.get() != nullptr;
}

template <typename T>
bool operator!=(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return nullptr != right.get();
}

template <typename T>
bool operator<(const Ubpa::USTL::shared_object<T>& left, std::nullptr_t) noexcept {
    return left.get() < static_cast<typename Ubpa::USTL::shared_object<T>::element_type*>(nullptr);
}

template <typename T>
bool operator<(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return static_cast<typename Ubpa::USTL::shared_object<T>::element_type*>(nullptr) < right.get();
}

template <typename T>
bool operator>=(const Ubpa::USTL::shared_object<T>& left, std::nullptr_t) noexcept {
    return left.get() >= static_cast<typename Ubpa::USTL::shared_object<T>::element_type*>(nullptr);
}

template <typename T>
bool operator>=(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return static_cast<typename Ubpa::USTL::shared_object<T>::element_type*>(nullptr) >= right.get();
}

template <typename T>
bool operator>(const Ubpa::USTL::shared_object<T>& left, std::nullptr_t) noexcept {
    return left.get() > static_cast<typename Ubpa::USTL::shared_object<T>::element_type*>(nullptr);
}

template <typename T>
bool operator>(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return static_cast<typename Ubpa::USTL::shared_object<T>::element_type*>(nullptr) > right.get();
}

template <typename T>
bool operator<=(const Ubpa::USTL::shared_object<T>& left, std::nullptr_t) noexcept {
    return left.get() <= static_cast<typename Ubpa::USTL::shared_object<T>::element_type*>(nullptr);
}

template <typename T>
bool operator<=(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return static_cast<typename Ubpa::USTL::shared_object<T>::element_type*>(nullptr) <= right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator==(const Ubpa::USTL::unique_object<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() == right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator!=(const Ubpa::USTL::unique_object<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() != right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator<(const Ubpa::USTL::unique_object<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() < right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator>=(const Ubpa::USTL::unique_object<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() >= right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator>(const Ubpa::USTL::unique_object<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() > right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator<=(const Ubpa::USTL::unique_object<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() <= right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator==(const std::unique_ptr<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() == right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator!=(const std::unique_ptr<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() != right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator<(const std::unique_ptr<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() < right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator>=(const std::unique_ptr<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() >= right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator>(const std::unique_ptr<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() > right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator<=(const std::unique_ptr<Ty1, D1>& left, const Ubpa::USTL::unique_object<Ty2, D2>& right) noexcept {
    return left.get() <= right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator==(const Ubpa::USTL::unique_object<Ty1, D1>& left, const std::unique_ptr<Ty2, D2>& right) noexcept {
    return left.get() == right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator!=(const Ubpa::USTL::unique_object<Ty1, D1>& left, const std::unique_ptr<Ty2, D2>& right) noexcept {
    return left.get() != right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator<(const Ubpa::USTL::unique_object<Ty1, D1>& left, const std::unique_ptr<Ty2, D2>& right) noexcept {
    return left.get() < right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator>=(const Ubpa::USTL::unique_object<Ty1, D1>& left, const std::unique_ptr<Ty2, D2>& right) noexcept {
    return left.get() >= right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator>(const Ubpa::USTL::unique_object<Ty1, D1>& left, const std::unique_ptr<Ty2, D2>& right) noexcept {
    return left.get() > right.get();
}

template<typename Ty1, typename D1, typename Ty2, typename D2>
bool operator<=(const Ubpa::USTL::unique_object<Ty1, D1>& left, const std::unique_ptr<Ty2, D2>& right) noexcept {
    return left.get() <= right.get();
}

template <typename T, typename D>
bool operator==(const Ubpa::USTL::unique_object<T, D>& left, std::nullptr_t) noexcept {
    return left.get() == nullptr;
}

template <typename T, typename D>
bool operator==(std::nullptr_t, const Ubpa::USTL::unique_object<T, D>& right) noexcept {
    return nullptr == right.get();
}

template <typename T, typename D>
bool operator!=(const Ubpa::USTL::unique_object<T, D>& left, std::nullptr_t) noexcept {
    return left.get() != nullptr;
}

template <typename T, typename D>
bool operator!=(std::nullptr_t, const Ubpa::USTL::unique_object<T, D>& right) noexcept {
    return nullptr != right.get();
}

template <typename T, typename D>
bool operator<(const Ubpa::USTL::unique_object<T, D>& left, std::nullptr_t) noexcept {
    return left.get() < static_cast<typename Ubpa::USTL::unique_object<T, D>::pointer>(nullptr);
}

template <typename T, typename D>
bool operator<(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return static_cast<typename Ubpa::USTL::unique_object<T, D>::pointer>(nullptr) < right.get();
}

template <typename T, typename D>
bool operator>=(const Ubpa::USTL::shared_object<T>& left, std::nullptr_t) noexcept {
    return left.get() >= static_cast<typename Ubpa::USTL::unique_object<T, D>::pointer>(nullptr);
}

template <typename T, typename D>
bool operator>=(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return static_cast<typename Ubpa::USTL::unique_object<T, D>::pointer>(nullptr) >= right.get();
}

template <typename T, typename D>
bool operator>(const Ubpa::USTL::shared_object<T>& left, std::nullptr_t) noexcept {
    return left.get() > static_cast<typename Ubpa::USTL::unique_object<T, D>::pointer>(nullptr);
}

template <typename T, typename D>
bool operator>(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return static_cast<typename Ubpa::USTL::unique_object<T, D>::pointer>(nullptr) > right.get();
}

template <typename T, typename D>
bool operator<=(const Ubpa::USTL::shared_object<T>& left, std::nullptr_t) noexcept {
    return left.get() <= static_cast<typename Ubpa::USTL::unique_object<T, D>::pointer>(nullptr);
}

template <typename T, typename D>
bool operator<=(std::nullptr_t, const Ubpa::USTL::shared_object<T>& right) noexcept {
    return static_cast<typename Ubpa::USTL::unique_object<T, D>::pointer>(nullptr) <= right.get();
}

// Output
///////////

template <class Elem, typename Traits, typename T>
std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& out, const Ubpa::USTL::shared_object<T>& obj) {
    return out << obj.get();
}

// Swap
/////////

namespace std {
    template <typename T>
    void swap(Ubpa::USTL::shared_object<T>& left, Ubpa::USTL::shared_object<T>& right) noexcept {
        left.swap(right);
    }

    template <typename T>
    void swap(Ubpa::USTL::shared_object<T>& left, shared_ptr<T>& right) noexcept {
        left.swap(right);
    }

    template <typename T>
    void swap(shared_ptr<T>& left, Ubpa::USTL::shared_object<T>& right) noexcept {
        right.swap(left);
    }

    template <typename T>
    void swap(Ubpa::USTL::weak_object<T>& left, Ubpa::USTL::weak_object<T>& right) noexcept {
        left.swap(right);
    }

    template <typename T>
    void swap(Ubpa::USTL::weak_object<T>& left, weak_ptr<T>& right) noexcept {
        left.swap(right);
    }

    template <typename T>
    void swap(weak_ptr<T>& left, Ubpa::USTL::weak_object<T>& right) noexcept {
        right.swap(left);
    }

    template <typename T, typename Deleter>
    void swap(Ubpa::USTL::unique_object<T, Deleter>& left, Ubpa::USTL::unique_object<T, Deleter>& right) noexcept {
        left.swap(right);
    }

    template <typename T, typename Deleter>
    void swap(Ubpa::USTL::unique_object<T, Deleter>& left, unique_ptr<T, Deleter>& right) noexcept {
        left.swap(right);
    }

    template <typename T, typename Deleter>
    void swap(unique_ptr<T, Deleter>& left, Ubpa::USTL::unique_object<T, Deleter>& right) noexcept {
        right.swap(left);
    }
}

// owner_less
///////////////

template<typename T>
struct std::owner_less<Ubpa::USTL::shared_object<T>> {
    bool operator()(const Ubpa::USTL::shared_object<T>& left, const Ubpa::USTL::shared_object<T>& right) const noexcept {
        return left.owner_before(right);
    }

    bool operator()(const Ubpa::USTL::shared_object<T>& left, const std::shared_ptr<T>& right) const noexcept {
        return left.owner_before(right);
    }

    bool operator()(const std::shared_ptr<T>& left, const Ubpa::USTL::shared_object<T>& right) const noexcept {
        return right.owner_after(left);
    }

    bool operator()(const Ubpa::USTL::shared_object<T>& left, const Ubpa::USTL::weak_object<T>& right) const noexcept {
        return left.owner_before(right);
    }

    bool operator()(const Ubpa::USTL::weak_object<T>& left, const Ubpa::USTL::shared_object<T>& right) const noexcept {
        return left.owner_before(right);
    }

    bool operator()(const Ubpa::USTL::shared_object<T>& left, const std::weak_ptr<T>& right) const noexcept {
        return left.owner_before(right);
    }

    bool operator()(const std::weak_ptr<T>& left, const Ubpa::USTL::shared_object<T>& right) const noexcept {
        return right.owner_after(right);
    }
};

template<typename T>
struct std::owner_less<Ubpa::USTL::weak_object<T>> {
    bool operator()(const Ubpa::USTL::weak_object<T>& left, const Ubpa::USTL::weak_object<T>& right) const noexcept {
        return left.owner_before(right);
    }

    bool operator()(const Ubpa::USTL::weak_object<T>& left, const std::weak_ptr<T>& right) const noexcept {
        return left.owner_before(right);
    }

    bool operator()(const std::weak_ptr<T>& left, const Ubpa::USTL::weak_object<T>& right) const noexcept {
        return right.owner_after(left);
    }
};
