#include "stdafx.h"
#include "Gun.h"
#include "GameWidget.h"

// ----------------------------- Arrow Class ----------------------
// ������ ������ Arrow
// -------------------

Arrow::Arrow(float scale, float speed, float angle, EffectsContainer &effCont) {
	// ������
	_eff = effCont.AddEffect("ArrowBack");
	// ��������� �������� ������� �� ���������� ������������
	_scale = scale;
	_speed = speed;
	_angle = angle;
	// �������������
	Init();
}

void Arrow::Init() {
	_kTime = 10;
	// �������� �������� ������
	_arrow = Core::resourceManager.Get<Render::Texture>("arrow");
	// ��������� ��������� �������
	_position = IPoint(GameWidget::WINDOW_WIDTH / 2, 10);
	_isHit = false;

	// ���������� ������� ������� ����������� � ������ �� ����
	_eff->posX = _position.x;
	_eff->posY = _position.y;
	_eff->Reset();
}

Arrow::~Arrow() {

}

FPoint Arrow::getPosition() {
	return _position;
}

// ���������� ����� "���������"
void Arrow::setHit(bool hit) {
	_isHit = hit;
	// ��������� ������ � ����������� ������
	//if (_eff) {
	_eff->Finish();
	_eff = NULL;
	//}
}

void Arrow::Draw() {
	// ��������� ��������� ��������
	//_effCont.Draw();

	// ���� ��� ��������� �� ����
	if (!_isHit) {
		Render::device.PushMatrix();
		// �������� �� ������� �������
		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
		// ������������ �� ������ ����
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

		// �������� ������ ��������
		IRect texRect = _arrow->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_arrow->TranslateUV(rect, uv);

		// ������������� ������� ��������
		Render::device.MatrixScale(_scale);
		// �������� �� "����������" ������
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height, 0.f));

		// ���������� ��������
		_arrow->Bind();
		Render::DrawQuad(rect, uv);

		Render::device.PopMatrix();
	}
}

void Arrow::Update(float dt) {	
	// ������������ ��������� ������
	_position.x += _speed * math::cos((_angle + 90) * math::PI / 180) * dt * _kTime;
	_position.y += _speed * math::sin((_angle + 90) * math::PI / 180) * dt * _kTime;

	//_effCont.Update(dt);
	// ���� ��������� �� ����
	if (!_isHit) {
		// �������� ������
		_eff->posX = _position.x;
		_eff->posY = _position.y;
	}
}

bool Arrow::getHit() {
	return _isHit;
}

void Arrow::KillEffect() {
	if (_eff) {
		_eff->Kill();
		_eff = NULL;
	}
}

// --------------------------- Gun Class -------------------
// ����� ������ (��������)

Gun::Gun() {
	Init();
}

void Gun::Init() {
	// ��������� �������� ��������
	_arb = Core::resourceManager.Get<Render::Texture>("arbalet");
	// ������� ������� ����������� ��� ������� �������� �������� � �����
	_scale = 0.3;
	// ��������� ���� 0 ��������
	_angle = 0;
	// ������� ����� �� �������� ������
	_posArb = IPoint(GameWidget::WINDOW_WIDTH / 2, 10);

}

void Gun::Draw() {
	// ������ ������, ���� ��� ����
	for (unsigned i = 0; i < _arrows.size(); i++) {
		_arrows[i]->Draw();		
	}

	// ������ ������� � �������� ������������ ��������� �����
	Render::device.PushMatrix();
	
	Render::device.MatrixTranslate(math::Vector3(_posArb.x, _posArb.y, 0));
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	IRect texRect = _arb->getBitmapRect(); 
	FRect rect(texRect), uv(0, 1, 0, 1);
	_arb->TranslateUV(rect, uv);

	Render::device.MatrixScale(_scale);
	Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height / 2.f, 0.f));

	_arb->Bind();

	Render::DrawQuad(rect, uv);

	Render::device.PopMatrix();
}

void Gun::DeletArrow(unsigned n) {
	// �� ������� ������ �� ������
	_arrows[n]->KillEffect();
	delete _arrows[n];
	// � �� �������
	_arrows.erase(_arrows.begin() + n);
}

void Gun::Update(float dt) {
	unsigned i = 0;
	IRect texRect = _arb->getBitmapRect();
	// ��������� ��� ������ ������� ����
	while (i < _arrows.size()) {
		_arrows[i]->Update(dt);
		// ���������, ���� ������ ����� �� ������� ������
		FPoint aPoint = _arrows[i]->getPosition();

		if ((aPoint.y > (GameWidget::WINDOW_HEIGHT + texRect.height)) ||
			(aPoint.y < (-texRect.height)) || 
			(aPoint.x >(GameWidget::WINDOW_WIDTH + texRect.height)) ||
			(aPoint.x < (-texRect.height))) {
			// �� ������� ������ �� ������
			DeletArrow(i);
		}
		else {
			i++;			
		}
	}
	IPoint mouse_pos = Core::mainInput.GetMousePos();
	// ����������� ���� ������� �������� � �����
	if ((mouse_pos.x - _posArb.x) != 0) {
		_angle = math::atan((mouse_pos.y - _posArb.y) , (mouse_pos.x - _posArb.x)) * 180.0f / math::PI - 90;
	} 
}

// ������ ����� ������ �����
bool Gun::MouseDown(const IPoint& mouse_pos, int speed, EffectsContainer &effCont) {
	// ������� ����� ������
	_arrows.push_back(new Arrow(_scale, speed, _angle, effCont));

	return false;
}

// �������� ��������� � ������
void Gun::CheckTargetsHit(TargetsRifle *targetsRuf, EffectsContainer &effCont) {
	for (int i = _arrows.size() - 1; i >= 0; i--) {
		// ���� ������ ��� �� �������� �� � ���� ������
		if (!_arrows[i]->getHit()) {
			// � ���� ��� ������ � ������
			if (targetsRuf->ChekingHits(_arrows[i]->getPosition(), effCont)) {
				// �� ������������� ���� "���������"
				_arrows[i]->setHit(true);
			}
		}
	}
}

// ������� ��� ������
void Gun::ClearArrows() {
	for (int i = _arrows.size() - 1; i >= 0; i--) {
		_arrows[i]->KillEffect();
		delete _arrows[i];
	}
	_arrows.clear();
}

int Gun::getCountArrow() {
	return _arrows.size();
}

