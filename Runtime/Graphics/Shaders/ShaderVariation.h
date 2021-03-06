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

//= INCLUDES =============================
#include "../D3D11/D3D11Shader.h"
#include "../../Math/Matrix.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector4.h"
#include "../Material.h"
#include "../D3D11/D3D11GraphicsDevice.h"
#include "../../Components/Light.h"
#include "../../Components/Camera.h"
#include "../D3D11/D3D11ConstantBuffer.h"
//=======================================

#define NULL_SHADER_ID "-1";

namespace Directus
{
	class ShaderVariation : public Resource
	{
	public:
		ShaderVariation();
		~ShaderVariation();

		void Initialize(
			Context* context,
			bool albedo,
			bool roughness,
			bool metallic,
			bool normal,
			bool height,
			bool occlusion,
			bool emission,
			bool mask,
			bool cubemap
		);
	
		//= RESOURCE INTERFACE ========================
		bool LoadFromFile(const std::string& filePath);
		bool SaveToFile(const std::string& filePath);
		//=============================================

		void Set();
		void UpdatePerFrameBuffer(Light* directionalLight, Camera* camera);
		void UpdatePerMaterialBuffer(std::weak_ptr<Material> material);
		void UpdatePerObjectBuffer(const Math::Matrix& mWorld, const Math::Matrix& mView, const Math::Matrix& mProjection, bool receiveShadows);
		void UpdateTextures(const std::vector<ID3D11ShaderResourceView*>& textureArray);
		void Render(int indexCount);

		bool HasAlbedoTexture() { return m_hasAlbedoTexture; }
		bool HasRoughnessTexture() { return m_hasRoughnessTexture; }
		bool HasMetallicTexture() { return m_hasMetallicTexture; }
		bool HasNormalTexture() { return m_hasNormalTexture; }
		bool HasHeightTexture() { return m_hasHeightTexture; }
		bool HasOcclusionTexture() { return m_hasOcclusionTexture; }
		bool HasEmissionTexture() { return m_hasEmissionTexture; }
		bool HasMaskTexture() { return m_hasMaskTexture; }
		bool HasCubeMapTexture() { return m_hasCubeMap; }

	private:
		void AddDefinesBasedOnMaterial(std::shared_ptr<D3D11Shader> shader);
		void Compile(const std::string& filePath);

		//= PROPERTIES ============
		bool m_hasAlbedoTexture;
		bool m_hasRoughnessTexture;
		bool m_hasMetallicTexture;
		bool m_hasNormalTexture;
		bool m_hasHeightTexture;
		bool m_hasOcclusionTexture;
		bool m_hasEmissionTexture;
		bool m_hasMaskTexture;
		bool m_hasCubeMap;

		//= MISC ==================================================
		Graphics* m_graphics;
		std::shared_ptr<D3D11ConstantBuffer> m_perObjectBuffer;
		std::shared_ptr<D3D11ConstantBuffer> m_materialBuffer;
		std::shared_ptr<D3D11ConstantBuffer> m_miscBuffer;
		std::shared_ptr<D3D11Shader> m_D3D11Shader;

		//= BUFFERS ===============================================
		const static int cascades = 3;
		struct PerFrameBufferType
		{
			Math::Vector2 viewport;
			float nearPlane;
			float farPlane;
			Math::Matrix mLightViewProjection[cascades];
			Math::Vector4 shadowSplits;
			Math::Vector3 lightDir;
			float shadowBias;
			float shadowMapResolution;
			float shadowMappingQuality;
			Math::Vector2 padding;
		};

		struct PerMaterialBufferType
		{
			// Material
			Math::Vector4 matAlbedo;
			Math::Vector2 matTilingUV;
			Math::Vector2 matOffsetUV;
			float matRoughnessMul;
			float matMetallicMul;
			float matOcclusionMul;
			float matNormalMul;
			float matSpecularMul;
			float matShadingMode;
			Math::Vector2 padding;
		};
		PerMaterialBufferType perMaterialBufferCPU;

		struct PerObjectBufferType
		{
			Math::Matrix mWorld;
			Math::Matrix mWorldView;
			Math::Matrix mWorldViewProjection;
			float receiveShadows;
			Math::Vector3 padding;
		};
		PerObjectBufferType perObjectBufferCPU;
		//==========================================================
	};
}