#pragma once

#include "TargetsRifle.h"
#include <vector>

// ----------------------------- Arrow Class ----------------------
// Класс "Стрела". Снаряд который вылетает из оружия
class Arrow
{
public:
	// Конструктор с параметрами - коеффициент величины текстуры, 
	// скорость полета снаряда и угол наклона текстуры
	Arrow(float scale, float speed, float angle, EffectsContainer &effCont);
	// Диструктор
	~Arrow();

	void Draw();
	void Update(float dt);

	// Установить значение флага "попадпние"
	void setHit(bool hit);
	// получить значение флага "попадание"
	bool getHit();
	// получить позицию снаряда
	FPoint getPosition();
	// Убиваем эффект
	void KillEffect();

private:
	void Init();

private:
	// Текстура стрелы
	Render::Texture *_arrow;
	// Позиция
	FPoint _position;
	// Угол поворота текстуры
	float _angle;
	// Масштаб текстуры
	float _scale;
	// Скорость снаряда (пикс / сек)
	float _speed;
	// Флаг "попадание"
	bool _isHit;
	// Коэффициент времени
	int _kTime;

	ParticleEffectPtr _eff;
};

// -------------------------- Gun Class -----------------------------
// Класс "Оружие". Арбалет внизу экрана
class Gun
{
public:
	Gun();

	void Draw();
	void Update(float dt);
	// Нажатие кнопки мыши
	bool MouseDown(const IPoint& mouse_pos, int speed, EffectsContainer &effCont);
	// Получить количество выпущенных стрел
	int getCountArrow();
	// Удалить все стрелы
	void ClearArrows();
	// Проверка попадания в мишени
	void CheckTargetsHit(TargetsRifle *targetsRuf, EffectsContainer &effCont);
	// Удаляем стрелу из вектора
	void DeletArrow(unsigned n);

private:
	void Init();

private:
	// Текстура арбалета
	Render::Texture *_arb;
	// Положение арбалета
	IPoint _posArb;
	// Масштаб текстуры
	float _scale;
	// Угол поворота текстуры
	float _angle;
	// Вектор со стрелами
	std::vector<Arrow *> _arrows;
};

