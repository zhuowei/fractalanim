// FracRotate.cpp : main project file.

#include "stdafx.h"
#include "sintable.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

#define WIDTH 1280
#define HEIGHT 800

UInt32 framebuffer[WIDTH*HEIGHT];

static float sindeg(int val) {
	val = val % 360;
	if (val < 0) val = 360 + val;
	//Console::Write(val + L":");
	return sin_table[val];
}

#define cosdeg(val) sindeg(val + 90)
#define MAX(x1, x2) (x1 > x2? x1: x2)
#define MIN(x1, x2) (x1 < x2? x1: x2)
#define ABS(x) ((x) < 0? -1 * (x): (x))
#define PIXELADDR(x, y) (y * WIDTH + x)

void renderLine(int x0, int y0, int x1, int y1) {
	/*Console::Write(x0);
	Console::Write(L", ");
	Console::Write(y0);
	Console::Write(L" ");
	Console::Write(x1);
	Console::Write(L", ");
	Console::WriteLine(y1);*/
	/* pulled from Wikipedia page on Bresenham's line algorithm */
	int deltaX = ABS(x1 - x0);
	int deltaY = ABS(y1 - y0);
	int sx = x0 < x1 ? 1 : -1; /* directions */
	int sy = y0 < y1 ? 1 : -1;
	int err = deltaX - deltaY;
	//Console::Write(deltaX);
	//Console::Write(deltaY);
	//Console::WriteLine(err);
	for(;;) {
		int addr = PIXELADDR(x0, y0);
		if (addr >= 0 && addr < WIDTH*HEIGHT) framebuffer[addr] = 0xff000000;
		if (x0 == x1 && y0 == y1) break;
		int e2 = err*2;
		if (e2 > -deltaY) {
			err -= deltaY;
			x0 += sx;
		}
		if (e2 < deltaX) {
			err += deltaX;
			y0 += sy;
		}
	}
}

void render(int beginX, int beginY, int length, int curDegree, int deltaDegree) {
	int endX = beginX + (int)(length * sindeg(curDegree));
	int endY = beginY - (int)(length * cosdeg(curDegree));
	//Console::WriteLine(sindeg(curDegree) + L" " + beginX + L" " + beginY + L" " + endX + L" " + endY + L" " + length + L" " + curDegree);
	renderLine(beginX, beginY, endX, endY);
	int nextLength = length * 7 / 10;
	if (nextLength > 0) {
		render(endX, endY, nextLength, curDegree - deltaDegree, deltaDegree);
		render(endX, endY, nextLength, curDegree + deltaDegree, deltaDegree);
	}
}

void renderFrame(int degrees) {
	render(WIDTH / 2, HEIGHT, HEIGHT / 2, 0, degrees);
}

void clrscr() {
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		framebuffer[i] = 0xffffffff;
	}
}

int main(array<System::String ^> ^args)
{
	Bitmap^ bmp = gcnew Bitmap(WIDTH, HEIGHT, 4 * WIDTH, PixelFormat::Format32bppArgb, (IntPtr) framebuffer);
	for (int degrees = 0; degrees < 180; degrees++) {
		Console::WriteLine(L"Frame");
		clrscr();
		renderFrame(degrees);
		bmp->Save("C:\\Users\\zhuowei\\outpic\\out" + degrees + ".png");
	}
    return 0;
}
