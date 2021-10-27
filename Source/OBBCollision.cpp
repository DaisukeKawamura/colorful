#include "./Header/OBBCollision.h"


void OBB::Initilize(XMFLOAT3 pos, XMMATRIX rotM, FLOAT xLength, FLOAT yLength, FLOAT zLength)
{
	//���肷��OBB�̏����R�s�[
	m_Pos.x = pos.x;
	m_Pos.y = pos.y;
	m_Pos.z = pos.z;

	m_NormaDirect[0].x = rotM.r[0].m128_f32[0];
	m_NormaDirect[0].y = rotM.r[0].m128_f32[1];
	m_NormaDirect[0].z = rotM.r[0].m128_f32[2];

	m_NormaDirect[1].x = rotM.r[1].m128_f32[0];
	m_NormaDirect[1].y = rotM.r[1].m128_f32[1];
	m_NormaDirect[1].z = rotM.r[1].m128_f32[2];

	m_NormaDirect[2].x = rotM.r[2].m128_f32[0];
	m_NormaDirect[2].y = rotM.r[2].m128_f32[1];
	m_NormaDirect[2].z = rotM.r[2].m128_f32[2];

	m_fLength[0] = xLength;
	m_fLength[1] = yLength;
	m_fLength[2] = zLength;
}


Vector3 OBB::GetDirect(int elem)
{
	switch (elem)
	{
	case 0:
		return m_NormaDirect[0];
		break;
	case 1:
		return  m_NormaDirect[1];
		break;
	case 2:
		return  m_NormaDirect[2];
		break;
	default:
		return Vector3();
		break;
	}
}

FLOAT OBB::GetLen_W(int elem)
{
	switch (elem)
	{
	case 0:
		return  m_fLength[0];
		break;
	case 1:
		return  m_fLength[1];
		break;
	case 2:
		return  m_fLength[2];
		break;
	default:
		return 0;
		break;
	}
}

//���W���擾
Vector3 OBB::GetPos_W()
{
	return m_Pos;
}

