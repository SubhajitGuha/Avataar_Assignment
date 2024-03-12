#pragma once
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"


class Camera
{
public:
	static float camera_MovementSpeed;
	Camera();
	Camera(float width, float Height);
	~Camera() = default;

	void SetPerspctive(float v_FOV,float Near,float Far);
	void SetCameraPosition(const glm::vec3& pos) { m_Position = pos; RecalculateProjectionView();}
	void SetViewDirection(const glm::vec3& dir) { m_ViewDirection = dir; RecalculateProjectionView(); }
	void SetUPVector(const glm::vec3& up) {	Up = up; RecalculateProjectionView();}
	void SetViewportSize(float aspectratio);
	void SetVerticalFOV(float fov) { m_verticalFOV = fov; SetPerspctive(m_verticalFOV, m_PerspectiveNear, m_PerspectiveFar); }
	void SetPerspectiveNear(float val) { m_PerspectiveNear = val; SetPerspctive(m_verticalFOV, m_PerspectiveNear, m_PerspectiveFar);}
	void SetPerspectiveFar(float val) { m_PerspectiveFar = val; SetPerspctive(m_verticalFOV, m_PerspectiveNear, m_PerspectiveFar);}

	float GetPerspectiveNear() { return m_PerspectiveNear; };
	float GetPerspectiveFar() { return m_PerspectiveFar; };
	glm::mat4 GetProjectionView() { return m_ProjectionView; }
	inline glm::mat4 GetViewMatrix() { return m_View; }
	inline glm::mat4 GetProjectionMatrix() { return m_Projection; }
	inline glm::vec3 GetCameraPosition() { return m_Position; }
	inline glm::vec3 GetCameraRotation() { return glm::vec3(m_pitch, m_yaw, m_roll); };
	inline glm::vec3 GetViewDirection() { return m_ViewDirection; }
	inline float GetAspectRatio() { return m_AspectRatio; }
	inline float GetVerticalFOV() { return m_verticalFOV; }

	void OnEvent(Event& e);
	void OnUpdate(float ts);
	void RotateCamera(float pitch = 0, float yaw = 0, float roll = 0);
private:
	void RecalculateProjection();
	void RecalculateProjectionView();

private:
	glm::mat4 m_View;
	glm::mat4 m_Projection;
	glm::mat4 m_ProjectionView;

	glm::vec3 m_Position = { 0,0,-10 }, m_ViewDirection = { 0,0,1 };
	//m_Viewdirection is the location we are looking at (it is the vector multiplied with rotation matrix)
	glm::vec3 Up = { 0,1,0 } , RightVector;//we get right vector by getting the cross product of m_ViewDirection and Up vectors

	float m_verticalFOV = 45.f;
	float m_PerspectiveNear = 0.1;
	float m_PerspectiveFar = 1000;
	
	float m_AspectRatio = 1.0;
	float m_pitch=0, m_yaw=0, m_roll=0;
	//camera parameters
	float m_movespeed = 30;
	glm::vec2 OldMousePos = { 0,0};
};