#include "HP.h"

HP::HP() :
	P_CurrentHP{},
	P_MaxHP{},
	P_MinHP{}
{
}

void HP::Init(float CurrenHP, float minHP, float maxHP)
{
	P_CurrentHP = CurrenHP;
	P_MinHP = minHP;
	P_MaxHP = maxHP;
}

float HP::GetMaxHP() const
{
	return P_MaxHP;
}

float HP::GetMinHP() const
{
	return P_MinHP;
}

float HP::GetCurrentHP() const
{
	return P_CurrentHP;
}

void HP::NormalizeHP()
{
	if (P_CurrentHP < P_MinHP) {
		P_CurrentHP = P_MinHP;
	}
	else if (P_CurrentHP > P_MaxHP) {
		P_CurrentHP = P_MaxHP;
	}
}

void HP::AddDamage(float value)
{
	P_CurrentHP -= value;
	NormalizeHP();
}

void HP::RecoverDamage(float value)
{
	P_CurrentHP += value;
	NormalizeHP();
}

void HP::SetMaxHP(float value)
{
	P_MaxHP = value;
	NormalizeHP();
}

bool HP::isEmpty()
{
	return (GetCurrentHP() <= P_MinHP);   // Å¬’lˆÈ‰º
}
