/*!
 *  @file SolveQuadratic.h
 *  @author Paul
 *  @date 2024-11-21
 *
 *  Function to compute ax² + bx + c = 0
 */

#pragma once

#include <cmath>

namespace Maths
{
	class FQuadraticRoot
	{
	public:
		FQuadraticRoot() = default;

		inline explicit FQuadraticRoot(float x)
			: Roots{ x }
			, Count(1)
		{
		}

		inline explicit FQuadraticRoot(float x1, float x2)
			: Roots{ x1, x2 }
			, Count(2)
		{
		}

		inline float operator[](size_t InIndex) const
		{
			return Roots[InIndex];
		}

		std::size_t Size() const { return Count; }
		const float* Data() const { return Roots; }
		float* Data() { return Roots; }

		float* begin() { return Roots; }
		float* end() { return Roots + Count; }
		const float* begin() const { return Roots; }
		const float* end() const { return Roots + Count; }

	private:
		float Roots[2];
		std::size_t Count = 0;
	};

	/**
	 * Function to solve axÂ² + bx + c = 0, given a,b & c
	 * /!\ Does not support complex solutions
	 * @param A Coefficient
	 * @param B Coefficient
	 * @param C Coefficient
	 * @return Roots
	 */
	inline FQuadraticRoot SolveQuadratic(float A, float B, float C)
	{
		const float Determinant = B * B - 4.0f * A * C;
		const float Inv2a = 1.0f / (2.0f * A);

		if (Determinant > 0.0f)
		{
			const float S = std::sqrt(Determinant);

			return FQuadraticRoot
			{
				(-B + S) * Inv2a,
				(-B - S) * Inv2a,
			};
		}
		else if (Determinant == 0.0f)
		{
			return FQuadraticRoot{ -B * Inv2a };
		}

		// < 0, no solution in real numbers
		return FQuadraticRoot{};
	}
}
