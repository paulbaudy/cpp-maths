/*!
 *  @file CallTraits.h
 *  @author Paul
 *  @date 2024-11-22
 *
 *  Call traits helpers
 */

#include <type_traits>
#include "AndOr.h"

namespace Maths
{
	template <typename T, bool TypeIsSmall>
	struct TCallTraitsParamTypeHelper
	{
		typedef const T& ParamType;
		typedef const T& ConstParamType;
	};
	template <typename T>
	struct TCallTraitsParamTypeHelper<T, true>
	{
		typedef const T ParamType;
		typedef const T ConstParamType;
	};
	template <typename T>
	struct TCallTraitsParamTypeHelper<T*, true>
	{
		typedef T* ParamType;
		typedef const T* ConstParamType;
	};

	/**
	 * base class for call traits. Used to more easily refine portions when specializing
	 */
	template <typename T>
	struct TCallTraitsBase
	{
	private:
		enum { PassByValue = true/**= Maths::TOr<Maths::TAnd<(sizeof(T) <= sizeof(void*)), std::is_pod_v<T>>, std::is_arithmetic_v<T>> */ };

	public:
		typedef T ValueType;
		typedef T& Reference;
		typedef const T& ConstReference;
		typedef typename TCallTraitsParamTypeHelper<T, PassByValue>::ParamType ParamType;
		typedef typename TCallTraitsParamTypeHelper<T, PassByValue>::ConstParamType ConstPointerType;
	};

	/**
	 * TCallTraits
	 */
	template <typename T>
	struct TCallTraits : public TCallTraitsBase<T> {};

	// Fix reference-to-reference problems.
	template <typename T>
	struct TCallTraits<T&>
	{
		typedef T& ValueType;
		typedef T& Reference;
		typedef const T& ConstReference;
		typedef T& ParamType;
		typedef T& ConstPointerType;
	};

	// Array types
	template <typename T, size_t N>
	struct TCallTraits<T[N]>
	{
	private:
		typedef T ArrayType[N];
	public:
		typedef const T* ValueType;
		typedef ArrayType& Reference;
		typedef const ArrayType& ConstReference;
		typedef const T* const ParamType;
		typedef const T* const ConstPointerType;
	};

	// const array types
	template <typename T, size_t N>
	struct TCallTraits<const T[N]>
	{
	private:
		typedef const T ArrayType[N];
	public:
		typedef const T* ValueType;
		typedef ArrayType& Reference;
		typedef const ArrayType& ConstReference;
		typedef const T* const ParamType;
		typedef const T* const ConstPointerType;
	};
}

