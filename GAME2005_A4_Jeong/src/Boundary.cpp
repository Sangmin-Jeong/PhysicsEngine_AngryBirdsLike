#include "Boundary.h"
#include "TextureManager.h"

Boundary::Boundary(glm::vec2 startP, float angle, float magnitude)
{
	SetStartPoint(startP);
	SetAngle(angle);
	SetMagnitude(magnitude);
}

Boundary::~Boundary()
= default;

void Boundary::Draw()
{
	// Normal Vector
	normal = Util::AngleMagnitudeToVec2(angle, 1.0f);
	SetEndPoint(startPoint + normal);
	Util::DrawLine(startPoint, endPoint);

	// Perpendicular Vectors in both side
	glm::vec2 perpendicular = Util::AngleMagnitudeToVec2(angle + 90.0f, magnitude);
	Util::DrawLine(startPoint - perpendicular, startPoint + perpendicular, red);

}

void Boundary::Update()
{

}
