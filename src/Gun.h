#pragma once

#include "TargetsRifle.h"
#include <vector>

// ----------------------------- Arrow Class ----------------------
// ����� "������". ������ ������� �������� �� ������
class Arrow
{
public:
	// ����������� � ����������� - ����������� �������� ��������, 
	// �������� ������ ������� � ���� ������� ��������
	Arrow(float scale, float speed, float angle, EffectsContainer &effCont);
	// ����������
	~Arrow();

	void Draw();
	void Update(float dt);

	// ���������� �������� ����� "���������"
	void setHit(bool hit);
	// �������� �������� ����� "���������"
	bool getHit();
	// �������� ������� �������
	FPoint getPosition();
	// ������� ������
	void KillEffect();

private:
	void Init();

private:
	// �������� ������
	Render::Texture *_arrow;
	// �������
	FPoint _position;
	// ���� �������� ��������
	float _angle;
	// ������� ��������
	float _scale;
	// �������� ������� (���� / ���)
	float _speed;
	// ���� "���������"
	bool _isHit;
	// ����������� �������
	int _kTime;

	ParticleEffectPtr _eff;
};

// -------------------------- Gun Class -----------------------------
// ����� "������". ������� ����� ������
class Gun
{
public:
	Gun();

	void Draw();
	void Update(float dt);
	// ������� ������ ����
	bool MouseDown(const IPoint& mouse_pos, int speed, EffectsContainer &effCont);
	// �������� ���������� ���������� �����
	int getCountArrow();
	// ������� ��� ������
	void ClearArrows();
	// �������� ��������� � ������
	void CheckTargetsHit(TargetsRifle *targetsRuf, EffectsContainer &effCont);
	// ������� ������ �� �������
	void DeletArrow(unsigned n);

private:
	void Init();

private:
	// �������� ��������
	Render::Texture *_arb;
	// ��������� ��������
	IPoint _posArb;
	// ������� ��������
	float _scale;
	// ���� �������� ��������
	float _angle;
	// ������ �� ��������
	std::vector<Arrow *> _arrows;
};

