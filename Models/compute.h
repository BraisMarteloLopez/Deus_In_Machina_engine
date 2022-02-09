#pragma once

#ifndef DEUSEX_COMPUTEMESHES_H
#define DEUSEX_COMPUTEMESHES_H

	#include "../AuxHeadersDef/Core_Headers.h"
	#include "../AuxHeadersDef/Aux_Headers.h"
	#include "../AuxHeadersDef/Stream_Headers.h"
	#include <glm/glm.hpp>
	#include <assimp/Importer.hpp>
	#include <assimp/scene.h>
	#include <assimp/matrix4x4.h>
	#include <assimp/postprocess.h>
	#include <assimp/Quaternion.h>
	#include <assimp/vector3.h>
	#include <glm/gtx/quaternion.hpp>
	#include <map>
	#include <stack>
	#include <string>
	#include <string_view>
	#include <span>
	#include <vector>
	#include <optional>
	#include <iterator>
	#include <algorithm>
	#include <filesystem>
	#include <cmath>
	#include <cassert>

	#if defined(ASSIMP_DOUBLE_PRECISION)
	#  error ASSIMP vs GLM: Need to convert double to float.
	#endif

	static const GLint max_weigths = 4;
	using BoneIndex = GLint;


	struct VertexWBone
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_uv;
		BoneIndex bone_ids[max_weigths];
		GLfloat weights[max_weigths];
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};


	struct BoneKeyFrames
	{

		BoneIndex _bone_index = -1;
		glm::mat4 _local_space_to_bone = glm::mat4(1.f);

		std::vector<glm::vec3> _positions;
		std::vector<glm::quat> _rotations;
		std::vector<glm::vec3> _scales;

		glm::mat4 get_frame_at(GLuint _keyframe)
		{

			const glm::mat4 translation = get_position(_keyframe);
			const glm::mat4 rotation = get_rotation(_keyframe);
			const glm::mat4 scale = get_scaling(_keyframe);

			return (translation * rotation * scale);
		}

		glm::mat4 get_position(GLuint _key)
		{

			return glm::translate(glm::mat4(1.0f), glm::vec3(_positions[_key]));
		}


		glm::mat4 get_rotation(GLuint _key)
		{

			return glm::toMat4(glm::quat(glm::normalize(_rotations[_key])));
		}

		glm::mat4 get_scaling(GLuint _keyF)
		{

			return glm::scale(glm::mat4(1.0f), glm::vec3(_scales[_keyF]));
		}
	};


	struct AnimNode
	{

		std::optional<BoneKeyFrames> bone;
		glm::mat4 bone_transform;

		GLint parent = -1;
		glm::mat4 node_transform;
	};


	static glm::mat4 Matrix_RowToColumn(const aiMatrix4x4& m)
	{

		const glm::vec4 c1(m.a1, m.b1, m.c1, m.d1);
		const glm::vec4 c2(m.a2, m.b2, m.c2, m.d2);
		const glm::vec4 c3(m.a3, m.b3, m.c3, m.d3);
		const glm::vec4 c4(m.a4, m.b4, m.c4, m.d4);
		return glm::mat4(c1, c2, c3, c4);
	}


	static glm::vec3 Vec_ToGLM(const aiVector3D& vec)
	{

		return glm::vec3(vec.x, vec.y, vec.z);
	}


	static glm::quat Quat_ToGLM(const aiQuaternion& pOrientation)
	{

		return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}


	struct AnimMesh
	{

		std::vector<VertexWBone> vertices;
		std::vector<GLuint> indices;
	};


	struct BoneMeshInfo
	{

		BoneIndex index = -1;
		glm::mat4 local_space_to_mesh;
	};


	struct BoneInfoMap
	{


		void add_new_bone(std::string&& name, glm::mat4 bone_to_model)
		{

			auto [it, inserted] = _name_to_info.insert(
				std::make_pair(std::move(name), BoneMeshInfo{ -1, bone_to_model }));

			BoneMeshInfo& _info = it->second;
			_info.index = _next_bone_id++;
		}

		const BoneMeshInfo* get(const char* name) const
		{
			auto it = _name_to_info.find(name);
			return ((it != _name_to_info.end()) ? &(it->second) : nullptr);
		}

		const GLuint get_Index(const char* name) const
		{

			auto it = _name_to_info.find(name);
			return ((it != _name_to_info.end()) ? (it->second.index) : -1);
		}

		std::map<std::string, BoneMeshInfo, std::less<>> _name_to_info;

		BoneIndex _next_bone_id = 0;
		GLuint nBones = 0;
	};



	struct Vertex {

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};


	/*struct VertexWBone {

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;

		glm::vec4 Ids;
		glm::vec4 Weights;
	};*/


	struct Animation
	{

		Animation() {};
		Animation(std::vector<AnimNode>&& nodes, GLuint bones_count, GLfloat duration, GLfloat ticks_per_second)
		{

			for (GLuint i = 0; i < nodes.size(); i++)
			{

				_nodes.push_back(nodes[i]);

				if (_nodes[i].bone && _nTicks == 0)
					_nTicks = _nodes[i].bone->_positions.size();
			}

			for (GLuint i = 0; i < bones_count + 1; i++)
				_transforms.push_back(glm::mat4(1.0f));

			//

			_bones_count = bones_count;
			_duration = duration;
			_ticks_per_second = ticks_per_second;
		}

		void update(GLuint _keyframe)
		{

			for (std::size_t i = 0, count = _nodes.size(); i < count; ++i)
			{

				AnimNode& node = _nodes[i];
				assert(GLint(i) > node.parent);

				glm::mat4 bone_transform = node.bone
					? node.bone->get_frame_at(_keyframe)
					: node.node_transform;

				glm::mat4 parent_transform = (node.parent > 0)
					? _nodes[node.parent].bone_transform
					: glm::mat4(1.0f);

				glm::mat4 final_ = parent_transform * bone_transform;
				node.bone_transform = final_;

				if (node.bone)
				{

					const std::size_t bone_index = node.bone->_bone_index;

					assert(bone_index < _transforms.size()
						&& "Too many bones. See kMaxBonesCount limit.");

					_transforms[bone_index] = final_ * node.bone->_local_space_to_bone;
				}
			}
		}

		std::vector <glm::mat4> transforms() const
		{

			std::vector <glm::mat4> temp_transforms;

			for (GLuint i = 0; i < _bones_count; i++)
				temp_transforms.push_back(_transforms[i]);

			return temp_transforms;
		}

		std::vector<glm::mat4> _transforms;
		std::vector<AnimNode> _nodes;

		GLuint _bones_count = 0;
		GLuint _nTicks = 0;
		GLuint _ticks_per_second = 0;
		GLfloat _duration = 0.0f;
	};


	static void Assimp_FillBones(const aiScene& scene, const aiMesh& mesh, BoneInfoMap& bone_info)
	{

		for (GLuint i = 0; i < mesh.mNumBones; ++i)
		{

			const aiString& bone_name = mesh.mBones[i]->mName;
			bone_info.add_new_bone(std::string(bone_name.data, bone_name.length), Matrix_RowToColumn(mesh.mBones[i]->mOffsetMatrix));

			const aiBone* const bone = mesh.mBones[i];
			assert(bone);
		}
	}



	static AnimMesh Assimp_LoadMesh(const aiScene& scene, const aiMesh& mesh, const BoneInfoMap& bone_info)
	{

		std::vector<VertexWBone> vertices;
		vertices.reserve(mesh.mNumVertices);

		for (GLuint i = 0; i < mesh.mNumVertices; ++i)
		{

			aiVector3D* uvs = mesh.mTextureCoords[0];

			assert(uvs);
			assert(mesh.mNormals);
			assert(mesh.mTangents);
			assert(mesh.mBitangents);

			//

			vertices.push_back(VertexWBone{});
			VertexWBone& v = vertices.back();

			v.position = Vec_ToGLM(mesh.mVertices[i]);
			v.normal = Vec_ToGLM(mesh.mNormals[i]);
			v.texture_uv = glm::vec2(uvs[i].x, uvs[i].y);
			v.tangent = Vec_ToGLM(mesh.mTangents[i]);
			//v.bitangent = Vec_ToGLM(mesh.mBitangents[i]);

			//

			std::fill(std::begin(v.bone_ids), std::end(v.bone_ids), -1);
			std::fill(std::begin(v.weights), std::end(v.weights), 0.f);
		}

		std::vector<GLuint> indices;
		indices.reserve(mesh.mNumFaces * std::size_t(3));

		for (GLuint i = 0; i < mesh.mNumFaces; ++i)
		{

			aiFace face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);

			for (GLuint j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}


		auto add_bone_weight_to_vertex = [](VertexWBone& vertex, const BoneIndex bone_index, GLfloat weight)
		{

			auto it = std::find_if(std::begin(vertex.bone_ids), std::end(vertex.bone_ids)
				, [&](BoneIndex index) { return (index < 0) && (index != bone_index); });

			assert(it != std::end(vertex.bone_ids)
				&& "Either more [max_weigths const] per vertex OR duplicated bone.");
			const std::size_t i = std::distance(std::begin(vertex.bone_ids), it);

			vertex.weights[i] = weight;
			vertex.bone_ids[i] = bone_index;
		};

		for (GLuint i = 0, n = 0; i < mesh.mNumBones; ++i, n++)
		{

			const aiString& bone_name = mesh.mBones[i]->mName;
			const BoneIndex bone_index = bone_info.get_Index(mesh.mBones[i]->mName.C_Str());

			const aiBone* const bone = mesh.mBones[i];
			assert(bone);
			const aiVertexWeight* const weights = bone->mWeights;


			for (GLuint j = 0; j < bone->mNumWeights; ++j)
			{
				const GLuint vertex_id = weights[j].mVertexId;
				const GLfloat weight = weights[j].mWeight;
				assert(vertex_id <= vertices.size());

				add_bone_weight_to_vertex(vertices[vertex_id], bone_index, weight);
			}

		}

		AnimMesh anim_mesh;
		anim_mesh.indices = std::move(indices);
		anim_mesh.vertices = std::move(vertices);

		//

		return anim_mesh;
	}


	static std::vector<AnimMesh> Assimp_LoadModelMeshWithAnimationsWeights(const aiScene& scene, const BoneInfoMap& bone_info)
	{

		std::vector<AnimMesh> meshes;
		std::stack<const aiNode*> stack_aiNode;

		stack_aiNode.push(scene.mRootNode);

		while (stack_aiNode.size() > 0)
		{

			const aiNode* const node = stack_aiNode.top();
			stack_aiNode.pop();

			for (GLuint i = 0; i < node->mNumMeshes; ++i)
			{

				const aiMesh* const mesh = scene.mMeshes[node->mMeshes[i]];
				assert(mesh);

				meshes.push_back(Assimp_LoadMesh(scene, *mesh, bone_info));
			}

			for (GLuint i = 0; i < node->mNumChildren; ++i)
				stack_aiNode.push(node->mChildren[i]);
		}

		return meshes;
	}


	static GLuint Assimp_LoadSkeleton(const aiScene& scene, BoneInfoMap& bone_info)
	{

		std::stack<const aiNode*> stack_aiNode;
		stack_aiNode.push(scene.mRootNode);

		while (stack_aiNode.size() > 0)
		{

			const aiNode* const node = stack_aiNode.top();
			stack_aiNode.pop();

			//

			if (node->mNumMeshes > 0)
			{

				const aiMesh* const mesh = scene.mMeshes[node->mMeshes[0]];
				assert(mesh);

				Assimp_FillBones(scene, *mesh, bone_info);

				return bone_info._next_bone_id;
			}

			for (GLuint i = 0; i < node->mNumChildren; ++i)
				stack_aiNode.push(node->mChildren[i]);
		}
	}


	static BoneKeyFrames Assimp_LoadBoneKeyFrames(const aiNodeAnim& channel, const BoneMeshInfo& bone_info)
	{
		BoneKeyFrames bone;

		bone._bone_index = bone_info.index;
		bone._local_space_to_bone = bone_info.local_space_to_mesh;
		bone._positions.reserve(channel.mNumPositionKeys);
		
		for (GLuint index = 0; index < channel.mNumPositionKeys; ++index)
			bone._positions.push_back(Vec_ToGLM(channel.mPositionKeys[index].mValue));

		//

		bone._rotations.reserve(channel.mNumRotationKeys);
		
		for (GLuint index = 0; index < channel.mNumRotationKeys; ++index)
			bone._rotations.push_back(Quat_ToGLM(channel.mRotationKeys[index].mValue));

		//

		bone._scales.reserve(channel.mNumScalingKeys);

		for (GLuint index = 0; index < channel.mNumScalingKeys; ++index)
			bone._scales.push_back(Vec_ToGLM(channel.mScalingKeys[index].mValue));

		//

		return bone;
	}


	static Animation Assimp_LoadAnimation(const aiScene& scene, const BoneInfoMap& bone_info)
	{

		assert(scene.mNumAnimations == 1);

		const aiAnimation* const animation = scene.mAnimations[0];
		const GLfloat duration = GLfloat(animation->mDuration);
		const GLfloat ticks_per_second = GLfloat(animation->mTicksPerSecond);

		std::vector<AnimNode> nodes;
		std::vector<const aiString*> node_names;

		struct Node
		{
			const aiNode* src = nullptr;
			GLint parent = -1;
		};

		std::stack<Node> dfs;
		dfs.push(Node{ scene.mRootNode, -1 }); //no parent

		while (dfs.size() > 0)
		{
			Node data = std::move(dfs.top());
			dfs.pop();

			AnimNode node;
			node.parent = data.parent;
			node.node_transform = Matrix_RowToColumn(data.src->mTransformation);
			assert(node.parent < GLint(nodes.size()));
			nodes.push_back(node);
			node_names.push_back(&data.src->mName);
			const GLint parent_index = GLint(nodes.size() - 1);

			for (GLuint i = 0; i < data.src->mNumChildren; ++i)
			{
				dfs.push(Node{ data.src->mChildren[i], parent_index });
			}
		}

		for (GLuint i = 0; i < animation->mNumChannels; ++i)
		{

			auto channel = animation->mChannels[i];
			const aiString& bone_name = channel->mNodeName;

			auto it = std::find_if(node_names.cbegin(), node_names.cend()
				, [&bone_name](const aiString* node_name)
			{
				return (bone_name == *node_name);
			});


			assert(it != node_names.end() && "No node matching a bone.");
			const GLint index = GLint(std::distance(node_names.cbegin(), it));
			const BoneMeshInfo* _bone_info = bone_info.get(bone_name.C_Str());

			if (_bone_info == NULL)
				continue;

			assert(_bone_info && "No bone info remap matching a bone.");

			AnimNode& node = nodes[index];
			node.bone.emplace(Assimp_LoadBoneKeyFrames(*channel, *_bone_info));
		}


		const GLuint bones_count = GLuint(bone_info._name_to_info.size());
		return Animation(std::move(nodes), bones_count, duration, ticks_per_second);
	}



	struct Mesh
	{




		Mesh(std::vector<struct VertexWBone> _vertices, std::vector<GLuint> _indices)
		{

			verticesBW = _vertices;
			indices = _indices;
		}

		~Mesh() {};


		//std::vector<struct Vertex> vertices;
		std::vector<struct VertexWBone> verticesBW;
		std::vector<GLuint> indices;
	};


#endif
