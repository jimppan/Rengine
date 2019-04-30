#ifndef RENGINE_TRANSFORM_H
#define RENGINE_TRANSFORM_H

#include "PCH.h"
namespace Rengine
{
	class Transform
	{
	public:
		Transform(Vector3f pos = Vector3f(0.0f, 0.0f, 0.0f),
			Vector3f rot = Vector3f(0.0f, 0.0f, 0.0f),
			Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f)):
			m_vecPosition(pos),
			m_vecRotation(rot),
			m_vecScale(scale)
		{

		}

		virtual void Update() {}


		inline virtual void Translate(glm::vec3 translation)
		{
			m_vecPosition += translation;

		}

		inline virtual void Rotate(glm::vec3 rotation)
		{
			m_vecRotation += rotation;
		}

		inline virtual void Scale(glm::vec3 scale)
		{
			m_vecScale += scale;
		}

		Vector3f m_vecPosition;
		Vector3f m_vecRotation;
		Vector3f m_vecScale;
	};
}

#endif // RENGINE_TRANSFORM_H