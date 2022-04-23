#pragma once

namespace ant2d {
/* Short names for C++ casts */
#define s_cast static_cast
#define r_cast reinterpret_cast
#define c_cast const_cast
#define d_cast dynamic_cast

//typedef const Slice& String;

/** @brief Helper function to iterate a std::tuple.
 @example Use it as below.

 // I have a tuple
 auto item = std::make_tuple(998, 233, "a pen");

 // I have a handler
 struct Handler
 {
 	template<typename T>
 	void operator()(const T& element)
 	{
 		cout << element << "\n";
 	}
 };

 // Em, start iteration
 Tuple::foreach(item, Handler());
 */
namespace Tuple
{
	template<typename TupleT, size_t Size>
	struct TupleHelper
	{
		template<typename Func>
		static void foreach(const TupleT& item, Func&& func)
		{
			TupleHelper<TupleT, Size - 1>::foreach(item, func);
			func(std::get<Size - 1>(item));
		}
	};
	template<typename TupleT>
	struct TupleHelper<TupleT, 0>
	{
		template<typename Func>
		static void foreach(const TupleT&, Func&&)
		{ }
	};
	template<typename TupleT, typename Func>
	inline int foreach(const TupleT& item, Func&& func)
	{
		TupleHelper<TupleT, std::tuple_size<TupleT>::value>::foreach(item, func);
		return s_cast<int>(std::tuple_size<TupleT>::value);
	}
} // namespace Tuple
}// namespace ant2d