// OBB v.s. OBB
bool OBBCollision::ColOBBs(OBB &obb1, OBB &obb2)
{
	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	Vector3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W(0);
	Vector3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
	Vector3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
	Vector3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W(0);
	Vector3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
	Vector3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
	Vector3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

	// ������ : Ae1
	FLOAT rA = Vector3::length(Ae1);
	FLOAT rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	FLOAT L = fabsf(Vector3::dot(Interval, NAe1));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�

	 // ������ : Ae2
	rA = Vector3::length(Ae2);
	rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabsf(Vector3::dot(Interval, NAe2));
	if (L > rA + rB)
		return false;

	// ������ : Ae3
	rA = Vector3::length(Ae3);
	rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabsf(Vector3::dot(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// ������ : Be1
	rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = Vector3::length(Be1);
	L = fabsf(Vector3::dot(Interval, NBe1));
	if (L > rA + rB)
		return false;

	// ������ : Be2
	rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = Vector3::length(Be2);
	L = fabsf(Vector3::dot(Interval, NBe2));
	if (L > rA + rB)
		return false;

	// ������ : Be3
	rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = Vector3::length(Be3);
	L = fabsf(Vector3::dot(Interval, NBe3));
	if (L > rA + rB)
		return false;

	// ������ : C11
	Vector3 Cross;
	Cross = Vector3::cross(NAe1, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Vector3::dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	Cross = Vector3::cross(NAe1, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Vector3::dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	Cross = Vector3::cross(NAe1, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Vector3::dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	Cross = Vector3::cross(NAe2, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Vector3::dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	Cross = Vector3::cross(NAe2, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Vector3::dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	Cross = Vector3::cross(NAe2, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Vector3::dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	Cross = Vector3::cross(NAe3, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Vector3::dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	Cross = Vector3::cross(NAe3, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Vector3::dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	Cross = Vector3::cross(NAe3, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Vector3::dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}


// �������ɓ��e���ꂽ���������瓊�e���������Z�o
FLOAT OBBCollision::LenSegOnSeparateAxis(Vector3 &Sep, Vector3 &e1, Vector3 &e2, Vector3 e3)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	FLOAT r1 = fabsf(Vector3::dot(Sep, e1));
	FLOAT r2 = fabsf(Vector3::dot(Sep, e2));
	FLOAT r3 = &e3 ? (fabsf(Vector3::dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}

void OBBCollision::PushbackPolygon(XMFLOAT3 &position, const XMFLOAT3 &oldPosition, OBB &player, OBB &block, bool &HitDown, const int up, const int down)
{
	//�v���C���[���W
	float PRight = position.x + player.GetLen_W(0), PLeft = position.x - player.GetLen_W(0);
	float PUp = position.y + player.GetLen_W(1), PDown = position.y - player.GetLen_W(1);
	//�v���C���[�̑O�̍��W
	float oldRight = oldPosition.x + player.GetLen_W(0), oldLeft = oldPosition.x - player.GetLen_W(0);
	float oldUp = oldPosition.y + player.GetLen_W(1), oldDown = oldPosition.y - player.GetLen_W(1);
	//�u���b�N���W
	Vector3 BPos = block.GetPos_W();
	float BRight = BPos.x + block.GetLen_W(0), BLeft = BPos.x - block.GetLen_W(0);
	float BUp = BPos.y + block.GetLen_W(1), BDown = BPos.y - block.GetLen_W(1);

	//����
	if (oldLeft >= BRight && oldUp <= BDown)
	{
		if (up == BLOCK || up == RED_BLOCK || up == BLUE_BLOCK || up == GREEN_BLOCK || up == YELLOW_BLOCK || up == NOPAINT_BLOCK)
		{
		}
		//��Ƀu���b�N���Ȃ��������D��
		else if (oldUp <= BDown)
		{
			position.y = BDown - player.GetLen_W(1);
		}
	}
	//����
	else if (oldLeft >= BRight && oldDown >= BUp)
	{
		if (down == BLOCK || down == RED_BLOCK || down == BLUE_BLOCK || down == GREEN_BLOCK || down == YELLOW_BLOCK || up == NOPAINT_BLOCK)
		{
		}
		//���Ƀu���b�N���Ȃ������牺�D��
		else if (oldDown >= BUp)
		{
			position.y = BUp + player.GetLen_W(1);
		}
	}
	//�E��
	else if (oldRight <= BLeft && oldUp <= BDown)
	{
		if (up == BLOCK || up == RED_BLOCK || up == BLUE_BLOCK || up == GREEN_BLOCK || up == YELLOW_BLOCK || up == NOPAINT_BLOCK)
		{
		}
		//��Ƀu���b�N���Ȃ��������D��
		else if (oldUp <= BDown)
		{
			position.y = BDown - player.GetLen_W(1);
		}
	}
	//�E��
	else if (oldRight <= BLeft && oldDown >= BUp)
	{
		if (down == BLOCK || down == RED_BLOCK || down == BLUE_BLOCK || down == GREEN_BLOCK || down == YELLOW_BLOCK || up == NOPAINT_BLOCK)
		{
		}
		//���Ƀu���b�N���Ȃ������牺�D��
		else if (oldDown >= BUp)
		{
			position.y = BUp + player.GetLen_W(1);
		}
	}
	else
	{
		//�E
		if (oldRight <= BLeft)
		{
			position.x = BLeft - player.GetLen_W(0);
		}
		//��
		if (oldLeft >= BRight)
		{
			position.x = BRight + player.GetLen_W(0);
		}
		//��
		if (oldUp <= BDown)
		{
			position.y = BDown - player.GetLen_W(1);
		}
		//��
		if (oldDown >= BUp)
		{
			position.y = BUp + player.GetLen_W(1);
			HitDown = true;
		}
	}
}

void OBBCollision::PushbackFloor(XMFLOAT3 &position, const XMFLOAT3 oldPosition, OBB &player, OBB &block, bool &HitDown)
{
	//�v���C���[���W
	float PRight = position.x + player.GetLen_W(0), PLeft = position.x - player.GetLen_W(0);
	float PUp = position.y + player.GetLen_W(1), PDown = position.y - player.GetLen_W(1);
	//�v���C���[�̑O�̍��W
	float oldRight = oldPosition.x + player.GetLen_W(0), oldLeft = oldPosition.x - player.GetLen_W(0);
	float oldUp = oldPosition.y + player.GetLen_W(1), oldDown = oldPosition.y - player.GetLen_W(1);
	//�u���b�N���W
	Vector3 BPos = block.GetPos_W();
	float BRight = BPos.x + block.GetLen_W(0), BLeft = BPos.x - block.GetLen_W(0);
	float BUp = BPos.y + block.GetLen_W(1), BDown = BPos.y - block.GetLen_W(1);

	//����
	if (oldLeft >= BRight && oldDown >= BUp)
	{
		//���Ƀu���b�N���Ȃ������牺�D��
		if (oldDown >= BUp)
		{
			position.y = BUp + player.GetLen_W(1);
		}
	}

	//�E��
	else if (oldRight <= BLeft && oldDown >= BUp)
	{
		//���Ƀu���b�N���Ȃ������牺�D��
		if (oldDown >= BUp)
		{
			position.y = BUp + player.GetLen_W(1);
		}
	}
	else
	{
		//��
		if (oldLeft >= BRight)
		{
			position.x = BRight + player.GetLen_W(0);
		}
		//��
		if (oldDown >= BUp)
		{
			position.y = BUp + player.GetLen_W(1);
			HitDown = true;
		}
	}
}