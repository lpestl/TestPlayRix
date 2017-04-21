#include "stdafx.h"
#include "PushTheString.h"
#include <string.h>
#include "ofxTweener.h"

PushTheString::PushTheString(char *str, bool isPushed = false)
	: _position(0, 0)
	, _startPosition(0, 0)
	, _endPosition(0, 0)
	, _color(255, 255, 255, 0)
	, _startColor(255, 255, 255, 255)
	, _mouseColor(0, 0, 0, 255)
	, _scale(1.0f)
	, _alpha(0.0f)
{
	_str = new char[strlen(str) + 1];
	strcpy(_str, str);
	_isPushed = isPushed;
}

PushTheString::~PushTheString()
{
	//delete[] _str;
}

/*void PushTheString::Update(float dt) {
	Tweener.update();
}*/

void PushTheString::Draw() {
	Render::device.SetTexturing(false);
	// Проверяем загружен ли шрифт
	Render::BindFont("tahomabd");

	Render::BeginColor(Color(_color.red, _color.green, _color.blue, (uint8_t)_alpha));
	Render::PrintString(_position, _str, _scale, CenterAlign);
	Render::EndColor();

	Render::device.SetTexturing(true);
}

bool PushTheString::MouseDown(const IPoint& mouse_pos) {
	if (PointIntoRect(mouse_pos)) {
		return true;
	}

	return false;
}

void PushTheString::MouseMove(const IPoint& mouse_pos) {
	if (PointIntoRect(mouse_pos)) {
		_color = _mouseColor;
	}
	else {
		_color = _startColor;
	}
}

void PushTheString::Show(float sec) {
	Tweener.addTween(_position.x, _endPosition.x, sec);
	Tweener.addTween(_position.y, _endPosition.y, sec);
	Tweener.addTween(_alpha, 255.0f, sec);
	Tweener.addTween(_scale, _endScale, sec);
}

void PushTheString::Hide(float sec) {
	Tweener.addTween(_position.x, _startPosition.x, sec);
	Tweener.addTween(_position.y, _startPosition.y, sec);
	Tweener.addTween(_alpha, 0.0f, sec);
	Tweener.addTween(_scale, _startScale, sec);
}

bool PushTheString::PointIntoRect(IPoint point) {
	if (((int)_alpha == 255) && (_isPushed)) {
		// Запоминаем высоту шрифта
		int dy = Render::getFontHeight("tahomabd");

		// Проверяем попадание курсора в надпись
		if ((point.y > (_position.y - dy * _scale)) &&
			(point.y < (_position.y)) &&
			(point.x > (_position.x - (dy * _scale) / 2.5f * strlen(_str) / 2)) &&
			(point.x < (_position.x + (dy * _scale) / 2.5f * strlen(_str) / 2))) {
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}

void PushTheString::SetString(const char *str) {
	strcpy(_str, str);
}