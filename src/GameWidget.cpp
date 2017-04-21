#include "stdafx.h"
#include "GameWidget.h"
#include "ofxTweener.h"

GameWidget::GameWidget(const std::string & name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, _timer(0)
	, _targetsRuf(nullptr)
	, _gun(nullptr)
	, _count(0)
	, _timeLimit(0)
	, _speed(0)
	, _scene(START_SCENE)
	, _bg(nullptr)
	, _bg_sm(nullptr)
	, _table(nullptr)
	, _timer_table(nullptr)
	, _aim(nullptr)
	, _level(0)
	, _countChilds(0)
	, _shading(255)
	, _start1("Start", true)
	, _start2("Start", true)
	, _timerStr("00", false)
	, _timeOutStr("Time out", false)
	, _hitStr("hit 0//0", false)
	, _restartStr("Restart", true)
	, _winnerStr("WINNER!!!", false)
	, _alphaForBg(255)
	, _yForTable(0)
{
	Init();
}

GameWidget::~GameWidget()
{
	// ������� �� ������ �������
	delete _gun;
}

// ������������� ���x �������
void GameWidget::Init()
{
	// ��������� ����� ���������� �� �����
	ReadFromFile();

	// �������� ������ (���������) �����
	_scene = START_SCENE;
	// ������� ������ (�������)
	_gun = new Gun();
	// ��������� �������� ����
	_bg = Core::resourceManager.Get<Render::Texture>("tir");
	_bg_sm = Core::resourceManager.Get<Render::Texture>("tir_sm");
	_table = Core::resourceManager.Get<Render::Texture>("bg_table");
	_timer_table = Core::resourceManager.Get<Render::Texture>("tir_timer");
	_alphaForBg = 255;
	_yForTable = 0;
	// � �������
	_aim = Core::resourceManager.Get<Render::Texture>("aim");

	//_effCont = new EffectsContainer();

	_shading = 255;

	// �������������� �����. �����, ������� ���������, �������
	_start1.SetColors(Color(30, 30, 30, 255), Color(30, 30, 30, 255));
	_start2.SetColors(Color(255, 128, 0, 255), Color(255, 201, 14, 255));
	_start1.SetPositions(FPoint(WINDOW_WIDTH / 2 - 5, WINDOW_HEIGHT), 
		FPoint(WINDOW_WIDTH / 2 - 5, WINDOW_HEIGHT / 3 * 2 + 5));
	_start2.SetPositions(FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT), 
		FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 * 2));
	_start1.SetScale(5.0f, 2.0f);
	_start2.SetScale(5.0f, 2.0f);
	// �������� ����� ����� �����
	_start1.Show(5.0f);
	_start2.Show(5.0f);

	// �������������� ������
	_timerStr.SetColors(Color(255, 0, 0, 255), Color(255, 0, 0, 255));
	_timerStr.SetPositions(FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 15),
		FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 15));
	_timerStr.SetScale(0.1f, 0.9f);

	// ������ ��� ������������ �������
	_timeOutStr.SetColors(Color(6, 76, 6, 255), Color(6, 76, 6, 255));
	_timeOutStr.SetPositions(FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 100),
		FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 100));
	_timeOutStr.SetScale(3.0f, 0.5f);

	_hitStr.SetColors(Color(6, 76, 6, 255), Color(6, 76, 6, 255));
	_hitStr.SetPositions(FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50),
		FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50));
	_hitStr.SetScale(3.0f, 0.5f);

	_restartStr.SetColors(Color(255, 128, 0, 255), Color(255, 201, 14, 255));
	_restartStr.SetPositions(FPoint(WINDOW_WIDTH / 2, 0),
		FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	_restartStr.SetScale(3.0f, 0.5f);

	// ������ WINNER!!!
	_winnerStr.SetColors(Color(255, 201, 14, 255), Color(255, 201, 14, 255));
	_winnerStr.SetPositions(FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT),
		FPoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100));
	_winnerStr.SetScale(3.0f, 1.0f);
}

void GameWidget::ReadFromFile()
{
	// ��������� ���� ��� ������
	IO::FileStream fs("input.txt");
	IO::TextReader reader(&fs);

	// ��������� ������ LEVEL - ������ ����������� ��� ������
	_level = ReadValue(&reader, "Level");
	// ��������� ������ COUNT - ���������� ������� ������� ������
	_countChilds = ReadValue(&reader, "Count");
	// ��������� ������ CountTarget - ���������� �������
	_count = ReadValue(&reader, "CountTarget");
	// ��������� ������ SPEED - �������� �������� ������� (������)
	_speed = ReadValue(&reader, "Speed");
	// ��������� ������ TIME - ����� �������
	_timeLimit = ReadValue(&reader, "Time");
}

int GameWidget::ReadValue(IO::TextReader *reader, char *arg)
{
	std::string str, name, value;
	int result;

	str = reader->ReadAsciiLine();
	if (utils::ReadNvp(str, name, value) && name == arg)
	{
		result = utils::lexical_cast<int>(value);
	}
	else {
		result = 1;
	}

	return result;
}

