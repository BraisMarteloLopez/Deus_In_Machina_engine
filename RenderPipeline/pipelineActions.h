#pragma once

#ifndef DEUSEX_PIPELINEACTIONS_H
#define DEUSEX_PIPELINEACTIONS_H

#include "../AuxHeadersDef/Core_Headers.h"
#include "../AuxHeadersDef/Aux_Headers.h"
#include "../AuxHeadersDef/Stream_Headers.h"








	struct pipelineActions
	{

		private:

			static pipelineActions* ObjectPipelineActions;

		protected:

			pipelineActions();

		public:

			~pipelineActions();
			static pipelineActions* getInstance();

			//

			void update_animations(/*world**/);
	};


#endif


