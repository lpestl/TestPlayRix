#pragma once

#include <vector>

//------------------------ Target Class --------------------------
// ����� ������
class Target {
public:
	// ����������� � ���������� ��������� �������
	Target(FPoint position);
	~Target();

	void Draw();
	void Update(float dt);

	// �������� ������� ������
	FPoint getPosition();
	// ��������� ��������� � ������
	bool chekHit(FPoint posArrow, EffectsContainer &effCont);
	// �������� �������� ���������
	bool getHit();

private:
	// �������� ������
	Render::Texture *_trg;
	// ������� ������
	FPoint _position;
	// �������� 
	float _speed;
	// ������� ��������
	float _scale;
	// ������ ��������
	FPoint _speedVector;
	// ���� "���������"
	bool _hitting;

};

//-------------------------- TargetsRifle Class ----------------
// ����� ���������� ��������

class TargetsRifle
{
public:
	// ����������� � ����������� ���������� ������� � ����������� �� �������
	TargetsRifle(int count, float timeLimit);
	// �������������
	void Init();

	void Draw();
	void Update(float dt);
	// �������� ��������� ����� � ������
	bool ChekingHits(FPoint posArrow, EffectsContainer &effCont);

	// ������� ������� �������
	void Clear();
	// �������� ���������� ������� � ������� ������
	int GetCountTargetNow();

private:
	// ������ � ��������
	std::vector <Target*> _targets;
	// ����������
	int _count;
	// ����������� �� �������
	float _timeLimit;
};

