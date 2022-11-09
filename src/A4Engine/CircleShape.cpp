#include <A4Engine/CircleShape.hpp>

CircleShape::CircleShape(float radius)
{
	m_radius = radius;
}

cpShape* CircleShape::CreateShape(cpBody* body)
{
	m_shape = cpCircleShapeNew(body, m_radius, cpVect());
	return m_shape;
}
float CircleShape::GetMoment(float mass)
{
	return cpMomentForCircle(mass, (double)m_radius * 2, (double)m_radius * 2, cpVect());
}
