#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>


// This is a SUPER basic interface for a physics thing.
// See your physics class for more details and a much more robust implementation.
//
// This is just so we can call a common physics (motion) integrator on any 
//	object that has a "physics" interface.
// 
// The physics integrator (updater) will get, integrate, then set the location, rotation, etc.
//
struct sPhysicalProps
{
	sPhysicalProps()
	{
		this->position = glm::vec3(0.0f);
		this->velocity = glm::vec3(0.0f);
		this->acceleration = glm::vec3(0.0f);
		this->orientationEuler = glm::vec3(0.0f);
		this->orientationQuat = glm::quat(glm::vec3(0.0f));
	}
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 orientationEuler;
	glm::quat orientationQuat;
};

class iPhysicalObject
{
public:
	virtual ~iPhysicalObject() {}

	virtual glm::vec3 getPosition(void) = 0;
	virtual void setPosition(const glm::vec3& newPosition) = 0;

	virtual glm::vec3 getVelocity(void) = 0;
	virtual void setVelocity(const glm::vec3& newVelocity) = 0;

	virtual glm::vec3 getAcceleration(void) = 0;
	virtual void setAcceleration(const glm::vec3& newAcceleration) = 0;

	virtual glm::vec3 getOrientationEuler(void) = 0;
	virtual void setOrientationEuler(const glm::vec3& newOrientationEuler) = 0;

	virtual glm::quat getOrientationQuaternion(void) = 0;
	virtual void setOrientationQuaternion(const glm::quat& newOrientationQuaternion) = 0;

	virtual sPhysicalProps getPhysicalProperties(void) = 0;
	virtual void setPhysicalProperties(const sPhysicalProps& newPhysProps) = 0;

};

