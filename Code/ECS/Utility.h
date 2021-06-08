#pragma once

#include <type_traits>
#include <limits>

namespace VSEngine {
namespace ECS {

template<typename ID_TYPE, typename TAG, bool isIntegral = std::is_integral<ID_TYPE>::value>
struct SId
{
    SId() = delete;
};

template<typename ID_TYPE, typename TAG>
struct SId<ID_TYPE, TAG, true>
{
private:
    using type = SId<ID_TYPE, TAG>;
public:
    SId() = delete;
    explicit SId(ID_TYPE value) : id(value) {}
    SId(const type& other) : id(other.id) {}
    SId(type&& other) : id(other.id)
    {
        // Invalidate id
        other.id = std::numeric_limits<ID_TYPE>::max();
    }

    type& operator=(const type& other)
    {
        if (this != &other)
        {
            id = other.id;
        }

        return *this;
    }

    type& operator=(type&& other)
    {
        if (this != &other)
        {
            id = other.id;
            other.id = std::numeric_limits<ID_TYPE>::max();
        }

        return *this;
    }

    explicit operator ID_TYPE()
    {
        return id;
    }

    bool operator==(const type& other)
    {
        return id == other.id;
    }

    bool operator!=(const type& other)
    {
        return id != other.id;
    }

    bool operator<(const type& other)
    {
        return id < other.id;
    }

    static type Invalid()
    {
        return type(std::numeric_limits<ID_TYPE>::max());
    }

private:
    ID_TYPE id;
};

struct EntityTag {};
using EntityId = SId<int, EntityTag>;

struct ComponentTag {};
using ComponentId = SId<int, ComponentTag>;

struct None {};

// Contains the list of types.
// PTYPE is the parent type if needs to hold the list of type derived from the common base type.
template<typename T, typename U>
class TypeList
{
    using Head = T;
    using Tail = U;
};

template<typename T>
class TypeList<T, None>
{
    using Head = T;
    using Tail = None;
};

template<typename TYPELIST, typename T>
class Append;

template<>
class Append<None, None>
{
public:
    using Result = None;
};

template<typename T>
class Append<None, T>
{
public:
    using Result = TypeList<T, None>;
};

template<typename Head, typename Tail>
class Append<None, TypeList<Head, Tail> >
{
public:
    using Result = TypeList<Head, Tail>;
};

template<typename Head, typename Tail, typename T>
class Append<TypeList<Head, Tail>, T>
{
public:
    using Result = TypeList<Head, typename Append<Tail, T>::Result>;
};

}
}