#include <A4Engine/BoxShape.hpp>

BoxShape::BoxShape(float width, float height) :
	m_width(width),
	m_height(height)
{
}

float BoxShape::GetMoment(float mass)
{
	return cpMomentForBox(mass, m_width, m_height);
}

cpShape* BoxShape::CreateShape(cpBody* body)
{
	m_shape = cpBoxShapeNew(body, m_width, m_height, 0);
	return m_shape;
}