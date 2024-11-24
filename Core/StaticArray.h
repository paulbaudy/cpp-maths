/*!
 *  @file StaticArray.h
 *  @author Paul
 *  @date 2024-11-22
 *
 */

#pragma once

#include "CallTraits.h"
#include <stdint.h>

using uint32 = unsigned int;
using int32 = int;

namespace Maths
{
	enum ENoInit { NoInit };
	enum EInPlace { InPlace };

	template <typename T, T... Indices>
	struct TIntegerSequence
	{
	};

#ifdef _MSC_VER

	template <typename T, T N>
	using TMakeIntegerSequence = __make_integer_seq<TIntegerSequence, T, N>;

#endif

	/** An array with a static number of elements. */
	template <typename InElementType, uint32 NumElements, uint32 Alignment = alignof(InElementType)>
	class alignas(Alignment) TStaticArray
	{
	public:
		using ElementType = InElementType;

		TStaticArray()
			: Storage()
		{
		}

		template <typename... ArgTypes>
		explicit TStaticArray(EInPlace, ArgTypes&&... Args)
			: Storage(InPlace, TMakeIntegerSequence<uint32, NumElements>(), Forward<ArgTypes>(Args)...)
		{
		}

		TStaticArray(TStaticArray&& Other) = default;
		TStaticArray(const TStaticArray& Other) = default;
		TStaticArray& operator=(TStaticArray&& Other) = default;
		TStaticArray& operator=(const TStaticArray& Other) = default;

		// Accessors.
		inline InElementType& operator[](uint32 Index)
		{
			return Storage.Elements[Index].Element;
		}

		inline const InElementType& operator[](uint32 Index) const
		{
			return Storage.Elements[Index].Element;
		}

		// Comparisons.
		friend bool operator==(const TStaticArray& A, const TStaticArray& B)
		{
			for (uint32 ElementIndex = 0; ElementIndex < NumElements; ++ElementIndex)
			{
				if (!(A[ElementIndex] == B[ElementIndex]))
				{
					return false;
				}
			}
			return true;
		}

		bool operator!=(const TStaticArray& B) const
		{
			for (uint32 ElementIndex = 0; ElementIndex < NumElements; ++ElementIndex)
			{
				if (!((*this)[ElementIndex] == B[ElementIndex]))
				{
					return true;
				}
			}
			return false;
		}

		/**
		 * Returns true if the array is empty and contains no elements.
		 *
		 * @returns True if the array is empty.
		 * @see Num
		 */
		bool IsEmpty() const
		{
			return NumElements == 0;
		}

		/** The number of elements in the array. */
		inline int32 Num() const { return NumElements; }

		/** A pointer to the first element of the array */
		inline       InElementType* GetData() { static_assert((alignof(ElementType) % Alignment) == 0, "GetData() cannot be called on a TStaticArray with non-standard alignment"); return &Storage.Elements[0].Element; }
		inline const InElementType* GetData() const { static_assert((alignof(ElementType) % Alignment) == 0, "GetData() cannot be called on a TStaticArray with non-standard alignment"); return &Storage.Elements[0].Element; }

	private:

		struct alignas(Alignment) TArrayStorageElementAligned
		{
			TArrayStorageElementAligned() {}

			// Index is used to achieve pack expansion in TArrayStorage, but is unused here
			template <typename... ArgTypes>
			explicit TArrayStorageElementAligned(EInPlace, uint32 /*Index*/, ArgTypes&&... Args)
				: Element(Forward<ArgTypes>(Args)...)
			{
			}

			InElementType Element;
		};

		struct TArrayStorage
		{
			TArrayStorage()
				: Elements()
			{
			}

