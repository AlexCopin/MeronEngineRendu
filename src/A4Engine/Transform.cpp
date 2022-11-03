#include <A4Engine/Transform.hpp>
#include <algorithm>
#include <cassert>

Transform::Transform() :
m_parent(nullptr),
m_position(0.f, 0.f),
m_rotation(0.f),
m_scale(1.f, 1.f)
{
}

Transform::Transform(const Transform& transform) :
m_parent(nullptr),
m_position(transform.m_position),
m_rotation(transform.m_rotation),
m_scale(transform.m_scale)
{
	SetParent(transform.m_parent);
}

Transform::Transform(Transform&& transform) noexcept :
m_children(std::move(transform.m_children)),
m_parent(nullptr),
m_position(transform.m_position),
m_rotation(transform.m_rotation),
m_scale(transform.m_scale)
{
	SetParent(transform.m_parent);
	for (Transform* child : m_children)
		child->m_parent = this;
}

Transform::~Transform()
{
	if (m_parent)
		m_parent->DetachChild(this);

	for (Transform* child : m_children)
		child->m_parent = nullptr;
}

Transform* Transform::GetParent() const
{
	return m_parent;
}

Vector2f Transform::GetGlobalPosition() const
{
	if (!m_parent)
		return m_position;

	return m_parent->TransformPoint(m_position);
}

float Transform::GetGlobalRotation() const
{
	if (!m_parent)
		return m_rotation;

	return m_parent->GetGlobalRotation() + m_rotation;
}

Vector2f Transform::GetGlobalScale() const
{
	if (!m_parent)
		return m_scale;

	return m_parent->GetGlobalScale() * m_scale;
}

const Vector2f& Transform::GetPosition() const
{
	return m_position;
}

float Transform::GetRotation() const
{
	return m_rotation;
}

const Vector2f& Transform::GetScale() const
{
	return m_scale;
}

void Transform::Rotate(float rotation)
{
	m_rotation += rotation;
}

void Transform::Scale(float scale)
{
	m_scale *= scale;
}

void Transform::Scale(const Vector2f& scale)
{
	m_scale *= scale;
}

void Transform::SetParent(Transform* parent)
{
	if (m_parent == parent)
		return;

	if (m_parent)
		m_parent->DetachChild(this);

	m_parent = parent;
	if (m_parent)
		m_parent->AttachChild(this);
}

void Transform::SetPosition(const Vector2f& position)
{
	m_position = position;
}

void Transform::SetRotation(float rotation)
{
	m_rotation = rotation;
}

void Transform::SetScale(const Vector2f& scale)
{
	m_scale = scale;
}

void Transform::Translate(const Vector2f& translation)
{
	m_position += translation;
}

Vector2f Transform::TransformPoint(Vector2f position) const
{
	// L'ordre des transformations s'appelle ici le SRT et est très courant

	// Scale
	position *= GetGlobalScale();

	// Rotation
	position = Vector2f::Rotate(position, GetGlobalRotation());

	// Translation
	if (m_parent)
		position += m_parent->TransformPoint(m_position);
	else
		position += m_position;

	return position;
}

Vector2f Transform::TransformInversePoint(Vector2f position) const
{
	// Lorsqu'on effectue l'inverse d'une transformation, l'ordre de celles-ci est également inversé, on fait alors du TRS

	// Translation
	if (m_parent)
		position -= m_parent->TransformPoint(m_position);
	else
		position -= m_position;

	// Rotation
	position = Vector2f::Rotate(position, -GetGlobalRotation());

	// Scale
	position /= GetGlobalScale();

	return position;
}

Transform& Transform::operator=(const Transform& transform)
{
	m_position = transform.m_position;
	m_rotation = transform.m_rotation;
	m_scale = transform.m_scale;
	SetParent(transform.m_parent);

	return *this;
}

Transform& Transform::operator=(Transform&& transform) noexcept
{
	for (Transform* child : m_children)
		child->m_parent = nullptr;

	m_children = std::move(transform.m_children);
	m_position = transform.m_position;
	m_rotation = transform.m_rotation;
	m_scale = transform.m_scale;
	SetParent(transform.m_parent);

	for (Transform* child : m_children)
		child->m_parent = this;

	return *this;
}

void Transform::AttachChild(Transform* child)
{
	m_children.push_back(child);
}

void Transform::DetachChild(Transform* child)
{
	auto it = std::find(m_children.begin(), m_children.end(), child);
	assert(it != m_children.end());

	m_children.erase(it);
}