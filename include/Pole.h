/*!
 *  @file Pole.h
 *  @author Paul
 *  @date 2024-11-21
 *
 *  Pole vector library
 */

#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp> // For length squared
#include <glm/gtc/quaternion.hpp>

 /**
  * Computes a pole vector location given the root, mid & tip translations
  * If the three of them are aligned, use a quaternion to compute the pole vector
  * @param Root Root translation
  * @param Mid Mid translation
  * @param Tip Tip translation
  * @param Rotation Fallback to use when root mid & tip are aligned (i.e V = Mid + Rot * PoleDir*Distance)
  * @param PoleDir Fallback to use when root mid & tip are aligned (i.e V = Mid + Rot * PoleDir*Distance)
  * @param Distance Fallback to use when root mid & tip are aligned (i.e V = Mid + Rot * PoleDir*Distance)
  * @return Pole location
  */

namespace Maths
{
	glm::vec3 Pole(const glm::vec3& Root, const glm::vec3& Mid, const glm::vec3& Tip, const glm::quat& Rotation, const glm::vec3& PoleDir, float Distance)
	{
		const glm::vec3 RefMidVector = Rotation * PoleDir * Distance;
		const glm::vec3 MidT = (Tip + Root) * 0.5f;
		const glm::vec3 V = Mid - MidT;

		if (glm::length2(V) <= 0.001f)
		{
			return MidT + RefMidVector;
		}

		const glm::vec3 Axis = glm::normalize(MidT - Root);

		float A = glm::dot(Mid - Root, Axis);

		const glm::vec3 PoleCenter = Root + Axis * A;
		glm::vec3 PoleVec = Mid - PoleCenter;

		if (glm::dot(PoleVec,RefMidVector) < 0.0f)
			PoleVec = -PoleVec;

		return PoleCenter + PoleVec + glm::normalize(PoleVec) * Distance;
	}
}