void GameWidget::Draw()
{
	// ������� ������ ���
	Render::device.PushMatrix();
	Render::BeginColor(Color(_shading, _shading, _shading, _shading));
	_bg->Draw();
	Render::EndColor();
	// ��������� ���
	Render::BeginColor(Color(255, 255, 255, _alphaForBg));
	_bg_sm->Draw();
	Render::EndColor();

	Render::device.PopMatrix();

	_effCont.Draw();

	std::string strTemp = "Hit 0//0";

	// � ����������� �� ������ ���� ������� �� �����
	switch (_scene) {
		// ����� = 0. ����� ������ ����. ��������� �����
	case START_SCENE:
		break;
		// ����� = 1. ��� ������� ����
	case ACTIONS:
		// ������ ������,
		_targetsRuf->Draw();
		// ������� � ������
		_gun->Draw();
		break;
		// ����� 2 - ��������� �����
	case TIME_OUT:
		// ���������� �������� ���������� ������� �������
		_targetsRuf->Draw();
		strTemp = "Hit " + utils::lexical_cast(_count - _targetsRuf->GetCountTargetNow()) + "/" + utils::lexical_cast(_count);
		break;
		// ����� 3 - ��� ������ �����
	case WINNER:
		// ������
		_targetsRuf->Draw();
		// ������ ������
		_gun->Draw();
		break;
	default:
		break;
	}

	// ������� ��������� �������
	_start1.Draw();
	_start2.Draw();
	// ������� �� ����� ������, ������� �������� �������
	// ��������� ���
	Render::BeginColor(Color(255, 255, 255, 255 - _alphaForBg));
	_timer_table->Draw();
	Render::EndColor();
	_timerStr.SetString(utils::lexical_cast((int)(_timeLimit - _timer)).c_str());
	_timerStr.Draw();
	// ������� ��������� � ������������� �������
	_timeOutStr.Draw();
	const char *str = strTemp.c_str();
	_hitStr.SetString(str);
	_hitStr.Draw();
	_restartStr.Draw();
	// ������� ��������� � ������
	_winnerStr.Draw();

	// ������� �� ����� ������
	Render::device.PushMatrix();
	// ���������� ��������� �����
	IPoint mouse_pos = Core::mainInput.GetMousePos();

	Render::device.MatrixTranslate(math::Vector3(mouse_pos.x, mouse_pos.y, 0));
	IRect texRect = _aim->getBitmapRect();
	FRect rect(texRect), uv(0, 1, 0, 1);
	_aim->TranslateUV(rect, uv);
	Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height / 2.f, 0.f));

	_aim->Bind();
	Render::DrawQuad(rect, uv);
	Render::device.PopMatrix();

	for (unsigned i = 0; i < _fireworksQueue.size(); i++) {
		_fireworksQueue[i]->Draw();
	}

	// ����� �������������� ����������
	Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 70, utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.5f, CenterAlign);
	Render::PrintString(10, 70, "Arrows: " + utils::lexical_cast(_gun->getCountArrow()), 1.5f, LeftAlign);
	int countF = 0;
	for (unsigned i = 0; i < _fireworksQueue.size(); i++) {
		countF += _fireworksQueue[i]->getCountFire();
	}
	Render::PrintString(10, 100, "Fireworks: " + utils::lexical_cast(countF), 1.5f, LeftAlign);
	if (_targetsRuf) Render::PrintString(10, 130, "Targets: " + utils::lexical_cast(_targetsRuf->GetCountTargetNow()), 1.5f, LeftAlign);
	else Render::PrintString(10, 130, "Targets: 0", 1.5f, LeftAlign);

	Render::device.PushMatrix();
	// �������� �� ������� �������
	Render::device.MatrixTranslate(math::Vector3(0, _yForTable, 0));
	_table->Draw();
	Render::device.PopMatrix();
}

