#pragma once

class PushTheString
{
public:
	PushTheString(char *str, bool isPushed);
	~PushTheString();

	//void Init();

	//void Update(float dt);
	void Draw();

	bool MouseDown(const IPoint& mouse_pos);
	void MouseMove(const IPoint& mouse_pos);

	bool PointIntoRect(IPoint point);

	void Show(float sec);
	void Hide(float sec);
	//void Scale(float scale);
	//void MouseUp(const IPoint& mouse_pos);

	void SetScale(float startScale, float endScale) { _startScale = startScale; _scale = startScale; _endScale = endScale; }
	void SetColors(Color startColor, Color mouseColor) { _startColor = startColor; _color = startColor; _mouseColor = mouseColor; }
	void SetPositions(FPoint startPos, FPoint endPos) { _startPosition = startPos; _position = startPos; _endPosition = endPos; }
	void SetString(const char *str);

private:
	char *_str;
	bool _isPushed;

	float _scale;
	FPoint _position;
	Color _color;

	float _startScale;
	float _endScale;

	float _alpha;

	FPoint _startPosition;
	FPoint _endPosition;

	Color _startColor;
	Color _mouseColor;
};

