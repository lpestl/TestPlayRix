#pragma once

#include <vector>

//------------------------ Target Class --------------------------
// Класс мишени
class Target {
public:
	// Конструктор с параметром начальной позиции
	Target(FPoint position);
	~Target();

	void Draw();
	void Update(float dt);

	// Получить позицию мишени
	FPoint getPosition();
	// Проверить попадание в мишень
	bool chekHit(FPoint posArrow, EffectsContainer &effCont);
	// Получить значение попадания
	bool getHit();

private:
	// Текстура мишени
	Render::Texture *_trg;
	// Позиция мишени
	FPoint _position;
	// Скорость 
	float _speed;
	// Масштаб текстуры
	float _scale;
	// Вектор скорости
	FPoint _speedVector;
	// Флаг "попадание"
	bool _hitting;

};

//-------------------------- TargetsRifle Class ----------------
// Класс управления мишенями

class TargetsRifle
{
public:
	// Конструктор с параметрами количества мишеней и ограничение по времени
	TargetsRifle(int count, float timeLimit);
	// Инициализация
	void Init();

	void Draw();
	void Update(float dt);
	// Проверка попадания точки в мишени
	bool ChekingHits(FPoint posArrow, EffectsContainer &effCont);

	// Очистка вектора мишеней
	void Clear();
	// Полкчить количество мишеней в текущий момент
	int GetCountTargetNow();

private:
	// Вектор с мишенями
	std::vector <Target*> _targets;
	// Количество
	int _count;
	// Ограничение по времени
	float _timeLimit;
};

