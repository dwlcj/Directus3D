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

//= INCLUDES ==================
#include "FullScreenQuad.h"
#include "../Core/Helper.h"
#include "../Graphics/Vertex.h"
//=============================

//= NAMESPACES ================
using namespace Directus::Math;
//=============================

namespace Directus
{
	FullScreenQuad::FullScreenQuad()
	{
		m_graphics = nullptr;
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;
		m_vertexCount = 0;
		m_indexCount = 0;
	}

	FullScreenQuad::~FullScreenQuad()
	{
		SafeRelease(m_vertexBuffer);
		SafeRelease(m_indexBuffer);
	}

	bool FullScreenQuad::Initialize(int width, int height, Graphics* graphics)
	{
		m_graphics = graphics;
		if (!m_graphics->GetDevice())
			return false;

		// Calculate the screen coordinates of the left side of the window.
		float left = static_cast<float>((width / 2) * -1);

		// Calculate the screen coordinates of the right side of the window.
		float right = left + static_cast<float>(width);

		// Calculate the screen coordinates of the top of the window.
		float top = static_cast<float>(height / 2);

		// Calculate the screen coordinates of the bottom of the window.
		float bottom = top - static_cast<float>(height);

		// Set the number of vertices in the vertex array.
		m_vertexCount = 6;

		// Set the number of indices in the index array.
		m_indexCount = m_vertexCount;

		// Create index & vertex arrays
		VertexPosTex* vertices = new VertexPosTex[m_vertexCount];
		unsigned long* indices = new unsigned long[m_indexCount];

		// Load the vertex array with data.
		// First triangle.
		vertices[0].position = Vector3(left, top, 0.0f); // Top left.
		vertices[0].uv = Vector2(0.0f, 0.0f);

		vertices[1].position = Vector3(right, bottom, 0.0f); // Bottom right.
		vertices[1].uv = Vector2(1.0f, 1.0f);

		vertices[2].position = Vector3(left, bottom, 0.0f); // Bottom left.
		vertices[2].uv = Vector2(0.0f, 1.0f);

		// Second triangle.
		vertices[3].position = Vector3(left, top, 0.0f); // Top left.
		vertices[3].uv = Vector2(0.0f, 0.0f);

		vertices[4].position = Vector3(right, top, 0.0f);// Top right.
		vertices[4].uv = Vector2(1.0f, 0.0f);

		vertices[5].position = Vector3(right, bottom, 0.0f); // Bottom right.
		vertices[5].uv = Vector2(1.0f, 1.0f);

		// Load the index array with data.
		for (int i = 0; i < m_indexCount; i++)
			indices[i] = i;

		// Set up the description of the vertex buffer.
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexPosTex) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now finally create the vertex buffer.
		HRESULT result = m_graphics->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
			return false;

		// Set up the description of the index buffer.
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = m_graphics->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(result))
			return false;

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete[] vertices;
		vertices = nullptr;

		delete[] indices;
		indices = nullptr;

		return true;
	}

	void FullScreenQuad::SetBuffers()
	{
		// Set vertex buffer stride and offset.
		unsigned int stride = sizeof(VertexPosTex);
		unsigned int offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		m_graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		m_graphics->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		m_graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}