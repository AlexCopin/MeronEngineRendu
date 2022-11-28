#pragma once

#include <A4Engine/Export.hpp>
#include <chipmunk/chipmunk.h>

enum CollisionTypes
{
	PLAYER,
	KILLER,
	HANDRESPAWN
};
class A4ENGINE_API Shape
{
public:
	Shape();
	~Shape() = default;

	cpShape* GetShape();

	void SetCollisionType(cpCollisionType type);

	virtual cpShape* CreateShape(cpBody* body);
	virtual float GetMoment(float mass);

protected:
	cpShape* m_shape;
};