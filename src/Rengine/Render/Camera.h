#ifndef RENGINE_CAMERA_H
#define RENGINE_CAMERA_H

#include "Rengine/Render/Transform.h"
#include "Rengine/Window.h"
#include "Rengine/Application.h"

namespace Rengine
{
	constexpr float YAW = -90.0f;
	constexpr float PITCH = 0.0f;
	constexpr float FOV = 45.0f;

	class Camera : public Transform
	{
	public:

		enum ProjectionType
		{
			ORTHOGRAPHIC,
			PERSPECTIVE
		};

		Camera(Window* window,
			ProjectionType projectionType = ProjectionType::ORTHOGRAPHIC,
			Vector3f position = Vector3f(0.0f, 0.0f, 0.0f),
			Vector3f up = Vector3f(0.0f, 1.0f, 0.0f),
			float yaw = YAW,
			float pitch = PITCH,
			float fov = FOV);
		~Camera();

		Vector2f    ScreenPosToWorldPos(Vector2f screenPos);

		bool        IsRectInView(const Rect& rect);

		void        Update();

		void        SetNeedUpdate(bool needUpdate);

		void        Translate(Vector3f translation) override;
		void        Rotate(Vector3f rotation) override;
		void        Scale(Vector3f scale) override;

		float       GetCameraFov();
		void        SetCameraFov(float fov);

		void        SetFar(float far_plane);
		void        SetNear(float near_plane);

		float       GetFar() const;
		float       GetNear() const;

		Matrix4f    GetCameraMatrix() const;
		Matrix4f    GetProjectionMatrix() const;

		inline Rect GetWorldRect() const { return m_WorldRect; }
		inline Rect GetViewRect() const { return m_ViewRect; }

		inline void SetPixelPerfect(bool value) { m_bPixelPerfect = value; }

		Vector3f m_vecFront;
		Vector3f m_vecUp;
		Vector3f m_vecRight;
		Vector3f m_vecWorldUp;

		float m_flFov;
	private:
		void UpdateProjectionVectors();
	private:
		Matrix4f m_CameraMatrix;
		Matrix4f m_ProjectionMatrix;
		float m_flFar;
		float m_flNear;
		bool m_bUpdateMatrix;
		ProjectionType m_eProjectionType;
		bool m_bPixelPerfect;
		Window* m_Window;

		Rect m_WorldRect = { 0.0f, 0.0f, 0.0f, 0.0f };
		Rect m_ViewRect = { 0.0f, 0.0f, 0.0f, 0.0f };
	};
}

#endif // RENGINE_CAMERA_H

