#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Shape.h>

class A4ENGINE_API BoxShape : public Shape
{
public:
	BoxShape(int width, int height);
	~BoxShape() = default;

	float GetMoment(float mass) override;
	cpShape* CreateShape(cpBody* body) override;
private:
	int m_width;
	int m_height;
};