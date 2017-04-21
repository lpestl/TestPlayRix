#include "stdafx.h"
#include "Fireworks.h"

Fireworks::Fireworks()
	: _eff(NULL)
	, _g(-9.8f)
	, _position(0, 0)
	, _speed(0, 0)
	, _curLevel(0)
	, _level(1)
	, _countChild(5)
	, _kTime(10)
	, _timeExplosion(0)
	, _totalTime(0)
	, _startSpeed(0)
	, _boom(false)
	, _star(NULL)
{
	//Init();
}

// ���������� � �����������
Fireworks::Fireworks(FPoint startPosition, FPoint speed, int level, int curLevel, int countChild, float timeExplosion, EffectsContainer &effCont)
	: _g(-9.8f)
	, _kTime(5)
	, _totalTime(0)
	, _boom(false)
	, _star(NULL)
{
	// ������ ��������� �������� �� ���������� ������������
	_level = level;
	_curLevel = curLevel;
	_countChild = countChild;
	_speed = speed;
	_position = startPosition;
	_timeExplosion = timeExplosion;
	_startSpeed = sqrtf(_speed.y * _speed.y + _speed.x * _speed.x);
	
	_star = Core::resourceManager.Get<Render::Texture>("star");

	_eff = NULL;

	// ��������� ��������� ��������. �������������� ����� ��� �������� �������
	// �����: ������� ��� � ����� �����������. ��� ���� ����� ������� �������� �� �������� �������� �� ����� ������
	if (_curLevel < _level) {
		for (int i = 0; i < _countChild; i++) {
			// ������� ������ �������� ��� �������� ������� � ����������� �� �� ����������
			// ��� ���� ����� ��� ���������� ����������� �� �����
			float dxSp = _startSpeed * 2 / 3 * cosf(2 * math::PI / _countChild * i + math::PI / 2);// +_speed.x;
			float dySp = _startSpeed * 2 / 3 * sinf(2 * math::PI / _countChild * i + math::PI / 2);// +_speed.y;

			// ������� �������� ��������
			Fireworks *newChild = new Fireworks(_position, FPoint(dxSp, dySp), _level, _curLevel + 1, _countChild, _timeExplosion /* 2 / 3*/, effCont);
			_childFires.push_back(newChild);
		}
	}
}

// ��������� ������
void Fireworks::Start(EffectsContainer &effCont) {
	_eff = effCont.AddEffect("FireWorks");
	setPosition(_position);
}

// ���������� �������
void Fireworks::setPosition(FPoint pos) {
	_position = pos;
	if (_eff) {
		_eff->SetPos(pos);
		_eff->Reset();
	}
}

// ��������� ������
void Fireworks::Finish() {
	_eff->Finish();
	_eff = NULL;
}

// ����������� ���������� �������� �������� + 1 �������
int Fireworks::getCountFire() {
	int result = 1;
	for (unsigned i = 0; i < _childFires.size(); i++) {
		result += _childFires[i]->getCountFire();
	}
	return result;
}

// �������� �������
FPoint Fireworks::getPosition() {
	return _position;
}

// �������� ��������� ���� �������� �������� 
bool Fireworks::allChildrenHidden() {
	if (_curLevel < _level) {
		bool res = true;
		// ���� �� ���� �������� �������
		for (unsigned i = 0; i < _childFires.size(); i++) {
			// ���� ���� ���� ����� �����, �� � ������� �������� �������
			if (!_childFires[i]->allChildrenHidden()) {
				res = false;
			}
			// ���� � ������ ��� �������� ������ �� ������ � �� ��� �������� ��� ���� ���������
			if ((_childFires[i]->allChildrenHidden()) && (_childFires[i]->getPosition().y < 0)) {
				// �� ��������� ������ �������� �������
				_childFires[i]->Finish();
				// � ������� ��� �� ������
				delete _childFires[i];
				// � �� �������
				_childFires.erase(_childFires.begin() + i);
			}
		}
		return res;
	}
	else {
		// ���� ����� ��� ���� ��������� �� ���������� TRUE
		if (_position.y < 0) {
			return true;
		}
		else {
			return false;
		}
	}
}

void Fireworks::Draw() {
	// ���� ����� ��� ���
	if (_boom) {
		for (unsigned i = 0; i < _childFires.size(); i++) {
			// �� ������������ �������� ������
			_childFires[i]->Draw();
		}			
	}
	// ������ ������� ������
	if (_eff) {
		Render::device.PushMatrix();
		// �������� �� ������� �������
		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
		// ������������ �� ������ ����
		//Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

		// �������� ������ ��������
		IRect texRect = _star->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_star->TranslateUV(rect, uv);

		// ������������� ������� ��������
		Render::device.MatrixScale(0.5f);
		// �������� d ����� ��������
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height / 2, 0.f));

		// ���������� ��������
		_star->Bind();
		Render::DrawQuad(rect, uv);

		Render::device.PopMatrix();
	}
}

void Fireworks::Update(float dt, FPoint speedParent, EffectsContainer &effCont) {
	// � ���� ������ �� ������
	if (_eff)
	{
		// ������� ��� ��������� ��������� � ������ ���� ����������
		float dx = _speed.x * dt * _kTime + speedParent.x;
		float dy = _speed.y * dt * _kTime + _g * dt * dt * _kTime * _kTime / 2 + speedParent.y;
		
		// ���������� ��� ����������� ��������
		_position.x += dx;
		_position.y += dy;

		// ������������ �������� �� ����������
		_speed.y += _g * dt;

		// �������� ������
		_eff->posX = _position.x;// +0.f;
		_eff->posY = _position.y;// +0.f;
						
		// ���� ����� ����� ������ ��� ����� �� ������ � ������ ��� �� ����
		if ((_totalTime > _timeExplosion) && (!_boom) && (_curLevel < _level)) {
			for (unsigned i = 0; i < _childFires.size(); i++) {
				// ������������ ������� ���� �������� �������
				_childFires[i]->setPosition(_position);
				// � ��������� �� � ������ �������
				_childFires[i]->Start(effCont);
			}

			// ������ ����
			_boom = true;
			
			ParticleEffectPtr eff = effCont.AddEffect("Boom");
			eff->posX = _position.x + 0.f;
			eff->posY = _position.y + 0.f;
			eff->Reset();
		}
		else {
			_totalTime += dt;
		}

		// ����� ������ ��������� ��� �������� �������� � �������������� ��������
		// �������� ������� ������������� ������
		if (_boom) {
			for (unsigned i = 0; i < _childFires.size(); i++) {
				_childFires[i]->Update(dt, FPoint(dx, dy), effCont);
			}
				
			if (_curLevel < _level) {
				if (_childFires.size() > 0) {
					_eff->SetPos(_childFires[0]->getPosition());
					_position = _childFires[0]->getPosition();
				}
			}
		}	
	}
}
