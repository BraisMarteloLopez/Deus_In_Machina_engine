#pragma once

#ifndef MARTELO_WORLD_H
#define MARTELO_WORLD_H

	#include "../AuxHeadersDef/Core_Headers.h"
	#include "../AuxHeadersDef/Aux_Headers.h"
	#include "../AuxHeadersDef/Stream_Headers.h"
	
	#include "../Models/modelLoader.h"
	#include "../Entities/entity.h"



	struct world
	{

		private:

			static world* ObjectWorld;

		protected:

			world();

		public:

			~world();
			static world* getInstance();

			//

			entitySkeletalAnimations SkeleticalAnimations;
	};

#endif
