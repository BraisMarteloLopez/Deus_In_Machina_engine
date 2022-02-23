#pragma once


#ifndef DEUSEX_ENTITY_H
#define DEUSEX_ENTITY_H

	#include "../AuxHeadersDef/Aux_Headers.h"
	#include "../Models/modelLoader.h"



	struct entityAnimation
	{

		entityAnimation() {};
		entityAnimation(const std::string _path, const std::string _name, const BoneInfoMap& _boneInfo, GLuint _startRange) {

			startRange = _startRange;
			name = _name;

			animation = animationLoader(_path, _boneInfo).animation;
		};

		std::string name = "";
		GLuint startRange = 0;

		Animation animation;
	};


	struct entitySkeleton
	{

		entitySkeleton(const std::string _path, std::string _name, GLuint _startRange)
		{

			skeleton = skeletonLoader(_path, _name);
			startRange = _startRange;
		};

		GLuint startRange = 0;
		GLuint lastRange = 0;

		skeletonLoader skeleton;

		std::vector<struct entityAnimation> animations;

		GLint anim_idle_ID = -1;
		GLint anim_attack_ID = -1;
	};


	struct entitySkeletalAnimations
	{

		entitySkeletalAnimations() {};

		void addSkeleton(const std::string _path, std::string _name)
		{

			skeletons.push_back(entitySkeleton(_path, _name, (GLuint)array_animation.size()));
		};

		void addAnimation(const std::string _path, std::string _name, GLint& _type)
		{

			GLuint indexSk = (GLuint)skeletons.size() - 1;
			GLuint indexAnim = (GLuint)skeletons[indexSk].animations.size();

			_type = indexAnim;
			skeletons[indexSk].animations.push_back(entityAnimation(_path, _name, skeletons[indexSk].skeleton.skeleton, skeletons[indexSk].lastRange));

			//

			for (GLuint i = 0; i < skeletons[indexSk].animations[indexAnim].animation._nTicks; i++)
			{

				skeletons[indexSk].animations[indexAnim].animation.update(i);

				for (GLuint j = 0; j < skeletons[indexSk].animations[indexAnim].animation._bones_count; j++)
					array_animation.push_back(skeletons[indexSk].animations[indexAnim].animation._transforms[j]);
			}

			skeletons[indexSk].lastRange = (GLuint)array_animation.size();
		};


		const entitySkeleton* getSkeletonByName(std::string _name)
		{

			std::for_each(_name.begin(), _name.end(), [](char& c) {
				c = ::tolower(c);
			});

			for (GLuint i = 0; i < skeletons.size(); i++)
				if (skeletons[i].skeleton.name == _name)
					return &skeletons[i];

			//

			std::cout << "No one skeleton found by: " << _name << std::endl;
			return 0;
		}

		GLint& linkIdle() { return skeletons[skeletons.size() - 1].anim_idle_ID; }
		GLint& linkAttack() { return skeletons[skeletons.size() - 1].anim_attack_ID; }

		std::vector <entitySkeleton> skeletons;
		std::vector<glm::mat4> array_animation;
	};




#endif
