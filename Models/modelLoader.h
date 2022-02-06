#pragma once

#ifndef DEUSEX_MODELLOADER_H
#define DEUSEX_MODELLOADER_H

	#include <assimp/postprocess.h>
	#include <assimp/Importer.hpp>
	
	#include "../Models/compute.h"



	struct skeletonLoader
	{

		skeletonLoader() {};
		skeletonLoader(const std::string&, std::string);

		BoneInfoMap skeleton;

		std::string name;
		GLint m_BoneCounter = 0;
	};



	struct animationLoader
	{

		animationLoader() {};
		animationLoader(const std::string&, const BoneInfoMap&);

		Animation animation;
	};


	struct modelLoader
	{


		modelLoader(const std::string&, GLuint startRangeAnim, const skeletonLoader& _skeletonItem, animationLoader& _animationLoader);
		Mesh processMeshWithAnim(AnimMesh& mesh, GLuint skeletonRange, Animation& animation, const BoneInfoMap& boneInfo, const aiScene* scene);
		
		std::vector<Mesh> meshes;
	};



#endif

