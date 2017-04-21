#include "stdafx.h"
#include "TestWidget.h"

TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, _curTex(0)
	, _timer(0)
	, _angle(0)
	, _eff(NULL)
	, _scale(0.f)
{
	Init();
}

void TestWidget::Init()
{
	_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");
	_tex2 = Core::resourceManager.Get<Render::Texture>("Circle");
	_tex3 = Core::resourceManager.Get<Render::Texture>("Star");

	_curTex = 0;
	_angle = 0;

	spline.addKey(0.0f, FPoint(100.0f, 100.0f));
	spline.addKey(0.25f, FPoint(150.0f, 300.0f));
	spline.addKey(0.5f, FPoint(500.0f, 300.0f));
	spline.addKey(0.75f, FPoint(630.0f, 450.0f));
	spline.addKey(1.0f, FPoint(600.0f, 550.0f));
	spline.CalculateGradient();
}

void TestWidget::Draw()
{
	//
	// �������� ������� ��������� ������� ����.
	//
	IPoint mouse_pos = Core::mainInput.GetMousePos();

	//
	// ������������ � ���� ������� �������������� ���������, ����� � ����������
	// ����� ���� ������������ ��� �������������� ������� PopMatrix.
	//
	Render::device.PushMatrix();
	
	//
	// �������� ������� �������������� ���������, ��������� ����� ��������� � ������� ����
	// � ����������� ���������� ������������ ����� ������ ������ ��� z �� ���� _angle.
	//
	Render::device.MatrixTranslate((float)mouse_pos.x, (float)mouse_pos.y, 0);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	if (!_curTex)
	{
		//
		// ����� Texture::Draw() ������� ���������� ������ � ��������� ��������
		// � ������ ��������� (0, 0). ����� ��������� � �������������� ��������� ������
		// ������� ��������������� � ����������� �������� ��������������.
		//
		// ��� ������ ������ Texture::Draw() �������� Texture::Bind() �������������.
		//
		_tex1->Draw();
	}
	else
	{
		IRect texRect = _tex2->getBitmapRect();

		//
		// ��� ��������� �������� ����� ������� �������� UV ���������� ��� ����� ��������,
		// ������� ����� �������� �� ������� �������. UV ���������� ������ ���� �������������,
		// �.�., ��� ����������� �� ������� �������� � ��������, ������ ����� ��������
		// ����� 1 (UV ���������� �������� � ��������� 0..1, ���� ����� �� ������ ������ ��
		// ������ �������, ��� ���� � ������ ������������� ��������� �������� REPEAT, ��������
		// ����� ���������� �� ���� ������� ��������������� ���������� ���).
		//

		FRect rect(texRect);
		FRect uv(0, 1, 0, 1);

		_tex2->TranslateUV(rect, uv);

		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(-texRect.width * 0.5f, -texRect.height * 0.5f, 0.0f);

		//
		// ����������� ��������.
		//
		_tex2->Bind();
		
		//
		// ����� DrawQuad() ������� � ����������� ���������� ���������� ������, ��������� �� ����
		// ���������� - �������������, ��������� ��� ���� ������� ���� ��� ������ � ����������� (binded) ��������,
		// ���� ��������� ���������������.
		//
		// ����� ������� DrawQuad() ������ ���� ����� Texture::Bind() ���� SetTexturing(false),
		// ����� ���������� ��������� ����� ���������������.
		//
		Render::DrawQuad(rect, uv);
	}

	//
	// ��������������� ������� �������������� ���������, ������ �� ����� ��������� �����.
	//
	Render::device.PopMatrix();
	
	//
	// �������� ������� ���������� �������, ������������ �� �������
	//
	FPoint currentPosition = spline.getGlobalFrame(math::clamp(0.0f, 1.0f, _timer / 6.0f));

	//
	// � ������ ������ � ���� �����������
	//
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(currentPosition.x, currentPosition.y, 0);
	_tex3->Draw();
	Render::device.PopMatrix();

	//
	// ���� ����� ��������� ��������������� ��� ���������.
	//
	Render::device.SetTexturing(false);
	
	//
	// ����� BeginColor() ������������ � ���� ������� ���� ������ � ������������� �����.
	//
	Render::BeginColor(Color(255, 128, 0, 255));
	
	//
	// ����� DrawRect() ������� � ����������� ���������� ���������� ������, ��������� �� ����
	// ���������� - �������������, ��������� ��� ���� ������� ���� ��� ������ � ����������� (binded) ��������,
	// ���� ��������� ���������������.
	//
	// ����� ������� DrawRect() ������ ���� ����� Texture::Bind(), ���� SetTexturing(false),
	// ����� ���������� ��������� ����� ���������������.
	//
	Render::DrawRect(924, 0, 100, 100);
	
	//
	// ����� EndColor() ������� �� ����� ������� ���� ������, �������������� �������.
	//
	Render::EndColor();
	
	//
	// ����� �������� ���������������.
	//
	Render::device.SetTexturing(true);

	//
	// ������ ��� �������, ������� �������� � ��������� (Update() ��� ���������� �������� �� �����).
	//
	_effCont.Draw();

	Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.f, CenterAlign);

}

