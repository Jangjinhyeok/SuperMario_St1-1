#include "Collision.h"

Collision::Collision()
{
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_size = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_max = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_min = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Collision::~Collision()
{
}

void Collision::InitAABB(/*Tag tag,*/ XMFLOAT3 pos, XMFLOAT3 size)
{
	//m_tag = tag;
	m_pos = pos;
	m_size = size;

	m_max = XMFLOAT3(m_pos.x + m_size.x, m_pos.y + m_size.y, m_pos.z + m_size.z);
	m_min = XMFLOAT3(m_pos.x - m_size.x, m_pos.y, m_pos.z - m_size.z);
}

void Collision::UpdateAABB(XMFLOAT3 pos)
{
	m_pos = pos;

	m_max = XMFLOAT3(m_pos.x + m_size.x, m_pos.y + m_size.y, m_pos.z + m_size.z);
	m_min = XMFLOAT3(m_pos.x - m_size.x, m_pos.y - m_size.y, m_pos.z - m_size.z);
}

void Collision::UpdateAABB(float x, float y, float z)
{
	m_pos = XMFLOAT3(x, y, z);

	m_max = XMFLOAT3(m_pos.x + m_size.x, m_pos.y + m_size.y, m_pos.z + m_size.z);
	m_min = XMFLOAT3(m_pos.x - m_size.x, m_pos.y - m_size.y, m_pos.z - m_size.z);
}


bool Collision::CheckAABB(Collision* col)
{
	if (m_max.x < col->GetMin().x || m_min.x > col->GetMax().x) return false;
	if (m_max.y < col->GetMin().y || m_min.y > col->GetMax().y) return false;
	if (m_max.z < col->GetMin().z || m_min.z > col->GetMax().z) return false;

	return true;
}

//bool Collision::CheckCollision(Collision* col)
//{
//	if (CheckAABB(col))
//	{
//		return true;
//	}
//
//	return false;
//}