			template<uint32... Indices, typename... ArgTypes>
			explicit TArrayStorage(EInPlace, TIntegerSequence<uint32, Indices...>, ArgTypes&&... Args)
				: Elements{ TArrayStorageElementAligned(InPlace, Indices, Args...)... }
			{
				// The arguments are deliberately not forwarded arguments here, because we're initializing multiple elements
				// and don't want an argument to be mutated by the first element's constructor and then that moved-from state
				// be used to construct the remaining elements.
				//
				// This'll mean that it'll be a compile error to use move-only types like TUniquePtr when in-place constructing
				// TStaticArray elements, which is a natural expectation because that TUniquePtr can only transfer ownership to
				// a single element.
			}

			TArrayStorageElementAligned Elements[NumElements];
		};

		TArrayStorage Storage;


	public:

		template <typename StorageElementType, bool bReverse = false>
		struct FRangedForIterator
		{
			explicit FRangedForIterator(StorageElementType* InPtr)
				: Ptr(InPtr)
			{
			}

			auto& operator*() const
			{
				if constexpr (bReverse)
				{
					return (Ptr - 1)->Element;
				}
				else
				{
					return Ptr->Element;
				}
			}

			FRangedForIterator& operator++()
			{
				if constexpr (bReverse)
				{
					--Ptr;
				}
				else
				{
					++Ptr;
				}
				return *this;
			}

			bool operator!=(const FRangedForIterator& B) const
			{
				return Ptr != B.Ptr;
			}

		private:
			StorageElementType* Ptr;
		};

		using RangedForIteratorType = FRangedForIterator<      TArrayStorageElementAligned>;
		using RangedForConstIteratorType = FRangedForIterator<const TArrayStorageElementAligned>;
		using RangedForReverseIteratorType = FRangedForIterator<      TArrayStorageElementAligned, true>;
		using RangedForConstReverseIteratorType = FRangedForIterator<const TArrayStorageElementAligned, true>;

		/** STL-like iterators to enable range-based for loop support. */
		inline RangedForIteratorType				begin() { return RangedForIteratorType(Storage.Elements); }
		inline RangedForConstIteratorType			begin() const { return RangedForConstIteratorType(Storage.Elements); }
		inline RangedForIteratorType				end() { return RangedForIteratorType(Storage.Elements + NumElements); }
		inline RangedForConstIteratorType			end() const { return RangedForConstIteratorType(Storage.Elements + NumElements); }
		inline RangedForReverseIteratorType			rbegin() { return RangedForReverseIteratorType(Storage.Elements + NumElements); }
		inline RangedForConstReverseIteratorType	rbegin() const { return RangedForConstReverseIteratorType(Storage.Elements + NumElements); }
		inline RangedForReverseIteratorType			rend() { return RangedForReverseIteratorType(Storage.Elements); }
		inline RangedForConstReverseIteratorType	rend() const { return RangedForConstReverseIteratorType(Storage.Elements); }
	};

	/** Creates a static array filled with the specified value. */
	template <typename InElementType, uint32 NumElements>
	TStaticArray<InElementType, NumElements> MakeUniformStaticArray(typename TCallTraits<InElementType>::ParamType InValue)
	{
		TStaticArray<InElementType, NumElements> Result;
		for (uint32 ElementIndex = 0; ElementIndex < NumElements; ++ElementIndex)
		{
			Result[ElementIndex] = InValue;
		}
		return Result;
	}

	template <typename T> struct TIsContiguousContainer;

	template <typename ElementType, uint32 NumElements, uint32 Alignment>
	struct TIsContiguousContainer<TStaticArray<ElementType, NumElements, Alignment>>
	{
		enum { Value = (alignof(ElementType) % Alignment) == 0 };
	};


	/** Hash function. */
	template <typename ElementType, uint32 NumElements, uint32 Alignment>
	uint32 GetTypeHash(const TStaticArray<ElementType, NumElements, Alignment>& Array)
	{
		uint32 Hash = 0;
		for (const ElementType& Element : Array)
		{
			Hash = HashCombineFast(Hash, GetTypeHash(Element));
		}
		return Hash;
	}

}

