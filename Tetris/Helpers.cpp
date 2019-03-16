#include "stdafx.h"
#include "Helpers.h"

COLORREF Helpers::RgbaToRgb(COLORREF bckgrnd, COLORREF color, int alpha)
{
	int bgRed = GetRValue(bckgrnd);
	int bgGreen = GetGValue(bckgrnd);
	int bgBlue = GetBValue(bckgrnd);

	int topRed = GetRValue(bckgrnd);
	int topGreen = GetGValue(bckgrnd);
	int topBlue = GetBValue(bckgrnd);

	return RGB((1 - alpha) * bgRed + alpha * topRed,
		(1 - alpha) * bgGreen + alpha * topGreen,
		(1 - alpha) * bgBlue + alpha * topBlue);
}

/*int Helpers::findArrayLength(int * ptrToArr)
{
	int numRows = 0;
	int* ptr = ptrToArr;

	while (*ptr != '\0')
	{
		numRows++;
		ptr++;
	}

	return numRows;
}*/