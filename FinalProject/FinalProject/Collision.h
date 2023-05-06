#pragma once

#include"d3dclass.h"

class Collision
{
public :
	//enum Tag {Col_Player, Col_Zombie, Col_Cannon , Col_CannonITem, Col_HPItem, Col_End};

public :
	Collision();
	~Collision();

	void InitAABB(/*Tag,*/ XMFLOAT3, XMFLOAT3);
	void UpdateAABB(XMFLOAT3);
	void UpdateAABB(float, float, float);
	bool CheckAABB(Collision*);

	//Tag GetTag() { return m_tag; }
	XMFLOAT3 GeTPos() { return m_pos; }
	XMFLOAT3 GetMax() { return m_max; }
	XMFLOAT3 GetMin() { return m_min; }
	XMFLOAT3 GetSize() { return m_size; }

private :
	XMFLOAT3 m_pos;
	XMFLOAT3 m_size;
	XMFLOAT3 m_max;
	XMFLOAT3 m_min;

	//Tag m_tag;

};