void TestWidget::Update(float dt)
{
	//
	// ������� ��������� � ���������
	//
	_effCont.Update(dt);

	//
	// dt - �������� ������� � ��������, ��������� �� ����������� �����.
	// ��� ����� ��������� ������ ��������, � ����������� �� ������������������ �������
	// � ��������� �����.
	//
	// ��� ����, ����� ���� �������� �������� ������ �� �������, � �� �������� ��
	// ������������������ �������, �� ������ ������������ � �� ����� ��������.
	//
	// ����������� ��� ������ � ��������� ���������.
	//
	_timer += dt * 2;
	
	//
	// ����������� ������ � ��������� (0, 2�).
	// ��� ����� ������ ��� �������������� ��������� ������������ ��������,
	// ���� ����� ������������ ��������� ����� (float ������������) ��� ��������
	// �� ����������� ����� ���� ������� ������� (������� �������-�-��).
	//
	// �������� �������� ������ ������ (0, 2�), ������ ��� �� ���������� ��� ��������
	// ��� ������� ������, � ������� �������� ����� ������ ������������ ��������� ���������.
	//
	while (_timer > 2 * math::PI)
	{
		_timer -= 2 * math::PI;
	}
	
	//
	// ������������ ��������� ��������������� � ����������� �� �������.
	//
	_scale = 0.8f + 0.25f * sinf(_timer);
}

bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	if (Core::mainInput.GetMouseRightButton())
	{
		//
		// ��� ������� �� ������ ������ ����, ������ ������ ������ �� ������.
		//
		_eff = _effCont.AddEffect("Iskra");
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
		_eff->Reset();
		
		//
		// � �������� ���� ������� ��������.
		//
		_angle += 25;
		while (_angle > 360)
		{
			_angle -= 360;
		}
	}
	else
	{
		//
		// ��� ������� �� ����� ������ ����, ������ ��������� ������, ������� ���������� ���.
		//
		ParticleEffectPtr eff = _effCont.AddEffect("FindItem2");
		eff->posX = mouse_pos.x + 0.f;
		eff->posY = mouse_pos.y + 0.f;
		eff->Reset();

		//
		// �������� �������� � 0 �� 1 � ��������.
		//
		_curTex = 1 - _curTex;
	}
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
	if (_eff)
	{
		//
		// ���� ������ ������, �� ���������� ��� � ������� ����.
		//
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
	}
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	if (_eff)
	{
		//
		// ���� ������ ������, �� ��� ���������� ���� ��������� ���.
		//
		_eff->Finish();
		_eff = NULL;
	}
}

void TestWidget::AcceptMessage(const Message& message)
{
	//
	// ������� ����� ���������� ��������� � �����������.
	//

	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

void TestWidget::KeyPressed(int keyCode)
{
	//
	// keyCode - ����������� ��� �������.
	// � �������� �������� ��� �������� ����� ������������ ��������� VK_.
	//

	if (keyCode == VK_A) {
		// ������� �� ������� ������ A
	}
}

void TestWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode ��� ��������� �������
	//

	if (unicodeChar == L'�') {
		// ������� �� ���� ������� '�'
	}
}
