#include <A4Engine/SegmentShape.hpp>

SegmentShape::SegmentShape(cpVect a, cpVect b, float radius) :
	m_a(a),
	m_b(b),
	m_radius(radius)
{
}

cpShape* SegmentShape::CreateShape(cpBody* body)
{
	m_shape = cpSegmentShapeNew(body, m_a, m_b, m_radius);
	return m_shape;
}

float SegmentShape::GetMoment(float mass)
{
	return cpMomentForSegment(mass, m_a, m_b, m_radius);
}
