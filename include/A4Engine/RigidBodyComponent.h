#pragma once

#include <A4Engine/Export.hpp>
#include <chipmunk/chipmunk.h>
#include <A4Engine/Shape.h>
#include <memory>
#include <vector>
#include <A4Engine/Vector2.hpp>

class A4ENGINE_API RigidBodyComponent {
public:

	RigidBodyComponent(float mass);
	RigidBodyComponent(const RigidBodyComponent&) = default;
	RigidBodyComponent(RigidBodyComponent&&) = default;
	~RigidBodyComponent();

	RigidBodyComponent& operator=(const RigidBodyComponent&) = default;
	RigidBodyComponent& operator=(RigidBodyComponent&&) = default;

	cpBody* GetBody();
	void SetBody(cpBody* body);

	cpVect GetPosition();
	float GetAngle();
	void SetPosition(Vector2f pos);
	void SetVelocity(Vector2f velocity);
	Vector2f GetVelocity();
	void SetAngle(float angle);

	void SetBodyStatic();
	void SetBodyDynamic();
	void SetBodyKinematic();
	void SetMoment();
	void Impulse(Vector2f direction);

	void AddShape(cpSpace* space, Shape* shape);
	void RemoveShape(Shape* shape);

	void CleanShapeBank(cpSpace* space);

private:
	cpBody* m_body;
	std::vector<Shape*> m_shapeBank;
};