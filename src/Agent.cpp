#include "Agent.h"

#include "Util.h"

Agent::Agent()
{
}

Agent::~Agent()
= default;

glm::vec2 Agent::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Agent::getCurrentDirection() const
{
	return m_currentDirection;
}

float Agent::getLOSDistance() const
{
	return m_LOSDistance;
}

float Agent::getDetectionDistance() const
{
	return m_DetectionDistance;
}

float Agent::getCOLDistance() const
{
	return m_COLDistance;
}

bool Agent::hasCOL() const
{
	return m_hasCOL;
}

void Agent::setCOLDistance(float distance)
{
	m_COLDistance = distance;
}

void Agent::setHasCOL(bool state)
{
	m_hasCOL = state;
	m_CollisionColour = (m_hasCOL) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

bool Agent::hasDetection() const
{
	return m_hasDetection;
}

glm::vec4 Agent::getDetectionColour() const
{
	return m_DetectionColour;
}

glm::vec4 Agent::getCOLColour() const
{
	return m_CollisionColour;
}

void Agent::setDetectionDistance(float distance)
{
	m_DetectionDistance = distance;
}

void Agent::setHasDetection(bool state)
{
	m_hasDetection = state;
	m_DetectionColour = (m_hasDetection) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
	
}

void Agent::setDetectionColour(glm::vec4 colour)
{
	m_DetectionColour = colour;
}

void Agent::setCOLColour(glm::vec4 colour)
{
	m_CollisionColour = colour;
}

bool Agent::hasLOS() const
{
	return m_hasLOS;
}

float Agent::getCurrentHeading() const
{
	return m_currentHeading;
}

glm::vec4 Agent::getLOSColour() const
{
	return m_LOSColour;
}

void Agent::setTargetPosition(const glm::vec2 new_position)
{
	m_targetPosition = new_position;
}

void Agent::setCurrentDirection(const glm::vec2 new_direction)
{
	m_currentDirection = new_direction;
}

void Agent::setLOSDistance(const float distance)
{
	m_LOSDistance = distance;
}

void Agent::setHasLOS(const bool state)
{
	m_hasLOS = state;
	m_LOSColour = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void Agent::setCurrentHeading(const float heading)
{
	m_currentHeading = heading;
	m_changeDirection();
}

void Agent::setLOSColour(const glm::vec4 colour)
{
	m_LOSColour = colour;
}

void Agent::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}
