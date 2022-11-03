#pragma once

#include <A4Engine/Export.hpp>
#include <memory>

class Renderable;

struct A4ENGINE_API GraphicsComponent
{
	std::shared_ptr<Renderable> renderable;
};
