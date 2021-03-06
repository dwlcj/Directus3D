/*
Copyright(c) 2016-2017 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

//= INCLUDES ====================
#include <memory>
#include <vector>
#include "../Resource/Resource.h"
#include "../Math/Vector3.h"
#include "../Math/BoundingBox.h"
//===============================

namespace Directus
{
	class ResourceManager;
	class GameObject;
	class Mesh;
	class Material;
	struct VertexPosTexNorTan;

	namespace Math
	{
		class BoundingBox;
	}

	class Model : public Resource
	{
	public:
		Model(Context* context);
		~Model();

		//= RESOURCE INTERFACE ================================
		virtual bool LoadFromFile(const std::string& filePath);
		virtual bool SaveToFile(const std::string& filePath);
		//======================================================

		// Sets the  GameObject that represents this model in the scene
		void SetRootGameObject(std::weak_ptr<GameObject> gameObj) { m_rootGameObj = gameObj; }

		// Adds a mesh by creating from scratch
		std::weak_ptr<Mesh> AddMesh(const std::string& gameObjID, const std::string& name, std::vector<VertexPosTexNorTan> vertices, std::vector<unsigned int> indices);

		// Adds a mesh from memoery
		void AddMesh(std::shared_ptr<Mesh> mesh);

		// Adds a material to this model
		std::weak_ptr<Material> AddMaterial(std::shared_ptr<Material> material);

		std::weak_ptr<Mesh> GetMeshByID(const std::string& id);
		std::weak_ptr<Mesh> GetMeshByName(const std::string& name);

		std::string CopyTextureToLocalDirectory(const std::string& from);

		const Math::BoundingBox& GetBoundingBox() { return m_boundingBox; }
		float GetBoundingSphereRadius();

	private:
		// Load the model from disk
		bool LoadFromEngineFormat(const std::string& filePath);
		bool LoadFromForeignFormat(const std::string& filePath);

		//= SCALING / DIMENSIONS =======================
		void SetScale(float scale);
		float ComputeNormalizeScale();
		std::weak_ptr<Mesh> ComputeLargestBoundingBox();
		void ComputeDimensions();
		//==============================================

		// The root GameObject that represents this model in the scene
		std::weak_ptr<GameObject> m_rootGameObj;

		// The meshes that make up this model
		std::vector<std::shared_ptr<Mesh>> m_meshes;

		// The materials used by this model (materials also hold textures)
		std::vector<std::weak_ptr<Material>> m_materials;

		// Bounding box
		Math::BoundingBox m_boundingBox;
		float m_normalizedScale;

		// Dependencies
		ResourceManager* m_resourceManager;
	};
}