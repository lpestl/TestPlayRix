#pragma once

// Класс Фейерверк 
class Fireworks
{
public:
	// Конструктор по умолчанию с нулевыми значениями
	Fireworks();
	// Конструктор с параметрами
	Fireworks(FPoint startPosition,  // Точка начала салюта
		FPoint speed,	// Начальный вектор скорости
		int level,		// Максимальная глубина рекурсивного вызова
		int curLevel,	// Текущее поколение рекурсии
		int countChild, // Количество дочерни зарядов
		float timeExplosion, // Время до взрыва
		EffectsContainer &effCont); // Контейнер с эффектами
	
	void Draw();
	void Update(float dt, FPoint speedParent, EffectsContainer &effCont);

	// Старт (запуск) снаряда
	void Start(EffectsContainer &effCont);
	// Установить позицию снаряда
	void setPosition(FPoint pos);
	// Проверить, все ли дочерние заряды скрыты.
	bool allChildrenHidden();
	// Завершение салюта
	void Finish();
	// Получить текущую позицию
	FPoint getPosition();
	// Получить количество дочерних снарядов всего
	int getCountFire();

private:
	// Инициализация переменных
	//void Init();

	// Вектор скорости
	FPoint _speed;
	// Длинна вектора скорости в начальный момент
	float _startSpeed;
	// Текущая позиция снаряда
	FPoint _position;
	// Переменная гравитации
	float _g;
	// Текущее поколение рекурсии
	int _curLevel;
	// Коэффициент времени
	float _kTime;
	// Уровни рекурсии
	int _level;
	// Количество дочерних зарядов
	int _countChild;
	// Флаг взрыва
	bool _boom;
	// Время до взрыва
	float _timeExplosion;
	// Общее время существования
	float _totalTime;
	Render::Texture *_star;

	// Вектор с дочерними зарядами
	std::vector <Fireworks *> _childFires;

	ParticleEffectPtr _eff;
};

