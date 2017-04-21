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

// Конструтор с параметрами
Fireworks::Fireworks(FPoint startPosition, FPoint speed, int level, int curLevel, int countChild, float timeExplosion, EffectsContainer &effCont)
	: _g(-9.8f)
	, _kTime(5)
	, _totalTime(0)
	, _boom(false)
	, _star(NULL)
{
	// Задаем начальные значения от параметров конструктора
	_level = level;
	_curLevel = curLevel;
	_countChild = countChild;
	_speed = speed;
	_position = startPosition;
	_timeExplosion = timeExplosion;
	_startSpeed = sqrtf(_speed.y * _speed.y + _speed.x * _speed.x);
	
	_star = Core::resourceManager.Get<Render::Texture>("star");

	_eff = NULL;

	// Проверяем поколение фрактала. Инициализируем сразу все дочернии снаряды
	// ВАЖНО: сделано это с целью оптимизации. Для того чтобы снизить нагрузку на загрузку снарядов во время взрыва
	if (_curLevel < _level) {
		for (int i = 0; i < _countChild; i++) {
			// Считаем вектор скорости для дочерних зарядов в зависимости от их количества
			// для того чтобы они равномерно разлетались по кругу
			float dxSp = _startSpeed * 2 / 3 * cosf(2 * math::PI / _countChild * i + math::PI / 2);// +_speed.x;
			float dySp = _startSpeed * 2 / 3 * sinf(2 * math::PI / _countChild * i + math::PI / 2);// +_speed.y;

			// Создаем дочерние элементы
			Fireworks *newChild = new Fireworks(_position, FPoint(dxSp, dySp), _level, _curLevel + 1, _countChild, _timeExplosion /* 2 / 3*/, effCont);
			_childFires.push_back(newChild);
		}
	}
}

// Запускаем снаряд
void Fireworks::Start(EffectsContainer &effCont) {
	_eff = effCont.AddEffect("FireWorks");
	setPosition(_position);
}

// Установить позицию
void Fireworks::setPosition(FPoint pos) {
	_position = pos;
	if (_eff) {
		_eff->SetPos(pos);
		_eff->Reset();
	}
}

// Завершить эффект
void Fireworks::Finish() {
	_eff->Finish();
	_eff = NULL;
}

// Возрвращает количество дочерних снарядов + 1 текущий
int Fireworks::getCountFire() {
	int result = 1;
	for (unsigned i = 0; i < _childFires.size(); i++) {
		result += _childFires[i]->getCountFire();
	}
	return result;
}

// Получить позицию
FPoint Fireworks::getPosition() {
	return _position;
}

// Проверка видимости всех дочерних снарядов 
bool Fireworks::allChildrenHidden() {
	if (_curLevel < _level) {
		bool res = true;
		// Цикл по всем дочерним зарядам
		for (unsigned i = 0; i < _childFires.size(); i++) {
			// Если хоть один заряд виден, то и текущий остается видимым
			if (!_childFires[i]->allChildrenHidden()) {
				res = false;
			}
			// Если у заряда все дочерние заряды не видимы и он сам наодится вне зоны видимости
			if ((_childFires[i]->allChildrenHidden()) && (_childFires[i]->getPosition().y < 0)) {
				// То завершаем эффект текущего снаряда
				_childFires[i]->Finish();
				// И удаляем его из памити
				delete _childFires[i];
				// и из вектора
				_childFires.erase(_childFires.begin() + i);
			}
		}
		return res;
	}
	else {
		// Если заряд вне зоны видимости то возвращаем TRUE
		if (_position.y < 0) {
			return true;
		}
		else {
			return false;
		}
	}
}

void Fireworks::Draw() {
	// Если взрыв уже был
	if (_boom) {
		for (unsigned i = 0; i < _childFires.size(); i++) {
			// то отрисовываем дочернии заряды
			_childFires[i]->Draw();
		}			
	}
	// Рисуем текущий снаряд
	if (_eff) {
		Render::device.PushMatrix();
		// Сдвигаем на текущую позицию
		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
		// Поворачиваем на нужный угол
		//Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

		// Получаем ширину текстуры
		IRect texRect = _star->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_star->TranslateUV(rect, uv);

		// Устанавливаем масштаб текстуры
		Render::device.MatrixScale(0.5f);
		// Сдвигаем d центр текстуры
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height / 2, 0.f));

		// Натягиваем текстуру
		_star->Bind();
		Render::DrawQuad(rect, uv);

		Render::device.PopMatrix();
	}
}

void Fireworks::Update(float dt, FPoint speedParent, EffectsContainer &effCont) {
	// И если эффект не пустой
	if (_eff)
	{
		// Считаем его изменения положения с учетом силы гравитации
		float dx = _speed.x * dt * _kTime + speedParent.x;
		float dy = _speed.y * dt * _kTime + _g * dt * dt * _kTime * _kTime / 2 + speedParent.y;
		
		// Переместим его посчитанное значение
		_position.x += dx;
		_position.y += dy;

		// Корректируем скорость от гравитации
		_speed.y += _g * dt;

		// Сдвигаем эффект
		_eff->posX = _position.x;// +0.f;
		_eff->posY = _position.y;// +0.f;
						
		// Если общее время больше чем время до взрыва и взрыва еще не было
		if ((_totalTime > _timeExplosion) && (!_boom) && (_curLevel < _level)) {
			for (unsigned i = 0; i < _childFires.size(); i++) {
				// Корректируем позиции всех дочерних зарядов
				_childFires[i]->setPosition(_position);
				// И запускаем их в разные стороны
				_childFires[i]->Start(effCont);
			}

			// Меняем флаг
			_boom = true;
			
			ParticleEffectPtr eff = effCont.AddEffect("Boom");
			eff->posX = _position.x + 0.f;
			eff->posY = _position.y + 0.f;
			eff->Reset();
		}
		else {
			_totalTime += dt;
		}

		// После взрыва обновляем все дочернии элементы с корректировкой скорости
		// учитывая инерцию родительского заряда
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
