#include "A4Engine/Shape.h"

Shape::Shape() : 
	m_shape(nullptr)
{
}

cpShape* Shape::GetShape()
{
	return m_shape;
}

cpShape* Shape::CreateShape(cpBody* body)
{
	return nullptr;
}

float Shape::GetMoment(float mass)
{
	return 0.0f;
}
