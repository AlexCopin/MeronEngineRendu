#include "A4Engine/RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(float mass)
{
	m_body = cpBodyNew(mass, 1);
}

RigidBodyComponent::~RigidBodyComponent()
{
	cpBodyFree(m_body);
}
void RigidBodyComponent::SetBody(cpBody* body)
{
	m_body = std::move(body);
}

cpBody* RigidBodyComponent::GetBody()
{
	return m_body;
}

cpVect RigidBodyComponent::GetPosition()
{
	return cpBodyGetPosition(m_body);
}

float RigidBodyComponent::GetAngle()
{
	return cpBodyGetAngle(m_body);
}

void RigidBodyComponent::SetPosition(cpVect pos)
{
	cpBodySetPosition(m_body, pos);
}

void RigidBodyComponent::SetAngle(float angle)
{
	cpBodySetAngle(m_body, angle);
}

void RigidBodyComponent::SetBodyStatic()
{
	cpBodySetType(m_body, cpBodyType::CP_BODY_TYPE_STATIC);
}
void RigidBodyComponent::SetBodyDynamic()
{
	cpBodySetType(m_body, cpBodyType::CP_BODY_TYPE_DYNAMIC);
}
void RigidBodyComponent::SetBodyKinematic()
{
	cpBodySetType(m_body, cpBodyType::CP_BODY_TYPE_KINEMATIC);
}


void RigidBodyComponent::SetMoment()
{
	float tmp = 0;
	for (const auto& shape : m_shapeBank) {
		tmp += shape->GetMoment(cpBodyGetMass(m_body));
	}

	cpBodySetMoment(m_body, tmp);
}

void RigidBodyComponent::AddShape(cpSpace* space, Shape* shape)
{
	shape->CreateShape(m_body);
	m_shapeBank.push_back(shape);

	cpSpaceAddShape(space, shape->GetShape());
	cpSpaceAddBody(space, m_body);

	SetMoment();
}

void RigidBodyComponent::RemoveShape(Shape* shape)
{
	m_shapeBank.erase(std::find(m_shapeBank.begin(), m_shapeBank.end(), shape));
}

void RigidBodyComponent::CleanShapeBank(cpSpace* space)
{
	for(int i = 0; i < m_shapeBank.size(); ++i)
	{
		if (m_shapeBank[i] && m_shapeBank[i]->GetShape())
		{
			cpSpaceRemoveShape(space, m_shapeBank[i]->GetShape());
			cpShapeFree(m_shapeBank[i]->GetShape());
		}
	}
	if(cpBodyGetSpace(m_body))
		cpSpaceRemoveBody(space, m_body);
}
