#pragma once
#ifndef __BOUNDARY__
#define __BOUNDARY__

#include "Util.h"

class Boundary
{
public:
	Boundary() {}
	Boundary(glm::vec2 startP, float angle, float magnitude);
	~Boundary();

	void Draw();
	void Update();

	glm::vec2 GetStartPoint() { return startPoint; }
	glm::vec2 GetEndPoint() { return endPoint; }
	glm::vec2 GetNormal() { return normal; }
	float GetAngle() { return angle; }

	void SetStartPoint(glm::vec2 v2) { startPoint = v2; }
	void SetEndPoint(glm::vec2 v2) { endPoint = v2; }
	void SetAngle(float ag) { angle = ag; }
	void SetMagnitude(float v) { magnitude = v; }

private:
	glm::vec2 startPoint = glm::vec2(300,300);
	glm::vec2 endPoint;
	glm::vec2 normal;
	float angle = 90.0f;
	float magnitude;

	glm::vec4 red = glm::vec4(1, 0, 0, 1);
};

#endif /* defined (__BOUNDARY__) */
