#pragma once

#ifndef DEUSEX_CONFIG_H
#define DEUSEX_CONFIG_H

#include "../AuxHeadersDef/Core_Headers.h"
#include "../AuxHeadersDef/Aux_Headers.h"
#include "../AuxHeadersDef/Stream_Headers.h"



	struct _config
	{

		private:

			static _config* Object_Config;

		protected:

			_config() {};

		public:

			~_config() {};

			static _config* getInstance()
			{

				if (Object_Config == nullptr)
					Object_Config = new _config();

				return Object_Config;
			};


			// resolutions
			//-------------------------------------------------------------------------------

			/*GLfloat originalSourceScreen_X = 3840.0f, originalSourceScreen_Y = 2160.0f;

			GLfloat desktop_SCR_X = 0.0f, desktop_SCR_Y = 0.0f;
			GLfloat custom_SCR_X = 0.0f, custom_SCR_Y = 0.0f;

			GLfloat SCR_X = 800.0f, SCR_Y = 600.0f;

			GLfloat ratioResolution;
			GLfloat VCR_X = 0, VCR_Y = 0;*/


			// Time
			//-------------------------------------------------------------------------------

			/*GLfloat runTime = 0.0f;
			GLfloat deltaTime = 0.0f;
			GLfloat mouseStopTime = 0.0f;*/


			// game_options
			//-------------------------------------------------------------------------------

			static const GLuint nSkeletons = 10;
			static const GLuint nAnimationsSkeleton = 20;
	};

#endif