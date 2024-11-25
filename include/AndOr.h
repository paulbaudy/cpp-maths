/*!
 *  @file AndOr.h
 *  @author Paul
 *  @date 2024-11-25
 *
 *  Templated disjunction & conjunction
 */

#pragma once

namespace Maths
{
	template <typename... Types>
	struct TAnd;

	template <bool LHSValue, typename... RHS>
	struct TAndValue
	{
		static constexpr bool Value = TAnd<RHS...>::value;
		static constexpr bool value = TAnd<RHS...>::value;
	};

	template <typename... RHS>
	struct TAndValue<false, RHS...>
	{
		static constexpr bool Value = false;
		static constexpr bool value = false;
	};

	template <typename LHS, typename... RHS>
	struct TAnd<LHS, RHS...> : TAndValue<LHS::Value, RHS...>
	{
	};

	template <>
	struct TAnd<>
	{
		static constexpr bool Value = true;
		static constexpr bool value = true;
	};
	template <typename... Types>
	struct TOr;

	template <bool LHSValue, typename... RHS>
	struct TOrValue
	{
		static constexpr bool Value = TOr<RHS...>::value;
		static constexpr bool value = TOr<RHS...>::value;
	};

	template <typename... RHS>
	struct TOrValue<true, RHS...>
	{
		static constexpr bool Value = true;
		static constexpr bool value = true;
	};

	template <typename LHS, typename... RHS>
	struct TOr<LHS, RHS...> : TOrValue<LHS::Value, RHS...>
	{
	};

	template <>
	struct TOr<>
	{
		static constexpr bool Value = false;
		static constexpr bool value = false;
	};

	template <typename Type>
	struct TNot
	{
		static constexpr bool Value = !Type::Value;
		static constexpr bool value = !Type::Value;
	};

}

