/*!
 *  @file NurbsCurve.h
 *  @author Paul
 *  @date 2024-11-21
 *
 *  Nurbs joint curve library
 */

#pragma once

#include "StaticArray.h"

#include <algorithm>
#include <iostream>
#include <span>
#include <glm/glm.hpp>

using int32 = std::int32_t;
using uint32 = std::uint32_t;

namespace Maths
{
	class PImplNurbsCurve
	{
	public:
		/**
		 * Finds the span index given knots, alpha & degrees
		 */
		template<int32 degrees>
		static int32 FindSpan(std::span<const float> Knots, float Alpha)
		{
			const int32 N = Knots.size() - degrees * 2;

			int32 Span0 = degrees;
			int32 SpanN = N - 1;

			for (int32 i = Span0; i <= SpanN; ++i)
			{
				if (Alpha >= Knots[i] && Alpha <= Knots[i + 1])
					return i;
			}

			return SpanN;
		}

		/**
		 * Computes the basis function
		 * @param i Span index
		 * @param Knots Knots value
		 * @param Alpha Alpha
		 * @param N Output N[i]
		 */
		template<int32 degrees>
		static void BasisFunction(int32 i, std::span<const float> Knots, float Alpha, std::span<float> N)
		{
			float Left[degrees + 1];
			float Right[degrees + 1];

			N[0] = 1.0f;

			for (int32 j = 1; j <= degrees; ++j)
			{
				Left[j] = Alpha - Knots[i + 1 - j];
				Right[j] = Knots[i + j] - Alpha;
				float Saved = 0.0f;

				for (size_t r = 0; r < static_cast<size_t>(j); ++r)
				{
					float Denum = Right[r + 1] + Left[static_cast<size_t>(j) - r];
					if (std::abs(Denum) <= 0.0001f)
						Denum = 0.00001f;

					float Temp = N[r] / Denum;
					N[r] = Saved + Right[r + 1] * Temp;
					Saved = Left[static_cast<size_t>(j) - r] * Temp;
				}

				N[j] = Saved;
			}
		}
	};

	/**
	 * Samples a B-spline curve given points, knots & alpha
	 * @param Points Points
	 * @param Knots Knots
	 * @param Alpha Alpha
	 */
	template<int32 degrees>
	glm::vec3 EvaluateNurbsCurve(std::span<const glm::vec3> Points, std::span<const float> Knots, float Alpha)
	{
		Alpha = std::clamp(Alpha, 0.0f, 1.0f);

		const int32 Span = PImplNurbsCurve::FindSpan<degrees>(Knots, Alpha);

		float N[degrees + 1];
		PImplNurbsCurve::BasisFunction<degrees>(Span, Knots, Alpha, N);

		glm::vec3 P = glm::vec3::ZeroVector;
		for (int32 i = 0; i <= degrees; ++i)
		{
			P += N[i] * Points[Span - degrees + i];
		}

		return P;
	}

	/**
	 * Structure returned by EvaluateNurbsBasis
	 * Exposes point weight & index to compute the final point value
	 */
	template<int32 degrees>
	class TNurbsBasis
	{
	public:
		constexpr int32 Num() const { return degrees + 1; }
		inline float Weight(int32 i) const { return N[static_cast<uint32>(i)]; }
		inline int32 Index(int32 i) const { return SpanIndex - degrees + i; }

	private:
		int32 SpanIndex;
		TStaticArray<float, degrees + 1> N;

		friend class PImplNurbsCurve;
	};

	/**
	 * Evaluates the basis function of a nurbs curve
	 * @param Knots Knot vector
	 * @param Alpha Time at which to evaluate the curve
	 * @return Nurbs basis
	 */
	template<int32 degrees>
	TNurbsBasis<degrees> EvaluateNurbsBasis(std::span<const float> Knots, float Alpha)
	{
		Alpha = std::clamp(Alpha, 0.0f, 1.0f);

		const int32 Span = PImplNurbsCurve::FindSpan<degrees>(Knots, Alpha);

		TStaticArray<float, degrees + 1> N;
		PImplNurbsCurve::BasisFunction<degrees>(Span, Knots, Alpha, N);

		return TNurbsBasis<degrees>{ Span, N };
	}

	/**
	 * Creates an open-clamped knot vector
	 */
	template<int32 degrees>
	void MakeOpenClampedKnot(std::span<float> InKnots)
	{
		float* U = InKnots.data();
		const int32 N = InKnots.size();

		const float Div = 1.0f / static_cast<float>(N - 3 * degrees);
		const int32 EndKnot = N - degrees * 2;

		for (int32 i = 0; i <= degrees - 1; ++i)
		{
			U[i] = 0.0f;
		}

		for (int32 i = degrees; i < EndKnot; ++i)
		{
			U[i] = static_cast<float>(i - degrees) * Div;
		}

		for (int32 i = EndKnot; i < N; ++i)
		{
			U[i] = 1.0f;
		}
	}

	template<int32 degrees>
	void MakeClosedKnot(std::span<float> InKnots)
	{
		float* U = InKnots.data();
		const int32 N = InKnots.size();

		const float Div = 1.0f / (N - 3 * degrees);
		const int32 EndKnot = N - degrees * 2;

		for (int32 i = 0; i <= degrees - 1; ++i)
		{
			U[i] = (degrees - i) * -Div;
		}

		for (int32 i = degrees; i < EndKnot; ++i)
		{
			U[i] = (i - degrees) * Div;
		}

		for (int32 i = EndKnot; i < N; ++i)
		{
			U[i] = 1.0f + (i - EndKnot) * Div;
		}
	}

	template<int32 degree, typename RigBone>
	static void CalculateLocal(std::span<RigBone>& Bones, std::span<const float> InKnots, std::span<const glm::mat4> InControlPoints, std::span<glm::vec3> Locals)
	{
		if (Bones.size() <= 1)
			return;

		const float Denum = 1.0f / float(Bones.size() - 1);

		for (int32 i = 0; i < Bones.size(); ++i)
		{
			const float Alpha = float(i) * Denum;

			const TNurbsBasis<degree> Basis = Maths::EvaluateNurbsBasis<degree>(InKnots, Alpha);

			glm::vec3 Location = glm::vec3::ZeroVector;
			for (int32 j = 0; j < Basis.size(); ++j)
				Location += Basis.Weight(j) * InControlPoints[Basis.Index(j)].GetLocation();

			Locals[i] = Bones[i].Transform().GetLocation() - Location;
		}
	}
}