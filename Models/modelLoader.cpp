
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


	modelLoader::modelLoader(const std::string& _path, GLuint skeletonRange, const skeletonLoader& _skeletonItem, animationLoader& _animationItem)
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

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "xERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		//

		std::vector<AnimMesh> anim_meshes = Assimp_LoadModelMeshWithAnimationsWeights(*scene, _skeletonItem.skeleton);
		meshes.push_back(processMeshWithAnim(anim_meshes[0], skeletonRange, _animationItem.animation, _skeletonItem.skeleton, scene));
	}


	Mesh modelLoader::processMeshWithAnim(AnimMesh& mesh, GLuint skeletonRange, Animation& animation, const BoneInfoMap& boneInfo, const aiScene* scene)
	{


		return Mesh(mesh.vertices, mesh.indices);
	}