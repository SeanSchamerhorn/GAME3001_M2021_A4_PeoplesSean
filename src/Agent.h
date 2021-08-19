#pragma once
#ifndef __AGENT__
#define __AGENT__

#include <glm/vec4.hpp>
#include "DisplayObject.h"

class Agent : public DisplayObject
{
public:
	Agent();
	~Agent();

	// Inherited via GameObject
	void draw() override = 0;
	void update() override = 0;
	void clean() override = 0;

	// getters
	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;

	float getLOSDistance() const;
	float getDetectionDistance() const;
	float getCOLDistance()const;

	bool hasLOS() const;
	bool hasDetection() const;
	bool hasCOL()const;

	float getCurrentHeading() const;
	glm::vec4 getLOSColour() const;
	glm::vec4 getDetectionColour() const;
	glm::vec4 getCOLColour()const;

	// setters
	void setTargetPosition(glm::vec2 new_position);
	void setCurrentDirection(glm::vec2 new_direction);

	void setLOSDistance(float distance);
	void setDetectionDistance(float distance);
	void setCOLDistance(float distance);

	void setHasCOL(bool state);
	void setHasLOS(bool state);
	void setHasDetection(bool state);

	void setCurrentHeading(float heading);
	void setLOSColour(glm::vec4 colour);
	void setDetectionColour(glm::vec4 colour);
	void setCOLColour(glm::vec4 colour);

private:
	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;

	// LOS
	float m_LOSDistance;
	bool m_hasLOS;
	glm::vec4 m_LOSColour;

	// Detection
	float m_DetectionDistance;
	bool m_hasDetection;
	glm::vec4 m_DetectionColour;

	// Collision
	float m_COLDistance;
	bool m_hasCOL;
	glm::vec4 m_CollisionColour;
};

#endif /* defined ( __AGENT__) */
