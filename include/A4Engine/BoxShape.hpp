#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Shape.h>

class A4ENGINE_API BoxShape : public Shape
{
public:
	BoxShape(float width, float height);
	~BoxShape() = default;

	float GetMoment(float mass) override;
	cpShape* CreateShape(cpBody* body) override;
private:
	float m_width;
	float m_height;
};