////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;

	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;
	m_instanceCount = 0;

	for (int i = 0; i < 14; i++)
	{
		GoombaPos[i] = XMFLOAT3(0.0f,0.0f,0.0f);
		goombadeadpos[i] = 0;
	}
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize_Instance(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename, int modelCount, const char* name)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeInstanceBuffers(device,modelCount,name);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetVertexCount()
{
	return m_vertexCount;
}

int ModelClass::GetInstanceCount()
{
	return m_instanceCount;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeInstanceBuffers(ID3D11Device* device, int receivedInstanceCount, const char* receivedInstanceName)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	m_instanceCount = receivedInstanceCount;
	m_instanceName = receivedInstanceName;

	InstanceType* instances = new InstanceType[m_instanceCount];
	if (!instances)
	{
		return false;
	}

	//ground
	if (m_instanceName == "Ground")
	{
		instances[0].position = XMFLOAT3(0.60f, -1.3f, 0.0f);
		instances[1].position = XMFLOAT3(-0.72f, -1.3f, 0.0f);
		instances[2].position = XMFLOAT3(-1.8f, -1.3f, 0.0f);
	}

	//ground_s
	else if (m_instanceName == "Ground_s")
	{
		instances[0].position = XMFLOAT3(-0.24f, -1.3f, 0.0f);
	}

	//mario
	else if (m_instanceName == "Mario")
	{
		instances[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	//pipe_s
	else if (m_instanceName == "Pipe_s")
	{
		instances[0].position = XMFLOAT3(0.0f, -0.05f, -7.0f);
		instances[1].position = XMFLOAT3(0.0f, 0.0f, 21.5f);
		instances[2].position = XMFLOAT3(0.0f, 0.0f, 25.0f);
	}

	//pipe_m
	else if (m_instanceName == "Pipe_m")
	{
		instances[0].position = XMFLOAT3(0.0f, 0.0f, -6.0f);
	}

	//pipe_l
	else if (m_instanceName == "Pipe_l")
	{
		instances[0].position = XMFLOAT3(0.0f, 0.0f, -5.0f);
		instances[1].position = XMFLOAT3(0.0f, 0.0f, -3.5f);
	}

	//brick cube
	else if (m_instanceName == "BrickCube")
	{
		//on ground1
		instances[0].position = XMFLOAT3(0.0f, 200.0f, -2800.0f);
		instances[1].position = XMFLOAT3(0.0f, 200.0f, -3010.0f);
		instances[2].position = XMFLOAT3(0.0f, 200.0f, -3220.0f);
		instances[3].position = XMFLOAT3(0.0f, 225.0f, -930.0f);

		//on ground2
		instances[4].position = XMFLOAT3(0.0f, 200.0f, 150.0f);
		instances[5].position = XMFLOAT3(0.0f, 200.0f, 360.0f);

		instances[6].position = XMFLOAT3(0.0f, 480.0f, 460.0f);
		instances[7].position = XMFLOAT3(0.0f, 480.0f, 560.0f);
		instances[8].position = XMFLOAT3(0.0f, 480.0f, 660.0f);
		instances[9].position = XMFLOAT3(0.0f, 480.0f, 760.0f);
		instances[10].position = XMFLOAT3(0.0f, 480.0f, 860.0f);
		instances[11].position = XMFLOAT3(0.0f, 480.0f, 960.0f);

		//on ground3
		instances[12].position = XMFLOAT3(0.0f, 480.0f, 1360.0f);
		instances[13].position = XMFLOAT3(0.0f, 480.0f, 1460.0f);
		instances[14].position = XMFLOAT3(0.0f, 200.0f, 1560.0f);

		instances[15].position = XMFLOAT3(0.0f, 200.0f, 1960.0f);
		instances[16].position = XMFLOAT3(0.0f, 200.0f, 2060.0f);

		instances[17].position = XMFLOAT3(0.0f, 200.0f, 3360.0f);

		instances[18].position = XMFLOAT3(0.0f, 480.0f, 3560.0f);
		instances[19].position = XMFLOAT3(0.0f, 480.0f, 3660.0f);
		instances[20].position = XMFLOAT3(0.0f, 480.0f, 3760.0f);

		instances[21].position = XMFLOAT3(0.0f, 480.0f, 4160.0f);
		instances[22].position = XMFLOAT3(0.0f, 480.0f, 4470.0f);

		instances[23].position = XMFLOAT3(0.0f, 200.0f, 4270.0f);
		instances[24].position = XMFLOAT3(0.0f, 200.0f, 4370.0f);

		//on ground4
		instances[25].position = XMFLOAT3(0.0f, 200.0f, 7370.0f);
		instances[26].position = XMFLOAT3(0.0f, 200.0f, 7470.0f);
		instances[27].position = XMFLOAT3(0.0f, 200.0f, 7680.0f);
	}

	//stair
	else if (m_instanceName == "Stair")
	{
		instances[0].position = XMFLOAT3(0.0f, 0.0f, 4750.0f);
		instances[1].position = XMFLOAT3(-100.0f, 0.0f, 4750.0f);
		instances[2].position = XMFLOAT3(100.0f, 0.0f, 4750.0f);
		instances[3].position = XMFLOAT3(0.0f, 0.0f, 4850.0f);
		instances[4].position = XMFLOAT3(-100.0f, 0.0f, 4850.0f);
		instances[5].position = XMFLOAT3(100.0f, 0.0f, 4850.0f);
		instances[6].position = XMFLOAT3(0.0f, 0.0f, 4950.0f);
		instances[7].position = XMFLOAT3(-100.0f, 0.0f, 4950.0f);
		instances[8].position = XMFLOAT3(100.0f, 0.0f, 4950.0f);
		instances[9].position = XMFLOAT3(0.0f, 0.0f, 5050.0f);
		instances[10].position = XMFLOAT3(-100.0f, 0.0f, 5050.0f);
		instances[11].position = XMFLOAT3(100.0f, 0.0f, 5050.0f);

		instances[12].position = XMFLOAT3(0.0f, 100.0f, 4850.0f);
		instances[13].position = XMFLOAT3(-100.0f, 100.0f, 4850.0f);
		instances[14].position = XMFLOAT3(100.0f, 100.0f, 4850.0f);
		instances[15].position = XMFLOAT3(0.0f, 100.0f, 4950.0f);
		instances[16].position = XMFLOAT3(-100.0f, 100.0f, 4950.0f);
		instances[17].position = XMFLOAT3(100.0f, 100.0f, 4950.0f);
		instances[18].position = XMFLOAT3(0.0f, 100.0f, 5050.0f);
		instances[19].position = XMFLOAT3(-100.0f, 100.0f, 5050.0f);
		instances[20].position = XMFLOAT3(100.0f, 100.0f, 5050.0f);

		instances[21].position = XMFLOAT3(0.0f, 200.0f, 4950.0f);
		instances[22].position = XMFLOAT3(-100.0f, 200.0f, 4950.0f);
		instances[23].position = XMFLOAT3(100.0f, 200.0f, 4950.0f);
		instances[24].position = XMFLOAT3(0.0f, 200.0f, 5050.0f);
		instances[25].position = XMFLOAT3(-100.0f, 200.0f, 5050.0f);
		instances[26].position = XMFLOAT3(100.0f, 200.0f, 5050.0f);

		instances[27].position = XMFLOAT3(0.0f, 300.0f, 5050.0f);
		instances[28].position = XMFLOAT3(-100.0f, 300.0f, 5050.0f);
		instances[29].position = XMFLOAT3(100.0f, 300.0f, 5050.0f);

		instances[30].position = XMFLOAT3(200.0f, 0.0f, 4750.0f);
		instances[31].position = XMFLOAT3(200.0f, 0.0f, 4850.0f);
		instances[32].position = XMFLOAT3(200.0f, 0.0f, 4950.0f);
		instances[33].position = XMFLOAT3(200.0f, 0.0f, 5050.0f);
		instances[34].position = XMFLOAT3(200.0f, 100.0f, 4850.0f);
		instances[35].position = XMFLOAT3(200.0f, 100.0f, 4950.0f);
		instances[36].position = XMFLOAT3(200.0f, 100.0f, 5050.0f);
		instances[37].position = XMFLOAT3(200.0f, 200.0f, 4950.0f);
		instances[38].position = XMFLOAT3(200.0f, 200.0f, 5050.0f);
		instances[39].position = XMFLOAT3(200.0f, 300.0f, 5050.0f);
	}

	//stair2
	else if (m_instanceName == "Stair2")
	{
		instances[0].position = XMFLOAT3(0.0f, 0.0f, 6750.0f);
		instances[1].position = XMFLOAT3(-100.0f, 0.0f, 6750.0f);
		instances[2].position = XMFLOAT3(100.0f, 0.0f, 6750.0f);
		instances[3].position = XMFLOAT3(200.0f, 0.0f, 6750.0f);

		instances[4].position = XMFLOAT3(0.0f, 100.0f, 6750.0f);
		instances[5].position = XMFLOAT3(-100.0f, 100.0f, 6750.0f);
		instances[6].position = XMFLOAT3(100.0f, 100.0f, 6750.0f);
		instances[7].position = XMFLOAT3(200.0f, 100.0f, 6750.0f);

		instances[8].position = XMFLOAT3(0.0f, 200.0f, 6750.0f);
		instances[9].position = XMFLOAT3(-100.0f, 200.0f, 6750.0f);
		instances[10].position = XMFLOAT3(100.0f, 200.0f, 6750.0f);
		instances[11].position = XMFLOAT3(200.0f, 200.0f, 6750.0f);

		instances[12].position = XMFLOAT3(0.0f, 300.0f, 6750.0f);
		instances[13].position = XMFLOAT3(-100.0f, 300.0f, 6750.0f);
		instances[14].position = XMFLOAT3(100.0f, 300.0f, 6750.0f);
		instances[15].position = XMFLOAT3(200.0f, 300.0f, 6750.0f);
	}
	
	//stair3
	else if (m_instanceName == "Stair3")
	{
		//1층
		instances[0].position = XMFLOAT3(0.0f, 0.0f, 5150.0f);
		instances[1].position = XMFLOAT3(-100.0f, 0.0f, 5150.0f);
		instances[2].position = XMFLOAT3(100.0f, 0.0f, 5150.0f);
		instances[3].position = XMFLOAT3(200.0f, 0.0f, 5150.0f);

		instances[4].position = XMFLOAT3(0.0f, 0.0f, 5250.0f);
		instances[5].position = XMFLOAT3(-100.0f, 0.0f, 5250.0f);
		instances[6].position = XMFLOAT3(100.0f, 0.0f, 5250.0f);
		instances[7].position = XMFLOAT3(200.0f, 0.0f, 5250.0f);

		instances[8].position = XMFLOAT3(0.0f, 0.0f, 5350.0f);
		instances[9].position = XMFLOAT3(-100.0f, 0.0f, 5350.0f);
		instances[10].position = XMFLOAT3(100.0f, 0.0f, 5350.0f);
		instances[11].position = XMFLOAT3(200.0f, 0.0f, 5350.0f);

		instances[12].position = XMFLOAT3(0.0f, 0.0f, 5450.0f);
		instances[13].position = XMFLOAT3(-100.0f, 0.0f, 5450.0f);
		instances[14].position = XMFLOAT3(100.0f, 0.0f, 5450.0f);
		instances[15].position = XMFLOAT3(200.0f, 0.0f, 5450.0f);

		instances[16].position = XMFLOAT3(0.0f, 0.0f, 5550.0f);
		instances[17].position = XMFLOAT3(-100.0f, 0.0f, 5550.0f);
		instances[18].position = XMFLOAT3(100.0f, 0.0f, 5550.0f);
		instances[19].position = XMFLOAT3(200.0f, 0.0f, 5550.0f);

		//2층
		instances[20].position = XMFLOAT3(0.0f, 100.0f, 5150.0f);
		instances[21].position = XMFLOAT3(-100.0f, 100.0f, 5150.0f);
		instances[22].position = XMFLOAT3(100.0f, 100.0f, 5150.0f);
		instances[23].position = XMFLOAT3(200.0f, 100.0f, 5150.0f);

		instances[24].position = XMFLOAT3(0.0f, 100.0f, 5250.0f);
		instances[25].position = XMFLOAT3(-100.0f, 100.0f, 5250.0f);
		instances[26].position = XMFLOAT3(100.0f, 100.0f, 5250.0f);
		instances[27].position = XMFLOAT3(200.0f, 100.0f, 5250.0f);

		instances[28].position = XMFLOAT3(0.0f, 100.0f, 5350.0f);
		instances[29].position = XMFLOAT3(-100.0f, 100.0f, 5350.0f);
		instances[30].position = XMFLOAT3(100.0f, 100.0f, 5350.0f);
		instances[31].position = XMFLOAT3(200.0f, 100.0f, 5350.0f);

		instances[32].position = XMFLOAT3(0.0f, 100.0f, 5450.0f);
		instances[33].position = XMFLOAT3(-100.0f, 100.0f, 5450.0f);
		instances[34].position = XMFLOAT3(100.0f, 100.0f, 5450.0f);
		instances[35].position = XMFLOAT3(200.0f, 100.0f, 5450.0f);

		instances[36].position = XMFLOAT3(0.0f, 100.0f, 5550.0f);
		instances[37].position = XMFLOAT3(-100.0f, 100.0f, 5550.0f);
		instances[38].position = XMFLOAT3(100.0f, 100.0f, 5550.0f);
		instances[39].position = XMFLOAT3(200.0f, 100.0f, 5550.0f);

		//3층
		instances[40].position = XMFLOAT3(0.0f, 200.0f, 5150.0f);
		instances[41].position = XMFLOAT3(-100.0f, 200.0f, 5150.0f);
		instances[42].position = XMFLOAT3(100.0f, 200.0f, 5150.0f);
		instances[43].position = XMFLOAT3(200.0f, 200.0f, 5150.0f);

		instances[44].position = XMFLOAT3(0.0f, 200.0f, 5250.0f);
		instances[45].position = XMFLOAT3(-100.0f, 200.0f, 5250.0f);
		instances[46].position = XMFLOAT3(100.0f, 200.0f, 5250.0f);
		instances[47].position = XMFLOAT3(200.0f, 200.0f, 5250.0f);

		instances[48].position = XMFLOAT3(0.0f, 200.0f, 5350.0f);
		instances[49].position = XMFLOAT3(-100.0f, 200.0f, 5350.0f);
		instances[50].position = XMFLOAT3(100.0f, 200.0f, 5350.0f);
		instances[51].position = XMFLOAT3(200.0f, 200.0f, 5350.0f);

		instances[52].position = XMFLOAT3(0.0f, 200.0f, 5450.0f);
		instances[53].position = XMFLOAT3(-100.0f, 200.0f, 5450.0f);
		instances[54].position = XMFLOAT3(100.0f, 200.0f, 5450.0f);
		instances[55].position = XMFLOAT3(200.0f, 200.0f, 5450.0f);

		instances[56].position = XMFLOAT3(0.0f, 200.0f, 5550.0f);
		instances[57].position = XMFLOAT3(-100.0f, 200.0f, 5550.0f);
		instances[58].position = XMFLOAT3(100.0f, 200.0f, 5550.0f);
		instances[59].position = XMFLOAT3(200.0f, 200.0f, 5550.0f);

		//4층
		instances[60].position = XMFLOAT3(0.0f, 300.0f, 5150.0f);
		instances[61].position = XMFLOAT3(-100.0f, 300.0f, 5150.0f);
		instances[62].position = XMFLOAT3(100.0f, 300.0f, 5150.0f);
		instances[63].position = XMFLOAT3(200.0f, 300.0f, 5150.0f);

		instances[64].position = XMFLOAT3(0.0f, 300.0f, 5250.0f);
		instances[65].position = XMFLOAT3(-100.0f, 300.0f, 5250.0f);
		instances[66].position = XMFLOAT3(100.0f, 300.0f, 5250.0f);
		instances[67].position = XMFLOAT3(200.0f, 300.0f, 5250.0f);

		instances[68].position = XMFLOAT3(0.0f, 300.0f, 5350.0f);
		instances[69].position = XMFLOAT3(-100.0f, 300.0f, 5350.0f);
		instances[70].position = XMFLOAT3(100.0f, 300.0f, 5350.0f);
		instances[71].position = XMFLOAT3(200.0f, 300.0f, 5350.0f);

		instances[72].position = XMFLOAT3(0.0f, 300.0f, 5450.0f);
		instances[73].position = XMFLOAT3(-100.0f, 300.0f, 5450.0f);
		instances[74].position = XMFLOAT3(100.0f, 300.0f, 5450.0f);
		instances[75].position = XMFLOAT3(200.0f, 300.0f, 5450.0f);

		instances[76].position = XMFLOAT3(0.0f, 300.0f, 5550.0f);
		instances[77].position = XMFLOAT3(-100.0f, 300.0f, 5550.0f);
		instances[78].position = XMFLOAT3(100.0f, 300.0f, 5550.0f);
		instances[79].position = XMFLOAT3(200.0f, 300.0f, 5550.0f);

		//5층
		instances[80].position = XMFLOAT3(0.0f, 400.0f, 5150.0f);
		instances[81].position = XMFLOAT3(-100.0f, 400.0f, 5150.0f);
		instances[82].position = XMFLOAT3(100.0f, 400.0f, 5150.0f);
		instances[83].position = XMFLOAT3(200.0f, 400.0f, 5150.0f);

		instances[84].position = XMFLOAT3(0.0f, 400.0f, 5250.0f);
		instances[85].position = XMFLOAT3(-100.0f, 400.0f, 5250.0f);
		instances[86].position = XMFLOAT3(100.0f, 400.0f, 5250.0f);
		instances[87].position = XMFLOAT3(200.0f, 400.0f, 5250.0f);

		instances[88].position = XMFLOAT3(0.0f, 400.0f, 5350.0f);
		instances[89].position = XMFLOAT3(-100.0f, 400.0f, 5350.0f);
		instances[90].position = XMFLOAT3(100.0f, 400.0f, 5350.0f);
		instances[91].position = XMFLOAT3(200.0f, 400.0f, 5350.0f);

		instances[92].position = XMFLOAT3(0.0f, 400.0f, 5450.0f);
		instances[93].position = XMFLOAT3(-100.0f, 400.0f, 5450.0f);
		instances[94].position = XMFLOAT3(100.0f, 400.0f, 5450.0f);
		instances[95].position = XMFLOAT3(200.0f, 400.0f, 5450.0f);
	
		instances[96].position = XMFLOAT3(0.0f, 400.0f, 5550.0f);
		instances[97].position = XMFLOAT3(-100.0f, 400.0f, 5550.0f);
		instances[98].position = XMFLOAT3(100.0f, 400.0f, 5550.0f);
		instances[99].position = XMFLOAT3(200.0f, 400.0f, 5550.0f);

		//6층
		instances[100].position = XMFLOAT3(0.0f, 500.0f, 5250.0f);
		instances[101].position = XMFLOAT3(-100.0f, 500.0f, 5250.0f);
		instances[102].position = XMFLOAT3(100.0f, 500.0f, 5250.0f);
		instances[103].position = XMFLOAT3(200.0f, 500.0f, 5250.0f);

		instances[104].position = XMFLOAT3(0.0f, 500.0f, 5350.0f);
		instances[105].position = XMFLOAT3(-100.0f, 500.0f, 5350.0f);
		instances[106].position = XMFLOAT3(100.0f, 500.0f, 5350.0f);
		instances[107].position = XMFLOAT3(200.0f, 500.0f, 5350.0f);

		instances[108].position = XMFLOAT3(0.0f, 500.0f, 5450.0f);
		instances[109].position = XMFLOAT3(-100.0f, 500.0f, 5450.0f);
		instances[110].position = XMFLOAT3(100.0f, 500.0f, 5450.0f);
		instances[111].position = XMFLOAT3(200.0f, 500.0f, 5450.0f);

		instances[112].position = XMFLOAT3(0.0f, 500.0f, 5550.0f);
		instances[113].position = XMFLOAT3(-100.0f, 500.0f, 5550.0f);
		instances[114].position = XMFLOAT3(100.0f, 500.0f, 5550.0f);
		instances[115].position = XMFLOAT3(200.0f, 500.0f, 5550.0f);

		//7층
		instances[116].position = XMFLOAT3(0.0f, 600.0f, 5350.0f);
		instances[117].position = XMFLOAT3(-100.0f, 600.0f, 5350.0f);
		instances[118].position = XMFLOAT3(100.0f, 600.0f, 5350.0f);
		instances[119].position = XMFLOAT3(200.0f, 600.0f, 5350.0f);

		instances[120].position = XMFLOAT3(0.0f, 600.0f, 5450.0f);
		instances[121].position = XMFLOAT3(-100.0f, 600.0f, 5450.0f);
		instances[122].position = XMFLOAT3(100.0f, 600.0f, 5450.0f);
		instances[123].position = XMFLOAT3(200.0f, 600.0f, 5450.0f);

		instances[124].position = XMFLOAT3(0.0f, 600.0f, 5550.0f);
		instances[125].position = XMFLOAT3(-100.0f, 600.0f, 5550.0f);
		instances[126].position = XMFLOAT3(100.0f, 600.0f, 5550.0f);
		instances[126].position = XMFLOAT3(200.0f, 600.0f, 5550.0f);

		//8층
		instances[127].position = XMFLOAT3(0.0f, 700.0f, 5450.0f);
		instances[128].position = XMFLOAT3(-100.0f, 700.0f, 5450.0f);
		instances[129].position = XMFLOAT3(100.0f, 700.0f, 5450.0f);
		instances[130].position = XMFLOAT3(200.0f, 700.0f, 5450.0f);

		instances[131].position = XMFLOAT3(0.0f, 700.0f, 5550.0f);
		instances[132].position = XMFLOAT3(-100.0f, 700.0f, 5550.0f);
		instances[133].position = XMFLOAT3(100.0f, 700.0f, 5550.0f);
		instances[134].position = XMFLOAT3(200.0f, 700.0f, 5550.0f);
	}

	//question cube
	else if (m_instanceName == "QCube")
	{
		//on ground1
		instances[0].position = XMFLOAT3(0.0f, 1.9f, -29.18f);
		instances[1].position = XMFLOAT3(0.0f, 1.9f, -27.2f);
		instances[2].position = XMFLOAT3(0.0f, 4.56f, -28.18f);
		instances[3].position = XMFLOAT3(0.0f, 1.9f, -34.0f);

		//on ground2
		instances[4].position = XMFLOAT3(0.0f, 1.9f, 2.9f);

		//on ground3
		instances[5].position = XMFLOAT3(0.0f, 4.56f, 15.35f);

		instances[6].position = XMFLOAT3(0.0f, 1.9f, 24.0f);
		instances[7].position = XMFLOAT3(0.0f, 1.9f, 26.5f);
		instances[8].position = XMFLOAT3(0.0f, 1.9f, 29.0f);
		instances[9].position = XMFLOAT3(0.0f, 4.56f, 26.5f);

		instances[10].position = XMFLOAT3(0.0f, 4.56f, 41.07f);
		instances[11].position = XMFLOAT3(0.0f, 4.56f, 42.085f);

		//on ground4
		instances[12].position = XMFLOAT3(0.0f, 1.9f, 72.625f);


	}

	//plant
	else if (m_instanceName == "Plant")
	{
		instances[0].position = XMFLOAT3(1.0f, 10.0f, -115.2f);
	}

	//goomba
	else if (m_instanceName == "Goomba")
	{
		//on ground1
		/*for (int i = 0; i < 14; i++)
		{
			if (i == 0)
			{
				GoombaPos[i] = XMFLOAT3(1325.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			else if (i == 1)
			{
				GoombaPos[i] = XMFLOAT3(800.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			else if (i == 2)
			{
				GoombaPos[i] = XMFLOAT3(600.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			else if (i == 3)
			{
				GoombaPos[i] = XMFLOAT3(550.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}

			else if (i == 4)
			{
				GoombaPos[i] = XMFLOAT3(-300.0f, 240.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			else if (i == 5)
			{
				GoombaPos[i] = XMFLOAT3(-350.0f, 240.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			
			else if (i == 6)
			{
				GoombaPos[i] = XMFLOAT3(-700.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			else if (i == 7)
			{
				GoombaPos[i] = XMFLOAT3(-750.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}

			else if (i == 8)
			{
				GoombaPos[i] = XMFLOAT3(-1550.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			else if (i == 9)
			{
				GoombaPos[i] = XMFLOAT3(-1600.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			else if (i == 10)
			{
				GoombaPos[i] = XMFLOAT3(-1650.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			else if (i == 11)
			{
				GoombaPos[i] = XMFLOAT3(-1650.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}

			else if (i == 12)
			{
				GoombaPos[i] = XMFLOAT3(-3400.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
			else if (i == 13)
			{
				GoombaPos[i] = XMFLOAT3(-3450.0f, -4.0f + goombadeadpos[i], 0.0f);
				instances[i].position = GoombaPos[i];
			}
		}*/
		instances[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	}

	//koopatroopa
	else if (m_instanceName == "KoopaTroopa")
	{
		//instances[0].position = XMFLOAT3(0.0f, -65.0f, 3800.0f);
		instances[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	//castle
	else if (m_instanceName == "Castle")
	{
		instances[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	//flag
	else if (m_instanceName == "Flag")
	{
		instances[0].position = XMFLOAT3(-86.0f, 0.0f, 0.0f);
	}

	//flagcube
	else if (m_instanceName == "Flagcube")
	{
		instances[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	
	//skybox
	else if (m_instanceName == "Skybox")
	{
		instances[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	// 인스턴스 버퍼의 설명을 설정합니다.
	D3D11_BUFFER_DESC instanceBufferDesc;
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인스턴스 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA instanceData;
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// 인스턴스 버퍼를 만듭니다.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	delete[] instances;
	instances = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 버퍼 오프셋과 스트라이드를 설정합니다.
	unsigned int strides[2] = { sizeof(VertexType), sizeof(InstanceType) };
	unsigned int offsets[2] = { 0, 0 };

	// 포인터의 배열을 정점 버퍼와 인스턴스 버퍼로 설정합니다.
	ID3D11Buffer* bufferPointers[2] = { m_vertexBuffer, m_instanceBuffer };

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 이 꼭지점 버퍼에서 렌더링되어야하는 프리미티브 유형을 설정합니다.이 경우에는 삼각형입니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(const WCHAR* filename)
{
	ReadFileCounts(filename);

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool ModelClass::ReadFileCounts(const WCHAR* filename)
{
	ifstream fin;
	char input;
	// Initialize the counts.
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;
	// Open the file.
	fin.open(filename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}
	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();

	LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);

	return true;
}

bool ModelClass::LoadDataStructures(const WCHAR* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	XMFLOAT3* vertices, * texcoords, * normals;
	FaceType* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the four data structures.
	vertices = new XMFLOAT3[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new XMFLOAT3[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new XMFLOAT3[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >>
					vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >>
					normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >>
					input2 >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2 >>
					faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >>
					faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	//// Close the file.
	//fin.close();
	//// Open the output file.
	//fout.open("model.txt");
	//// Write out the file header that our model format uses.
	//fout << "Vertex Count: " << (faceCount * 3) << endl;
	//fout << endl;
	//fout << "Data:" << endl;
	//fout << endl;

	m_vertexCount = faceCount * 3;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3].x = vertices[vIndex].x;
		m_model[i * 3].y = vertices[vIndex].y;
		m_model[i * 3].z = vertices[vIndex].z;

		m_model[i * 3].tu = texcoords[tIndex].x;
		m_model[i * 3].tv = texcoords[tIndex].y;

		m_model[i * 3].nx = normals[nIndex].x;
		m_model[i * 3].ny = normals[nIndex].y;
		m_model[i * 3].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3 + 1].x = vertices[vIndex].x;
		m_model[i * 3 + 1].y = vertices[vIndex].y;
		m_model[i * 3 + 1].z = vertices[vIndex].z;

		m_model[i * 3 + 1].tu = texcoords[tIndex].x;
		m_model[i * 3 + 1].tv = texcoords[tIndex].y;

		m_model[i * 3 + 1].nx = normals[nIndex].x;
		m_model[i * 3 + 1].ny = normals[nIndex].y;
		m_model[i * 3 + 1].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3 + 2].x = vertices[vIndex].x;
		m_model[i * 3 + 2].y = vertices[vIndex].y;
		m_model[i * 3 + 2].z = vertices[vIndex].z;

		m_model[i * 3 + 2].tu = texcoords[tIndex].x;
		m_model[i * 3 + 2].tv = texcoords[tIndex].y;

		m_model[i * 3 + 2].nx = normals[nIndex].x;
		m_model[i * 3 + 2].ny = normals[nIndex].y;
		m_model[i * 3 + 2].nz = normals[nIndex].z;
	}

	//// Close the output file.
	//fout.close();

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals)
	{
		delete[] normals;
		normals = 0;
	}

	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}

XMFLOAT3 ModelClass::GoombaPosSave(XMFLOAT3 pos, int num)
{
	pos = GoombaPos[num];

	pos.x = 0;
	pos.y = 0;
	pos.z = GoombaPos[num].x * (-0.07);

	return pos;
}

void ModelClass::CheckGoombalive(bool goombalive, int num)
{
	if (!goombalive)
	{
		goombadeadpos[num] = 10000.0f;
	}
}

bool ModelClass::UpdateGoombaInstanceBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}


	InstanceType* instances = new InstanceType[14];
	if (!instances)
	{
		return false;
	}


	for (int i = 0; i < 14; i++)
	{
		if (i == 0)
		{
			GoombaPos[i] = XMFLOAT3(1325.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
		else if (i == 1)
		{
			GoombaPos[i] = XMFLOAT3(800.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
		else if (i == 2)
		{
			GoombaPos[i] = XMFLOAT3(600.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
		else if (i == 3)
		{
			GoombaPos[i] = XMFLOAT3(550.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}

		else if (i == 4)
		{
			GoombaPos[i] = XMFLOAT3(-300.0f, 240.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
		else if (i == 5)
		{
			GoombaPos[i] = XMFLOAT3(-350.0f, 240.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}

		else if (i == 6)
		{
			GoombaPos[i] = XMFLOAT3(-700.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
		else if (i == 7)
		{
			GoombaPos[i] = XMFLOAT3(-750.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}

		else if (i == 8)
		{
			GoombaPos[i] = XMFLOAT3(-1550.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
		else if (i == 9)
		{
			GoombaPos[i] = XMFLOAT3(-1600.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
		else if (i == 10)
		{
			GoombaPos[i] = XMFLOAT3(-1650.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
		else if (i == 11)
		{
			GoombaPos[i] = XMFLOAT3(-1650.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}

		else if (i == 12)
		{
			GoombaPos[i] = XMFLOAT3(-3400.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
		else if (i == 13)
		{
			GoombaPos[i] = XMFLOAT3(-3450.0f, -4.0f + goombadeadpos[i], 0.0f);
			instances[i].position = GoombaPos[i];
		}
	}

	// 인스턴스 버퍼의 설명을 설정합니다.
	D3D11_BUFFER_DESC instanceBufferDesc;
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인스턴스 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA instanceData;
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// 인스턴스 버퍼를 만듭니다.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	delete[] instances;
	instances = 0;

	return true;
}