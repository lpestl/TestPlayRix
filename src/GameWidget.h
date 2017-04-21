#pragma once

#include "Fireworks.h"
#include "TargetsRifle.h"
#include "Gun.h"
// Чуть усовершенствовал вывод текста на экран
#include "PushTheString.h"

// ENUM interface_mode
enum interface_mode {
	START_SCENE,	// Стартовая сцена
	ACTIONS,		// Сцена игры (стрелять в мишени)
	TIME_OUT,		// Сцена конца времени
	WINNER			// Сцена победы
};

class GameWidget : public GUI::Widget
{
public:
	GameWidget(const std::string& name, rapidxml::xml_node<>* elem);
	~GameWidget();

	void Draw() override;
	void Update(float dt) override;

	void AcceptMessage(const Message& message) override;

	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;
	void CharPressed(int unicodeChar) override;
	
	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 768;

private:
	void Init();
	// Функция, которая будет читать файл input.txt
	void ReadFromFile();
	int ReadValue(IO::TextReader *reader, char *arg);

private:
	// Переменная класса генирации и хранения мишеней
	TargetsRifle		*_targetsRuf;
	// Переменная класса, описывающего оружие
	Gun					*_gun;

	// Количество мишеней
	int					_count;
	// Ограничение по времени
	float				_timeLimit;
	// Скорость движения снаряда (стрелы) пикселей / сек.
	float				_speed;
	// Таймер
	float				_timer;
	// Переменная для хранения режима интерфейса
	interface_mode		_scene;

	// Уровень вложенности фейерверка
	int					_level;
	// Количество маленьких снарядов, на которое разлетается большой снаряд
	int					_countChilds;
	// Затенение для фона во время победы
	int					_shading;
	// Вектор с запущенными фейерверками
	std::vector<Fireworks*> _fireworksQueue;

	// Контейнер с эффектами
	EffectsContainer	_effCont;

	// Текстура фона
	Render::Texture		*_bg;
	Render::Texture		*_bg_sm;
	Render::Texture		*_table;
	Render::Texture		*_timer_table;
	float					_alphaForBg;
	float					_yForTable;
	// Текстура прицела
	Render::Texture		*_aim;
	
	// Текст на первой стартовой сцене
	PushTheString _start1;
	PushTheString _start2;
	// Текст таймера
	PushTheString _timerStr;
	// Текст при кончившемся времени
	PushTheString _timeOutStr;
	PushTheString _hitStr;
	PushTheString _restartStr;
	// Текст при победе
	PushTheString _winnerStr;
};

