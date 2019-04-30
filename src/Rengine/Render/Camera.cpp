#include "PCH.h"
#include "Camera.h"

namespace Rengine
{
	Camera::Camera(Window* window,
		ProjectionType projectionType,
		Vector3f position,
		Vector3f up,
		float yaw,
		float pitch,
		float fov) :
		m_vecFront(Vector3f(0.0f, 0.0f, -1.0f)),
		m_vecWorldUp(up),
		m_flFov(fov),
		m_CameraMatrix(Matrix4f(1.0f)),
		m_ProjectionMatrix(Matrix4f(1.0f)),
		m_flFar(100.0f),
		m_flNear(0.1f),
		m_bUpdateMatrix(true),
		m_eProjectionType(ProjectionType::ORTHOGRAPHIC),
		m_bPixelPerfect(true),
		m_Window(window)
	{
		m_vecPosition = position;
		m_vecRotation.x = yaw;
		m_vecRotation.y = pitch;
		m_vecScale = { 1.0f, 1.0f, 0.0f };
	}


	Camera::~Camera()
	{
	}

	void Camera::Update()
	{
		if (m_bUpdateMatrix)
		{
			switch (m_eProjectionType)
			{
			case ProjectionType::ORTHOGRAPHIC:

				m_ViewRect.x = 0.0f;
				m_ViewRect.y = 0.0f;
				m_ViewRect.z = (float)m_Window->GetWidth();
				m_ViewRect.w = (float)m_Window->GetHeight();

				m_ProjectionMatrix = glm::ortho(m_ViewRect.x, m_ViewRect.z, m_ViewRect.y, m_ViewRect.w);

				Vector3f translate(-m_vecPosition.x + m_Window->GetWidth() / 2, -m_vecPosition.y + m_Window->GetHeight() / 2, 0.0f);
				Vector3f scale(m_vecScale.x, m_vecScale.y, m_vecScale.z);
				// Remove tile tearing by clamping positions to even numbers

				if (m_bPixelPerfect)
				{
					scale.x = glm::round(scale.x);
					scale.y = glm::round(scale.y);
					scale.z = glm::round(scale.z);

					translate.x = glm::round(translate.x);
					translate.y = glm::round(translate.y);
					translate.z = glm::round(translate.z);
				}

				m_CameraMatrix = glm::translate(m_ProjectionMatrix, translate);
				m_CameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_CameraMatrix;

				Vector2f bottomLeft = ScreenPosToWorldPos({ 0.0f, 0.0f });
				Vector2f topRight = ScreenPosToWorldPos({ m_Window->GetWidth(), m_Window->GetHeight() });
				m_WorldRect = { bottomLeft.x, bottomLeft.y, topRight.x, topRight.y };

					break;
				case ProjectionType::PERSPECTIVE:
					UpdateProjectionVectors();
					m_CameraMatrix = glm::lookAt(m_vecPosition, m_vecPosition + m_vecFront, m_vecUp);
					m_ProjectionMatrix = glm::perspective(glm::radians(m_flFov), (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), m_flNear, m_flFar);
					break;
			}

			m_bUpdateMatrix = false;
		}
	}

	Vector2f Camera::ScreenPosToWorldPos(Vector2f screenPos)
	{
		// 0 center
		screenPos -= Vector2f(m_Window->GetWidth() / 2.0f, m_Window->GetHeight() / 2.0f);

		// scale
		if (m_bPixelPerfect)
		{
			screenPos.x /= glm::round(m_vecScale.x);
			screenPos.y /= glm::round(m_vecScale.y);

			// translate cam pos
			screenPos += Vector2f(glm::round(m_vecPosition.x), glm::round(m_vecPosition.y));
		}
		else
		{
			screenPos.x /= m_vecScale.x;
			screenPos.y /= m_vecScale.y;

			// translate cam pos
			screenPos += Vector2f(m_vecPosition.x, m_vecPosition.y);
		}
		
		return screenPos;
	}

	bool Camera::IsRectInView(const Rect& rect)
	{
		if (rect.x > m_WorldRect.z || rect.y > m_WorldRect.w)
			return false;

		if (rect.x + rect.z < m_WorldRect.x || rect.y + rect.w < m_WorldRect.y)
			return false;

		return true;
	}

	void Camera::SetNeedUpdate(bool needUpdate)
	{
		m_bUpdateMatrix = needUpdate;
	}

	void Camera::Translate(Vector3f translation)
	{
		// Round cams position to prevent tile tearing (black lines between tiles)
		m_vecPosition += translation;
		m_bUpdateMatrix = true;
	}

	void Camera::Rotate(Vector3f rotation)
	{
		m_vecRotation += rotation;
		m_bUpdateMatrix = true;
	}

	void Camera::Scale(Vector3f scale)
	{
		m_vecScale += scale;
		m_bUpdateMatrix = true;
	}

	float Camera::GetCameraFov()
	{
		return m_flFov;
	}

	void Camera::SetCameraFov(float fov)
	{
		m_flFov = fov;
		m_bUpdateMatrix = true;
	}

	void Camera::SetFar(float far_plane)
	{
		m_flFar = far_plane;
	}

	void Camera::SetNear(float near_plane)
	{
		m_flNear = near_plane;
	}

	float Camera::GetFar() const
	{
		return m_flFar;
	}

	float Camera::GetNear() const
	{
		return m_flNear;
	}

	glm::mat4 Camera::GetCameraMatrix() const
	{
		return m_CameraMatrix;
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	void Camera::UpdateProjectionVectors()
	{
		Vector3f front;
		front.x = cos(glm::radians(m_vecRotation.x)) * cos(glm::radians(m_vecRotation.y));
		front.y = sin(glm::radians(m_vecRotation.y));
		front.z = sin(glm::radians(m_vecRotation.x)) * cos(glm::radians(m_vecRotation.y));
		m_vecFront = glm::normalize(front);

		m_vecRight = glm::normalize(glm::cross(m_vecFront, m_vecWorldUp));
		m_vecUp = glm::normalize(glm::cross(m_vecRight, m_vecFront));
	}
}