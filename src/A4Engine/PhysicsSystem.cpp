#include <A4Engine/PhysicsSystem.h>
#include <entt/entt.hpp>
#include <A4Engine/Math.hpp>
#include <A4Engine/Transform.hpp>


PhysicsSystem::PhysicsSystem(entt::registry& registry) :
	m_registry(registry)
{
	if (s_instance != nullptr)
		throw std::runtime_error("only one InputManager can be created");

	s_instance = this;

	m_space = cpSpaceNew();
	SetGravity(981.f);
	SetDamping(0.5f);
	m_timeStep = 0.01;
}

PhysicsSystem::~PhysicsSystem()
{
	auto RigidBodyView = m_registry.view<RigidBodyComponent>();
	for (entt::entity entity : RigidBodyView)
	{
		RigidBodyComponent& entityRigidBody = RigidBodyView.get<RigidBodyComponent>(entity);
		entityRigidBody.CleanShapeBank(m_space);
	}

	cpSpaceFree(m_space);
}

cpSpace* PhysicsSystem::GetSpace()
{
	return m_space;
}

void PhysicsSystem::DebugDraw(SDLppRenderer& renderer, const Matrix3& cameraInverseTransform)
{
	struct DrawData
	{
		SDLppRenderer& renderer;
		const Matrix3& viewMatrix;
	};

	DrawData drawData{ renderer, cameraInverseTransform };

	cpSpaceDebugDrawOptions drawOptions;
	drawOptions.collisionPointColor = cpSpaceDebugColor{ 0.f, 0.f, 1.f, 1.f };
	drawOptions.constraintColor = cpSpaceDebugColor{ 0.f, 1.f, 0.f, 1.f };
	drawOptions.shapeOutlineColor = cpSpaceDebugColor{ 1.f, 0.f, 0.f, 1.f };
	drawOptions.data = &drawData;

	std::underlying_type_t<cpSpaceDebugDrawFlags> drawFlags = 0;
	drawOptions.flags = static_cast<cpSpaceDebugDrawFlags>(CP_SPACE_DEBUG_DRAW_SHAPES | CP_SPACE_DEBUG_DRAW_COLLISION_POINTS);

	// Callback trampoline
	drawOptions.colorForShape = [](cpShape* /*shape*/, cpDataPointer /*userdata*/)
	{
		return cpSpaceDebugColor{ 1.f, 0.f, 0.f, 1.f };
	};

	drawOptions.drawCircle = [](cpVect pos, cpFloat /*angle*/, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor /*fillColor*/, cpDataPointer data)
	{
		DrawData& drawData = *static_cast<DrawData*>(data);

		std::array<SDL_FPoint, 21> points;
		for (std::size_t i = 0; i < 20; ++i)
		{
			float angle = 2 * M_PI * i / 20;
			Vector2f finalPos = drawData.viewMatrix * Vector2f(pos.x + std::sin(angle) * radius, pos.y + std::cos(angle) * radius);
			points[i] = { finalPos.x, finalPos.y };
		}
		// On ferme le cercle
		points[20] = points[0];

		drawData.renderer.SetDrawColor(outlineColor.r * 255, outlineColor.g * 255, outlineColor.b * 255, outlineColor.a * 255);
		drawData.renderer.DrawLines(points.data(), points.size());
	};

	drawOptions.drawDot = [](cpFloat size, cpVect pos, cpSpaceDebugColor color, cpDataPointer data)
	{
		DrawData& drawData = *static_cast<DrawData*>(data);

		Vector2f dotPos = drawData.viewMatrix * Vector2f(static_cast<float>(pos.x), static_cast<float>(pos.y));
		float s = static_cast<float>(size);

		std::array<SDL_FPoint, 5> points;
		points[0] = { dotPos.x - s, dotPos.y - s };
		points[1] = { dotPos.x + s, dotPos.y - s };
		points[2] = { dotPos.x + s, dotPos.y + s };
		points[3] = { dotPos.x - s, dotPos.y + s };
		points[4] = { dotPos.x - s, dotPos.y - s };

		drawData.renderer.SetDrawColor(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
		drawData.renderer.DrawLines(points.data(), points.size());
	};

	drawOptions.drawFatSegment = [](cpVect a, cpVect b, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor /*fillColor*/, cpDataPointer data)
	{
		DrawData& drawData = *static_cast<DrawData*>(data);

		Vector2f from = drawData.viewMatrix * Vector2f(static_cast<float>(a.x), static_cast<float>(a.y));
		Vector2f to = drawData.viewMatrix * Vector2f(static_cast<float>(b.x), static_cast<float>(b.y));
		float r = static_cast<float>(radius);

		std::array<SDL_FPoint, 5> points;
		points[0] = { from.x - r, from.y - r };
		points[1] = { to.x + r, to.y - r };
		points[2] = { to.x + r, to.y + r };
		points[3] = { from.x - r, from.y + r };
		points[4] = { from.x - r, from.y - r };

		drawData.renderer.SetDrawColor(outlineColor.r * 255, outlineColor.g * 255, outlineColor.b * 255, outlineColor.a * 255);
		drawData.renderer.DrawLines(points.data(), points.size());
	};

	drawOptions.drawPolygon = [](int vertexCount, const cpVect* vertices, cpFloat /*radius*/, cpSpaceDebugColor outlineColor, cpSpaceDebugColor /*fillColor*/, cpDataPointer data)
	{
		DrawData& drawData = *static_cast<DrawData*>(data);

		std::vector<SDL_FPoint> points;
		points.reserve(vertexCount);

		for (int i = 0; i < vertexCount; ++i)
		{
			Vector2f pos = drawData.viewMatrix * Vector2f(static_cast<float>(vertices[i].x), static_cast<float>(vertices[i].y));
			points.push_back({ pos.x, pos.y });
		}
		// On ferme le cercle
		points.push_back(points.front());

		drawData.renderer.SetDrawColor(outlineColor.r * 255, outlineColor.g * 255, outlineColor.b * 255, outlineColor.a * 255);
		drawData.renderer.DrawLines(points.data(), points.size());
	};

	drawOptions.drawSegment = [](cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data)
	{
		DrawData& drawData = *static_cast<DrawData*>(data);

		Vector2f from = drawData.viewMatrix * Vector2f(static_cast<float>(a.x), static_cast<float>(a.y));
		Vector2f to = drawData.viewMatrix * Vector2f(static_cast<float>(b.x), static_cast<float>(b.y));

		std::array<SDL_FPoint, 2> points;
		points[0] = { from.x, from.y };
		points[1] = { to.x, to.y };

		drawData.renderer.SetDrawColor(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
		drawData.renderer.DrawLines(points.data(), points.size());
	};

	cpSpaceDebugDraw(m_space, &drawOptions);
}

float PhysicsSystem::GetGravity()
{
	return cpSpaceGetGravity(m_space).y;
}

float PhysicsSystem::GetDamping()
{
	return cpSpaceGetDamping(m_space);
}

void PhysicsSystem::SetGravity(float value)
{
	cpSpaceSetGravity(m_space, cpv(0, value));
}

void PhysicsSystem::SetDamping(float value)
{
	cpSpaceSetDamping(m_space, value);
}

void PhysicsSystem::AddHandler(cpCollisionType type1, cpCollisionType type2, const cpCollisionBeginFunc& func)
{
	cpCollisionHandler* handler = cpSpaceAddCollisionHandler(m_space, type1, type2);
	handler->beginFunc = func;
	m_handlers.emplace_back(handler);
}

void PhysicsSystem::AddShape(Shape* shape)
{
	cpSpaceAddShape(m_space, shape->GetShape());
}

void PhysicsSystem::Step(float deltaTime)
{
	m_timeAccumulator += deltaTime;
	while (m_timeAccumulator >= m_timeStep) {
		cpSpaceStep(m_space, m_timeStep);
		m_timeAccumulator -= m_timeStep;
	}
}


//Tentative ratée de FixedUpdate (modifications enlevées)
void PhysicsSystem::FixedUpdate(float deltaTime)
{
	auto RigidBodyView = m_registry.view<RigidBodyComponent, Transform>();
	for (entt::entity entity : RigidBodyView)
	{
		RigidBodyComponent& entityRigidBody = RigidBodyView.get<RigidBodyComponent>(entity);
		Transform& entityTransform = RigidBodyView.get<Transform>(entity);

		cpVect pos = entityRigidBody.GetPosition();
		float rot = entityRigidBody.GetAngle() * Rad2Deg;

		entityTransform.SetPosition(Vector2f(pos.x, pos.y));
		entityTransform.SetRotation(rot);
	}
}

PhysicsSystem& PhysicsSystem::Instance()
{
	if (s_instance == nullptr)
		throw std::runtime_error("GameInstance hasn't been instantiated");

	return *s_instance;
}

PhysicsSystem* PhysicsSystem::s_instance = nullptr;