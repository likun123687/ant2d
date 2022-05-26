#pragma once

#include <memory>

namespace ant2d {
/** @brief Used with Composition Relationship. */
template<class Item, class Del = std::default_delete<Item>>
using Own = std::unique_ptr<Item, Del>;

template<class Item>
using OwnArray = std::unique_ptr<Item, std::default_delete<Item[]>>;

/** Useless */
template<class T>
inline Own<T> MakeOwn(T* item)
{
	return Own<T>(item);
}

template<class T, class... Args>
inline Own<T> New(Args&&... args)
{
	return Own<T>(new T(std::forward<Args>(args)...));
}

template<class T>
inline OwnArray<T> NewArray(size_t size)
{
	return OwnArray<T>(new T[size]);
}

template<class T>
inline OwnArray<T> MakeOwnArray(T* item)
{
	return OwnArray<T>(item);
}

} //namespace ant2d
