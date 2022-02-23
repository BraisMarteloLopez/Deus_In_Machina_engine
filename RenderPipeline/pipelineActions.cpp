
#include "../AuxHeadersDef/Core_Headers.h"
#include "../AuxHeadersDef/Aux_Headers.h"
#include "../AuxHeadersDef/Stream_Headers.h"

#include "pipelineActions.h"




void pipelineActions::update_animations(/*world* ObjectWorld*/)
{

	/*GLuint _i = 0;

	for (GLuint i = 0; i < ObjectWorld->SkeleticalAnimations.skeletons.size(); i++)
	{

		for (GLuint j = 0; j < ObjectWorld->SkeleticalAnimations.skeletons[i].animations.size(); j++)
		{

			ObjectWorld->SkeleticalAnimations.skeletons[i].animations[j].animation.current_keyFrame++;

			if (ObjectWorld->SkeleticalAnimations.skeletons[i].animations[j].animation.current_keyFrame >= ObjectWorld->SkeleticalAnimations.skeletons[i].animations[j].animation._nTicks)
				ObjectWorld->SkeleticalAnimations.skeletons[i].animations[j].animation.current_keyFrame = 0;

			ObjectWorld->ObjectBuildBuffers->UBO_ptr[_i] = ObjectWorld->SkeleticalAnimations.skeletons[i].animations[j].animation.current_keyFrame;
			_i += 4;
		}
	}

	//

	glBindBuffer(GL_UNIFORM_BUFFER, ObjectWorld->ObjectBuildBuffers->UBO_animation);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * ObjectWorld->ObjectBuildBuffers->UBO_size_bytes_block, 4 * _i, ObjectWorld->ObjectBuildBuffers->UBO_ptr);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
}