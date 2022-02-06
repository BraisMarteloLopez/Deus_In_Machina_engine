
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

		std::vector<VertexWBone> vertices;
		std::vector<GLuint> indices;

		unsigned char ids[max_weigths];
		GLfloat weights[max_weigths];

		//

		for (GLuint i = 0, j = 0; i < mesh.vertices.size(); i++)
		{

			VertexWBone vertex;

			//

			for (GLint j = 0; j < max_weigths; j++)
			{

				if (mesh.vertices[i].bone_ids[j] >= 0)
				{

					ids[j] = skeletonRange + mesh.vertices[i].bone_ids[j];
					weights[j] = mesh.vertices[i].weights[j];
				}
			}

			vertex.Ids.x = ids[0];
			vertex.Ids.y = ids[1];
			vertex.Ids.z = ids[2];
			vertex.Ids.w = ids[3];

			vertex.Weights.x = weights[0];
			vertex.Weights.y = weights[1];
			vertex.Weights.z = weights[2];
			vertex.Weights.w = weights[3];

			//

			vertex.Position = mesh.vertices[i].position;
			vertex.Normal = mesh.vertices[i].normal;
			vertex.Tangent = mesh.vertices[i].tangent;

			vertex.TexCoords.x = mesh.vertices[i].texture_uv.x;
			vertex.TexCoords.y = 1 - mesh.vertices[i].texture_uv.y;

			//

			vertices.push_back(vertex);
		}

		return Mesh(vertices, mesh.indices);
	}