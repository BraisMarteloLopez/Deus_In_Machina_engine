
#include "modelLoader.h"


	skeletonLoader::skeletonLoader(const std::string& _path, std::string _name)
	{

		std::for_each(_name.begin(), _name.end(), [](char& c) {
			c = ::tolower(c);
		});

		name = _name;

		Assimp::Importer importer;
		(void)importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
		const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_LimitBoneWeights);

		assert(scene);
		assert((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == 0);
		assert(scene->mRootNode);

		//

		m_BoneCounter = Assimp_LoadSkeleton(*scene, skeleton);
	}


	animationLoader::animationLoader(const std::string& _path, const BoneInfoMap& _skeleton)
	{

		Assimp::Importer importer;

		(void)importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
		const aiScene* scene = importer.ReadFile(_path, aiProcess_LimitBoneWeights);

		assert(scene);
		assert(scene->mRootNode);

		//

		animation = Assimp_LoadAnimation(*scene, _skeleton);
	}


	modelLoader::modelLoader(const std::string& _path, const skeletonLoader& _skeletonItem, animationLoader& _animationItem)
	{


		Assimp::Importer importer;
		(void)importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
		const aiScene* scene = importer.ReadFile(_path
			, aiProcess_Triangulate
			| aiProcess_CalcTangentSpace
			| aiProcess_FlipUVs
			| aiProcess_LimitBoneWeights);

		assert(scene);
		assert((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == 0);
		assert(scene->mRootNode);

		//

		std::vector<AnimMesh> anim_meshes = Assimp_LoadModelMeshWithAnimationsWeights(*scene, _skeletonItem.skeleton);
		meshes.push_back(processMeshWithAnim(anim_meshes, _animationItem.animation, _skeletonItem.skeleton, scene));
	}


	Mesh modelLoader::processMeshWithAnim(std::vector<AnimMesh>& meshes, Animation& animation, const BoneInfoMap& boneInfo, const aiScene* scene)
	{

		std::vector<VertexWBone> vertices;
		std::vector<GLuint> indices;

		//

		for (GLuint mesh = 0, prev_count = 0; mesh < (GLuint)meshes.size(); mesh++, prev_count = (GLuint)indices.size())
		{

			vertices.insert(vertices.end(), meshes[mesh].vertices.begin(), meshes[mesh].vertices.end());

			//

			for (GLuint k = 0; k < meshes[mesh].indices.size(); k++)
				indices.push_back(meshes[mesh].indices[k] + prev_count);
		}



		return Mesh(vertices, indices);
	}