
#include "Includes.h"
#include "Camera.h"

float Camera::camera_MovementSpeed = 10;
Camera::Camera()
	:m_View(1.0)
{
	RightVector = glm::normalize(glm::cross(m_ViewDirection, Up));
	m_Projection = glm::perspective(glm::radians(m_verticalFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
	m_ProjectionView = m_Projection * m_View;
	m_movespeed = camera_MovementSpeed;
}
Camera::Camera(float width, float Height)
{
	RightVector = glm::cross(m_ViewDirection, Up);
	SetViewportSize(width/Height);
	m_movespeed = camera_MovementSpeed;

}

void Camera::SetViewportSize(float aspectratio)
{
	m_AspectRatio = aspectratio;
	RecalculateProjection();
	RecalculateProjectionView();
}

void Camera::RotateCamera(float pitch, float yaw, float roll)
{
	m_pitch = pitch;
	m_yaw = yaw;
	m_roll = roll;
	//glm::quat rotation = glm::angleAxis(glm::radians(yaw), Up) * glm::angleAxis(glm::radians(pitch),RightVector) ;
	//m_ViewDirection = glm::rotate(rotation, m_ViewDirection);
	m_ViewDirection = glm::mat3(glm::rotate(glm::radians(yaw), Up)) * glm::mat3(glm::rotate(glm::radians(pitch), RightVector)) * glm::mat3(glm::rotate(glm::radians(roll), m_ViewDirection)) * m_ViewDirection;
	//m_ViewDirection = rotation * glm::vec3(0, 0, 1);
	RecalculateProjectionView();
}

void Camera::RecalculateProjection()
{
	// ** Take Vertical Fov in radians
	//m_Projection = glm::perspective(glm::radians(m_verticalFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
	m_Projection = glm::ortho(-10.0f*m_AspectRatio, 10.0f*m_AspectRatio, -10.0f, 10.0f, -1.0f, 100.f);

}

void Camera::SetPerspctive(float v_FOV, float Near, float Far)
{
	m_verticalFOV = v_FOV;
	m_PerspectiveNear = Near;
	m_PerspectiveFar = Far;

	RecalculateProjection();
	RecalculateProjectionView();
}

void Camera::OnEvent(Event& e)
{
	EventDispatcher dis(e);

	dis.Dispatch<MouseScrollEvent>([&](MouseScrollEvent& event) {
		m_verticalFOV += event.GetYOffset() * 0.1;//change the fov (idk how to implement zoom in camera so I change the FOV :) )
		RecalculateProjection();
		RecalculateProjectionView();
		return true; });
}

void Camera::OnUpdate(float deltatime)
{
	
}

void Camera::RecalculateProjectionView()
{
	//moving the camera is nothing but transforming the world
	//glm::vec3 up = glm::normalize(glm::cross(RightVector, glm::normalize(m_ViewDirection)));
	m_View = glm::lookAt(m_Position - glm::normalize(m_ViewDirection), m_Position, Up);//this gives the view matrix
	m_ProjectionView = m_Projection * m_View;
}

