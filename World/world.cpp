
#include "world.h"


	world* world::ObjectWorld = nullptr;

	world::world() {}
	world::~world() {}

	world* world::getInstance()
	{
		if (ObjectWorld == nullptr)
			ObjectWorld = new world();

		return ObjectWorld;
	}

		