void GameWidget::Update(float dt)
{
	// ��������� ���� "�������� ���������"
	Tweener.update();
	//
	// ������� ��������� � ���������
	//
	_effCont.Update(dt);

	switch (_scene) {
	case START_SCENE:
		break;
	case ACTIONS:
		// ��������� ������
		_targetsRuf->Update(dt);
		// ��������� ������
		_gun->Update(dt);
		// ��������� ��������� ����� �� �������
		_gun->CheckTargetsHit(_targetsRuf, _effCont);

		// ���� ����� �����, �� ������������� � ����� 2
		if ((_timeLimit - _timer) <= 0) {
			//_interface_mode = 2;
			_scene = TIME_OUT;
			// ������� �� ������ ��� ������
			_gun->ClearArrows();
			// �������� ������
			_timerStr.Hide(0.0f);
			// ���������� �������
			_timeOutStr.Show(2.0f);
			_hitStr.Show(2.0f);
			_restartStr.Show(3.0f);
		}

		// ���� ������� �� �������� (��� �������), �� ������������� � ����� 3
		if ((_targetsRuf->GetCountTargetNow() == 0)) {
			//_interface_mode = 3;
			_scene = WINNER;

			_timerStr.Hide(0.0f);

			_winnerStr.Show(3.0f);
			_restartStr.Show(5.0f);

			_shading = 127;
			// ������ ���� ������� ����� ���� ��� ����� ��� ������
			Fireworks *newFireworks = new Fireworks(
				FPoint(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4),
				FPoint(0, 30),
				_level,
				0,
				_countChilds,
				1.0f,
				_effCont);
			newFireworks->Start(_effCont);
			_fireworksQueue.push_back(newFireworks);

			Fireworks *newFireworks2 = new Fireworks(
				FPoint(WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT / 4),
				FPoint(0, 30),
				_level,
				0,
				_countChilds,
				1.0f,
				_effCont);
			newFireworks2->Start(_effCont);
			_fireworksQueue.push_back(newFireworks2);
		}

		break;
	case TIME_OUT:
		// ���������� ��������� ������
		_targetsRuf->Update(dt);
		break;
	case WINNER:
		// ���������� ��������� � ������, � ������
		_targetsRuf->Update(dt);
		_gun->Update(dt);
		break;
	default:
		break;
	}

	unsigned i = 0;
	while (i < _fireworksQueue.size()) {
		_fireworksQueue[i]->Update(dt, FPoint(0, 0), _effCont);
		if (_fireworksQueue[i]->allChildrenHidden()) {
			_fireworksQueue[i]->Finish();
			delete _fireworksQueue[i]; // ->~Fireworks();
			_fireworksQueue.erase(_fireworksQueue.begin() + i);
		}
		else {
			i++;
		}
	}

	// ����������� ������ �� dt
	_timer += dt;
}

void GameWidget::AcceptMessage(const Message & message)
{
}

bool GameWidget::MouseDown(const IPoint & mouse_pos)
{
	if (Core::mainInput.GetMouseRightButton())
	{
		//
		// ��� ������� �� ������ ������ ����,
	}
	else
	{
		//
		// ��� ������� �� ����� ������ ����,

		Fireworks *newFireworks;

		switch (_scene) {
			// �� ��������� ������
		case START_SCENE:
			// ��������� ��������� ����� � ������� ������� START
			if (_start2.MouseDown(mouse_pos)) {
				Tweener.addTween(_alphaForBg, 0.0f, 2.5f);
				Tweener.addTween(_yForTable, -WINDOW_HEIGHT / 2, 2.5f);

				_start1.Hide(2.0f);
				_start2.Hide(2.0f);

				// ������� ����� ��������� �������
				_targetsRuf = new TargetsRifle(_count, _timeLimit);
				// ����������� ����� � �������
				_scene = ACTIONS;
				_timer = 0;

				_timerStr.Show(1.0f);
			}

			break;

		case ACTIONS:
			// ���������� ������ ���������� �����
			_gun->MouseDown(mouse_pos, _speed, _effCont);
			break;

		case TIME_OUT:
			// ��������� ��������� � ������� ������� �������
			if (_restartStr.PointIntoRect(mouse_pos)) {
				// ������ �������
				_timeOutStr.Hide(1.0f);
				_hitStr.Hide(1.0f);
				_restartStr.Hide(1.0f);

				_timerStr.Show(1.0f);

				// ���� ������, �� ������� ������ � ������� � ������
				_targetsRuf->Clear();
				_targetsRuf->Init();
				//_gun->ClearArrows();

				// ������������� � ������� �����
				_scene = ACTIONS;
				_timer = 0;
			}
			break;

			// �����, ���� ����� ��� ������
		case WINNER:
			// ���������� ���� � ������� ������� ������
			if (_restartStr.PointIntoRect(mouse_pos)) {
				// ������ �����
				_winnerStr.Hide(2.0f);
				_restartStr.Hide(2.0f);
				_timerStr.Show(1.0f);

				// ������� ������ � ������ �� �������
				_targetsRuf->Clear();
				_targetsRuf->Init();
				//_gun->ClearArrows();
				// ������������� � ������� �����
				_scene = ACTIONS;
				_shading = 255;
				_timer = 0;
			}
			else {
				newFireworks = new Fireworks(
					FPoint(mouse_pos.x, mouse_pos.y),
					FPoint(0, 30),
					_level,
					0,
					_countChilds,
					1.0f,
					_effCont);
				newFireworks->Start(_effCont);
				_fireworksQueue.push_back(newFireworks);
			}
			break;
		default:
			break;
		}

	}
	return false;
}

void GameWidget::MouseMove(const IPoint & mouse_pos)
{
	switch (_scene) {
		// �� ��������� ������
	case START_SCENE:
		// ��������� ��������� ����� � ������� ������� START
		_start2.MouseMove(mouse_pos);
		break;

	case ACTIONS:
		break;

	case TIME_OUT:
		// ��������� ��������� � ������� ������� �������
		_restartStr.MouseMove(mouse_pos);
		break;

		// �����, ���� ����� ��� ������
	case WINNER:
		// ���������� ���� � ������� ������� ������
		_restartStr.MouseMove(mouse_pos);
		break;

	default:
		break;
	}
}

void GameWidget::MouseUp(const IPoint & mouse_pos)
{
}

void GameWidget::KeyPressed(int keyCode)
{
}

void GameWidget::CharPressed(int unicodeChar)
{
}
