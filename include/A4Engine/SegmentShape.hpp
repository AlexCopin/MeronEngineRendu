#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Shape.h>
#include <memory>

class RigidBodyComponent;


class A4ENGINE_API SegmentShape : public Shape
{
public:
	SegmentShape(cpVect a, cpVect b, float radius, RigidBodyComponent* body);
	~SegmentShape();

	cpShape* CreateShape(cpBody* body) override;

	float GetMoment(float mass) override;
private:

	cpVect m_a;
	cpVect m_b;
	float m_radius;
};