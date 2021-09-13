#pragma once
#define OLC_PGE_APPLICATION
#include "../graphic/olcPixelGameEngine.h"
#include <vector>
#include <chrono>


class Lab1 : public olc::PixelGameEngine
{
public:
	Lab1()
	{
		sAppName = "Lab1";
	}

public:
	bool OnUserCreate() override
	{
		auto begin = std::chrono::steady_clock::now();
		DDA(0, 0, 200, 200);
		auto end = std::chrono::steady_clock::now();
		auto dda_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		begin = std::chrono::steady_clock::now();
		BresenhamLine(200, 0, 300, 200);
		end = std::chrono::steady_clock::now();
		auto bresenham_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		begin = std::chrono::steady_clock::now();
		CircleBres(100, 300, 50);
		end = std::chrono::steady_clock::now();
		auto bresenham_circle_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		begin = std::chrono::steady_clock::now();
		WuLine(210, 210, 300, 400);
		end = std::chrono::steady_clock::now();
		auto wu_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		std::cout << "DDA line calculated in " << dda_time << " (ms)" << std::endl;
		std::cout << "Bresenham line calculated in " << bresenham_time << " (ms)" << std::endl;
		std::cout << "Bresenham circle calculated in " << bresenham_circle_time << " (ms)" << std::endl;
		std::cout << "Wu line calculated in " << wu_time << " (ms)" << std::endl;

		DrawLine(200, 0, 200, 400);
		DrawLine(0, 200, 400, 200);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		for (const auto& t : m_Text)
		{
			DrawStringDecal(t.first, t.second);
		}
		return true;
	}
private:
	void DDA(int X0, int Y0, int X1, int Y1)
	{
		int dx = X1 - X0;
		int dy = Y1 - Y0;

		int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

		float Xinc = dx / (float)steps;
		float Yinc = dy / (float)steps;

		float X = X0;
		float Y = Y0;
		for (int i = 0; i <= steps; i++)
		{
			Draw((int32_t)X, (int32_t)Y);
			X += Xinc;
			Y += Yinc;
		}
	}

	void BresenhamLine(int x0, int y0, int x1, int y1) 
	{
		bool steep = false;
		if (std::abs(x0 - x1) < std::abs(y0 - y1)) 
		{
			std::swap(x0, y0);
			std::swap(x1, y1);
			steep = true;
		}
		if (x0 > x1) 
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		for (int x = x0; x <= x1; x++) 
		{
			float t = (x - x0) / (float)(x1 - x0);
			int y = y0 * (1. - t) + y1 * t;
			if (steep) {
				Draw(y, x);
			}
			else {
				Draw(x, y);
			}
		}
	}

	void CircleBres(int x1, int y1, int r)
	{
		float x, y, p;
		x = 0;
		y = r;
		p = 3 - (2 * r);
		while (x <= y)
		{
			Draw(x1 + x, y1 + y);
			Draw(x1 - x, y1 + y);
			Draw(x1 + x, y1 - y);
			Draw(x1 - x, y1 - y);
			Draw(x1 + y, y1 + x);
			Draw(x1 + y, y1 - x);
			Draw(x1 - y, y1 + x);
			Draw(x1 - y, y1 - x);
			x = x + 1;
			if (p < 0)
			{
				p = p + 4 * x + 6;
			}
			else
			{
				p = p + 4 * (x - y) + 10;
				y = y - 1;
			}

		}
	}


	static float fPartOfNumber(float x)
	{
		if (x > 0) return x - (int)x;
		else return x - ((int)x + 1);

	}

	static float rfPartOfNumber(float x)
	{
		return 1 - fPartOfNumber(x);
	}

	void WuLine(int x0, int y0, int x1, int y1)
	{
		int steep = std::abs(y1 - y0) > std::abs(x1 - x0);
		if (steep)
		{
			std::swap(x0, y0);
			std::swap(x1, y1);
		}
		if (x0 > x1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		float dx = x1 - x0;
		float dy = y1 - y0;
		float gradient = dy / dx;
		if (dx == 0.0)
			gradient = 1;

		int xpxl1 = x0;
		int xpxl2 = x1;
		float intersectY = y0;

		if (steep)
		{
			int x;
			for (x = xpxl1; x <= xpxl2; x++)
			{

				float bright = rfPartOfNumber(intersectY);
				Draw((int)intersectY, x, olc::PixelF(bright,bright,bright));
				bright = fPartOfNumber(intersectY);
				Draw((int)intersectY - 1, x, olc::PixelF(bright,bright,bright));
				intersectY += gradient;
			}
		}
		else
		{
			int x;
			for (x = xpxl1; x <= xpxl2; x++)
			{
				float bright = rfPartOfNumber(intersectY);
				Draw(x, (int)intersectY, olc::PixelF(bright,bright,bright));
				bright = fPartOfNumber(intersectY);
				Draw( x, (int)intersectY - 1, olc::PixelF(bright,bright,bright) );
				intersectY += gradient;
			}
		}

	}
private:
	std::array<std::pair<olc::vi2d, std::string>, 4> m_Text =
	{
		std::pair<olc::vi2d,std::string>{ {50,20}, "DDA"},
		std::pair<olc::vi2d,std::string>{ {250,20}, "Bresenham Line"},
		std::pair<olc::vi2d,std::string>{ {50,220}, "Bresenham Circle"},
		std::pair<olc::vi2d,std::string>{ {250,220}, "Wu line"},
	};
};