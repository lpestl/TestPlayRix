#pragma once

// ����� ��������� 
class Fireworks
{
public:
	// ����������� �� ��������� � �������� ����������
	Fireworks();
	// ����������� � �����������
	Fireworks(FPoint startPosition,  // ����� ������ ������
		FPoint speed,	// ��������� ������ ��������
		int level,		// ������������ ������� ������������ ������
		int curLevel,	// ������� ��������� ��������
		int countChild, // ���������� ������� �������
		float timeExplosion, // ����� �� ������
		EffectsContainer &effCont); // ��������� � ���������
	
	void Draw();
	void Update(float dt, FPoint speedParent, EffectsContainer &effCont);

	// ����� (������) �������
	void Start(EffectsContainer &effCont);
	// ���������� ������� �������
	void setPosition(FPoint pos);
	// ���������, ��� �� �������� ������ ������.
	bool allChildrenHidden();
	// ���������� ������
	void Finish();
	// �������� ������� �������
	FPoint getPosition();
	// �������� ���������� �������� �������� �����
	int getCountFire();

private:
	// ������������� ����������
	//void Init();

	// ������ ��������
	FPoint _speed;
	// ������ ������� �������� � ��������� ������
	float _startSpeed;
	// ������� ������� �������
	FPoint _position;
	// ���������� ����������
	float _g;
	// ������� ��������� ��������
	int _curLevel;
	// ����������� �������
	float _kTime;
	// ������ ��������
	int _level;
	// ���������� �������� �������
	int _countChild;
	// ���� ������
	bool _boom;
	// ����� �� ������
	float _timeExplosion;
	// ����� ����� �������������
	float _totalTime;
	Render::Texture *_star;

	// ������ � ��������� ��������
	std::vector <Fireworks *> _childFires;

	ParticleEffectPtr _eff;
};

