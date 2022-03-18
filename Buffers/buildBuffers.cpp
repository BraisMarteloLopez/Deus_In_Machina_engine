
	#include "buildBuffers.h"
	#include "../_config.h"
	#include "../World/world.h"






	buildBuffers* buildBuffers::ObjectBuildBuffers = nullptr;

	buildBuffers::buildBuffers() {}
	buildBuffers::~buildBuffers() { delete UBO_ptr; UBO_ptr = nullptr; }

	buildBuffers* buildBuffers::getInstance()
	{
		if (ObjectBuildBuffers == nullptr)
			ObjectBuildBuffers = new buildBuffers();

		return ObjectBuildBuffers;
	}


	//


	void buildBuffers::buildSSBO(const std::vector<glm::mat4>& matrixvector)
	{


		auto arrayMatrix = std::make_unique<GLfloat[]>(matrixvector.size() * 16);

		//

		for (GLuint i = 0, j = 0; i < matrixvector.size(); i++)
			for (GLuint row = 0; row < 4; row++)
				for (GLuint column = 0; column < 4; column++)
					arrayMatrix[j++] = matrixvector[i][row][column];

		//

		glGenBuffers(1, &SSBAO_animation_units);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBAO_animation_units);
		glBufferData(GL_SHADER_STORAGE_BUFFER, matrixvector.size() * sizeof(glm::mat4), arrayMatrix.get(), GL_STATIC_DRAW);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glBindVertexArray(0);
	}



	void buildBuffers::buildAnimation_UBOBuffer()
	{

		world* ObjectWorld;
		ObjectWorld = ObjectWorld->getInstance();

		_config* Object_Config;
		Object_Config = Object_Config->getInstance();

		//

		UBO_elements_block = (Object_Config->nSkeletons * Object_Config->nAnimationsSkeleton) * 4;
		UBO_size_bytes_block = UBO_elements_block * 4;

		UBO_ptr = new GLint[UBO_elements_block];

		auto temp_ = std::make_unique<GLfloat[]>(3 * UBO_size_bytes_block);
		GLuint _i = 0;

		//

		for (GLuint i = 0; i < 10; i++)
		{

			for (GLuint j = 0; j < 20; j++)
			{

				temp_[_i] = 0;

				if (i < ObjectWorld->SkeleticalAnimations.skeletons.size())
					if (j < ObjectWorld->SkeleticalAnimations.skeletons[i].animations.size())
						temp_[_i] = ObjectWorld->SkeleticalAnimations.skeletons[i].animations[j].startRange;

				_i += 4;
			}
		}

		//

		for (GLuint i = 0; i < 10; i++)
		{

			for (GLuint j = 0; j < 20; j++)
			{

				temp_[_i] = 0;

				if (i < ObjectWorld->SkeleticalAnimations.skeletons.size())
					if (j < ObjectWorld->SkeleticalAnimations.skeletons[i].animations.size())
						temp_[_i] = ObjectWorld->SkeleticalAnimations.skeletons[i].animations[j].animation._nTicks;

				_i += 4;
			}
		}

		//

		for (GLuint i = 0; i < 10; i++)
		{

			for (GLuint j = 0; j < 20; j++)
			{

				temp_[_i] = 0;
				_i += 4;
			}
		}

		//

		for (GLuint i = 0; i < 10; i++)
		{

			temp_[_i] = 0;

			if (i < ObjectWorld->SkeleticalAnimations.skeletons.size())
				temp_[_i] = ObjectWorld->SkeleticalAnimations.skeletons[i].skeleton.m_BoneCounter;

			_i += 4;
		}

		GLuint UBO_simpleSingle_block = Object_Config->nSkeletons * 4;


		// no logic here !!Simple GL_buffer implementation example

		glGenBuffers(1, &UBO_animation);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO_animation);
		glBufferData(GL_UNIFORM_BUFFER, (4 * UBO_size_bytes_block) + (4 * UBO_simpleSingle_block), temp_.get(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO_animation, 0, 20);
	}