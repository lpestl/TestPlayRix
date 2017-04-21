#include "stdafx.h"
#include "TargetsRifle.h"
#include "GameWidget.h"

// ------------- Target --------------------
Target::Target(FPoint position) {
	// ��������� �������
	_position = position;
	// �������� ������ ��������� �������
	_speed = math::random(10, 600);
	// ��������� ������� ������ ����������� ��������� ��������
	_speedVector.x = math::random(-1.0f, 1.0f);
	_speedVector.y = sqrt(1 - _speedVector.x * _speedVector.x);
	
	_speedVector.x *= _speed;
	_speedVector.y *= _speed;
	// ������ ��������� ������� ��������
	_scale = math::random(0.5f, 1.0f);

	_hitting = false;

	// ��������� ������� �������� �������� �� 4�� ������������
	int bat;
	bat = math::random(0, 4);
	switch (bat) {
	case 0:
		_trg = Core::resourceManager.Get<Render::Texture>("target1");
		break;
	case 1:
		_trg = Core::resourceManager.Get<Render::Texture>("target2");
		break;
	case 2:
		_trg = Core::resourceManager.Get<Render::Texture>("target3");
		break;
	case 3:
		_trg = Core::resourceManager.Get<Render::Texture>("target4");
		break;
	case 4:
		_trg = Core::resourceManager.Get<Render::Texture>("target5");
		break;
	default:
		_trg = Core::resourceManager.Get<Render::Texture>("target1");
		break;
	}
}

void Target::Draw() {
	// ���� ��������� �� ����
	if (!_hitting) {
		// ������ ������
		Render::device.PushMatrix();

		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
		//Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

		IRect texRect = _trg->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_trg->TranslateUV(rect, uv);

		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height / 2.f, 0.f));

		_trg->Bind();
		Render::DrawQuad(rect, uv);
		Render::device.PopMatrix();
	}
}

void Target::Update(float dt) {
	IRect texRect = _trg->getBitmapRect();
	// ����������� ��������� ������� ������������ ����� ������ � ������ ���, 
	// ����� �������� ������ ������������
	if ((_position.x + _speedVector.x * dt) > (GameWidget::WINDOW_WIDTH - texRect.width * _scale) ||
		((_position.x + _speedVector.x * dt) < (texRect.width * _scale))) {
		_speedVector.x = -_speedVector.x;
	}
	if ((_position.y + _speedVector.y * dt) > (GameWidget::WINDOW_HEIGHT - texRect.height * _scale) ||
		((_position.y + _speedVector.y * dt) < (texRect.height * _scale))) {
		_speedVector.y = -_speedVector.y;
	}

	_position.x += _speedVector.x * dt;
	_position.y += _speedVector.y * dt;

	if (_hitting) {
		_scale = 0;
	}
}

FPoint Target::getPosition() {
	return _position;
}

// �������� ��������� � ������
bool Target::chekHit(FPoint posArrow, EffectsContainer &effCont) {
	IRect texRect = _trg->getBitmapRect();
	// ���� ����� ������� � RECT ��������,
	if (((posArrow.x) >= (_position.x - texRect.width / 2 * _scale)) &&
		((posArrow.y) >= (_position.y - texRect.height / 2 * _scale)) &&
		((posArrow.x) <= (_position.x - texRect.width / 2 * _scale + texRect.width / 2 * _scale * 2)) &&
		((posArrow.y) <= (_position.y - texRect.height / 2 * _scale + texRect.height / 2 * _scale * 2))) {
		// �� ����� �������, ��� ������
		_hitting = true;
		// � ��������� ������ "�������� ������"
		ParticleEffectPtr eff = effCont.AddEffect("Puh");
		eff->posX = _position.x + 0.f;
		eff->posY = _position.y + 0.f;
		eff->Reset();
	}

	return _hitting;
}

bool Target::getHit() {
	return _hitting;
}

Target::~Target() {
	//_effCont.Finish();
	//_effCont.KillAllEffects();
	//_effCont = NULL;
}


// ---------------------------  TargetsRifle Class --------------------
// ����������� � �����������

TargetsRifle::TargetsRifle(int count, float timeLimit)
{
	_count = count;
	_timeLimit = timeLimit;

	Init();
}

void TargetsRifle::Init() {
	Core::Application *app = Core::appInstance;
	for (int i = 0; i < _count; i++) {
		// ������� ����������� ���������� �������
		_targets.push_back(new Target(FPoint(math::random(128, GameWidget::WINDOW_WIDTH - 128), 
			math::random(128, GameWidget::WINDOW_HEIGHT - 128))));
	}
}

void TargetsRifle::Draw() {
	// ������ ������
	for (unsigned i = 0; i < _targets.size(); i++) {
		_targets[i]->Draw();
	}
}

// �������� ���������� ������� � ������ ������
int TargetsRifle::GetCountTargetNow() {
	int countNow = 0;

	if (_targets.size() == 0) {
		return 0;
	}

	for (int i = _targets.size() - 1; i >= 0; i--) {
		// ���� � ������ ��� �� ������	
		if (!_targets[i]->getHit()) {
			// �� ������� ��
			countNow++;
		}
	}
	return countNow;
}

void TargetsRifle::Update(float dt) {
	for (unsigned i = 0; i < _targets.size(); i++) {
		_targets[i]->Update(dt);
	}
}

void TargetsRifle::Clear() {
	for (int i = _targets.size() - 1; i >=0; i--) {
		delete _targets[i];
	}
	_targets.clear();
}

bool TargetsRifle::ChekingHits(FPoint posArrow, EffectsContainer &effCont) {
	// ��������� ����������
	bool res = false;
	for (int i = _targets.size() - 1; i >= 0; i--) {
		// ���� � ������ ��� �� ������
		if (!_targets[i]->getHit()) {
			// ��������� ��������� ���� �����
			if (_targets[i]->chekHit(posArrow, effCont)) {
				res = true;
				// ������� ������ �� �������
				delete _targets[i];
				_targets.erase(_targets.begin() + i);
				// ������� �� �����
				return res;
				//break;
			}
		}
	}
	
	return res;
}

