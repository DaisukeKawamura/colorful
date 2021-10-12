#pragma once
class HP
{
public://メンバ関数

	// コンストラクタ
	HP() ;
	// デストラクタ
	~HP() {};

	//初期化  //最初のHP、最小HP,最大HP
	void Init(float CurrenHP, float minHP, float maxHP);

	//最大HP取得
	float GetMaxHP() const;
	//最小HP取得
	float GetMinHP() const;
	//現在のHP取得
	float GetCurrentHP() const;

	//HP減少
	void AddDamage(float value);

	//HP回復
	void RecoverDamage(float value);

	//最大HPの設定
	void SetMaxHP(float value);

	//HPが空
	bool isEmpty();

private:
	//HPが最大値、最小値を超えないように
	void NormalizeHP();

private://メンバ変数
	float P_MaxHP;     // 最大HP
	float P_MinHP;     // 最小HP
	float P_CurrentHP;// 現在のHP

};