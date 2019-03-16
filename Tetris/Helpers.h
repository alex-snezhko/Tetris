#pragma once

class Helpers
{
	Helpers() {};
	~Helpers() {};

public:
	static COLORREF RgbaToRgb(COLORREF bckgrnd, COLORREF color, int alpha);
};
