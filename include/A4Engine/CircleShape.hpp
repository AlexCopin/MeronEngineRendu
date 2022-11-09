#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Shape.h>

class A4ENGINE_API CircleShape : public Shape
{
public:
	CircleShape(float radius);
	~CircleShape() = default;

	cpShape* CreateShape(cpBody* body) override;
	float GetMoment(float mass) override;
private:
	float m_radius;
};