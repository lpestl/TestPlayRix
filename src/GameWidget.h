#pragma once

#include "Fireworks.h"
#include "TargetsRifle.h"
#include "Gun.h"
// ���� ���������������� ����� ������ �� �����
#include "PushTheString.h"

// ENUM interface_mode
enum interface_mode {
	START_SCENE,	// ��������� �����
	ACTIONS,		// ����� ���� (�������� � ������)
	TIME_OUT,		// ����� ����� �������
	WINNER			// ����� ������
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
	// �������, ������� ����� ������ ���� input.txt
	void ReadFromFile();
	int ReadValue(IO::TextReader *reader, char *arg);

private:
	// ���������� ������ ��������� � �������� �������
	TargetsRifle		*_targetsRuf;
	// ���������� ������, ������������ ������
	Gun					*_gun;

	// ���������� �������
	int					_count;
	// ����������� �� �������
	float				_timeLimit;
	// �������� �������� ������� (������) �������� / ���.
	float				_speed;
	// ������
	float				_timer;
	// ���������� ��� �������� ������ ����������
	interface_mode		_scene;

	// ������� ����������� ����������
	int					_level;
	// ���������� ��������� ��������, �� ������� ����������� ������� ������
	int					_countChilds;
	// ��������� ��� ���� �� ����� ������
	int					_shading;
	// ������ � ����������� ������������
	std::vector<Fireworks*> _fireworksQueue;

	// ��������� � ���������
	EffectsContainer	_effCont;

	// �������� ����
	Render::Texture		*_bg;
	Render::Texture		*_bg_sm;
	Render::Texture		*_table;
	Render::Texture		*_timer_table;
	float					_alphaForBg;
	float					_yForTable;
	// �������� �������
	Render::Texture		*_aim;
	
	// ����� �� ������ ��������� �����
	PushTheString _start1;
	PushTheString _start2;
	// ����� �������
	PushTheString _timerStr;
	// ����� ��� ����������� �������
	PushTheString _timeOutStr;
	PushTheString _hitStr;
	PushTheString _restartStr;
	// ����� ��� ������
	PushTheString _winnerStr;
};

