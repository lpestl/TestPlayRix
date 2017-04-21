#include "stdafx.h"
#include "Gun.h"
#include "GameWidget.h"

// ----------------------------- Arrow Class ----------------------
// Методы класса Arrow
// -------------------

Arrow::Arrow(float scale, float speed, float angle, EffectsContainer &effCont) {
	// Эффект
	_eff = effCont.AddEffect("ArrowBack");
	// Установка значений свойств из параметров конструктора
	_scale = scale;
	_speed = speed;
	_angle = angle;
	// Инициализация
	Init();
}

void Arrow::Init() {
	_kTime = 10;
	// Загрузка текстуры стрелы
	_arrow = Core::resourceManager.Get<Render::Texture>("arrow");
	// Установка начальной позиции
	_position = IPoint(GameWidget::WINDOW_WIDTH / 2, 10);
	_isHit = false;

	// Добавление эффекта горения наконечника и шлейфа из дыма
	_eff->posX = _position.x;
	_eff->posY = _position.y;
	_eff->Reset();
}

Arrow::~Arrow() {

}

FPoint Arrow::getPosition() {
	return _position;
}

// Устанновка флага "попадание"
void Arrow::setHit(bool hit) {
	_isHit = hit;
	// Завершаем эффект с сохранением шлейфа
	//if (_eff) {
	_eff->Finish();
	_eff = NULL;
	//}
}

void Arrow::Draw() {
	// Обновляем контейнер эффектов
	//_effCont.Draw();

	// Если еще попадания не было
	if (!_isHit) {
		Render::device.PushMatrix();
		// Сдвигаем на текущую позицию
		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
		// Поворачиваем на нужный угол
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

		// Получаем ширину текстуры
		IRect texRect = _arrow->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_arrow->TranslateUV(rect, uv);

		// Устанавливаем масштаб текстуры
		Render::device.MatrixScale(_scale);
		// Сдвигаем на "наконечник" стрелы
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height, 0.f));

		// Натягиваем текстуру
		_arrow->Bind();
		Render::DrawQuad(rect, uv);

		Render::device.PopMatrix();
	}
}

void Arrow::Update(float dt) {	
	// Подсчитываем положение стрелы
	_position.x += _speed * math::cos((_angle + 90) * math::PI / 180) * dt * _kTime;
	_position.y += _speed * math::sin((_angle + 90) * math::PI / 180) * dt * _kTime;

	//_effCont.Update(dt);
	// Если попадания не было
	if (!_isHit) {
		// Сдвигаем эффект
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
// Класс оружия (арбалета)

Gun::Gun() {
	Init();
}

void Gun::Init() {
	// Загружаем текстуру арбалета
	_arb = Core::resourceManager.Get<Render::Texture>("arbalet");
	// Масштаб подоран оптимальный для размера текстуры арбалета и стрел
	_scale = 0.3;
	// Начальный угол 0 градусов
	_angle = 0;
	// Позиция внизу по середине экрана
	_posArb = IPoint(GameWidget::WINDOW_WIDTH / 2, 10);

}

void Gun::Draw() {
	// Рисуем стрелы, если они есть
	for (unsigned i = 0; i < _arrows.size(); i++) {
		_arrows[i]->Draw();		
	}

	// Рисуем арбалет с наклоном относительно положения мышки
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
	// То удаляем стрелу из памяти
	_arrows[n]->KillEffect();
	delete _arrows[n];
	// и из вектора
	_arrows.erase(_arrows.begin() + n);
}

void Gun::Update(float dt) {
	unsigned i = 0;
	IRect texRect = _arb->getBitmapRect();
	// Обновляем все стрелы которые есть
	while (i < _arrows.size()) {
		_arrows[i]->Update(dt);
		// Проверяем, если стрела вышла за пределы экрана
		FPoint aPoint = _arrows[i]->getPosition();

		if ((aPoint.y > (GameWidget::WINDOW_HEIGHT + texRect.height)) ||
			(aPoint.y < (-texRect.height)) || 
			(aPoint.x >(GameWidget::WINDOW_WIDTH + texRect.height)) ||
			(aPoint.x < (-texRect.height))) {
			// То удаляем стрелу из памяти
			DeletArrow(i);
		}
		else {
			i++;			
		}
	}
	IPoint mouse_pos = Core::mainInput.GetMousePos();
	// Высчитываем угол наклона арбалета и стрел
	if ((mouse_pos.x - _posArb.x) != 0) {
		_angle = math::atan((mouse_pos.y - _posArb.y) , (mouse_pos.x - _posArb.x)) * 180.0f / math::PI - 90;
	} 
}

// Нажата левая кнопка мышки
bool Gun::MouseDown(const IPoint& mouse_pos, int speed, EffectsContainer &effCont) {
	// Создаем новую стрелу
	_arrows.push_back(new Arrow(_scale, speed, _angle, effCont));

	return false;
}

// Проверка попадания в мишень
void Gun::CheckTargetsHit(TargetsRifle *targetsRuf, EffectsContainer &effCont) {
	for (int i = _arrows.size() - 1; i >= 0; i--) {
		// Если стрела еще не попадала ни в одну мишень
		if (!_arrows[i]->getHit()) {
			// И если она попала в мишень
			if (targetsRuf->ChekingHits(_arrows[i]->getPosition(), effCont)) {
				// То устанавливаем флаг "попадание"
				_arrows[i]->setHit(true);
			}
		}
	}
}

// Удаляем все стрелы
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

