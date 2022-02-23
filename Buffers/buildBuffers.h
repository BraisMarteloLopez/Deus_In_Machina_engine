#pragma once

#ifndef DEUSEX_BUILDBUFFERS_H
#define DEUSEX_BUILDBUFFERS_H

#include "../AuxHeadersDef/Core_Headers.h"
#include "../AuxHeadersDef/Aux_Headers.h"
#include "../AuxHeadersDef/Stream_Headers.h"




	struct buildBuffers
	{

		private:

			static buildBuffers* ObjectBuildBuffers;

		protected:

			buildBuffers();

		public:

			~buildBuffers();
			static buildBuffers* getInstance();


			void buildSSBO(const std::vector<glm::mat4>&);
			void buildAnimation_UBOBuffer();

			//

			GLuint SSBAO_animation_units = 0;
			GLuint UBO_animation = 0;

			GLint* UBO_ptr = nullptr;
			GLuint UBO_elements_block = 0;
			GLuint UBO_size_bytes_block = 0;
	};


#endif