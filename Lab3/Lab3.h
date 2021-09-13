#pragma once
#define OLC_PGE_APPLICATION
#include "../graphic/olcPixelGameEngine.h"
#include <functional>
#include <string>

class Lab3 : public olc::PixelGameEngine
{
public:
	Lab3()
	{
		sAppName = "Lab3";
	}

public:
	bool OnUserCreate() override
	{
		m_DrawBuffer = GetDrawTarget();

		m_Algo = [this](int x, int y, olc::Pixel c1, olc::Pixel c2) { FloodFill4(x, y, c1, c2); };
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::Key::Q).bHeld)
		{
			m_Algo = [this](int x, int y, olc::Pixel c1, olc::Pixel c2) { FloodFill4(x, y, c1, c2); };
			m_CurrentString = "Fill4";
			Clear(olc::BLACK);
		}
		if (GetKey(olc::Key::W).bHeld)
		{
			m_Algo = [this](int x, int y, olc::Pixel c1, olc::Pixel c2) { FloodFill4Stack(x, y, c1, c2); };
			m_CurrentString = "Fill4Stack";
			Clear(olc::BLACK);
		}
		if (GetKey(olc::Key::E).bHeld)
		{
			m_Algo = [this](int x, int y, olc::Pixel c1, olc::Pixel c2) { FloodFill8(x, y, c1, c2); };
			m_CurrentString = "Fill8";
			Clear(olc::BLACK);
		}
		if (GetKey(olc::Key::R).bHeld)
		{
			m_Algo = [this](int x, int y, olc::Pixel c1, olc::Pixel c2) { FloodFillScanLine(x, y, c1, c2); };
			m_CurrentString = "FillScanline";
			Clear(olc::BLACK);
		}

		if (GetMouse(0).bHeld)
		{
			m_Algo(GetMouseX(), GetMouseY(), olc::WHITE, olc::BLACK);
		}
		DrawString(0, 5, "QWER to change algo");
		DrawString(250, 10, m_CurrentString);
		DrawRect(20, 100, 40, 40);
		DrawCircle(200, 200, 50);
		DrawTriangle(10, 200, 50, 200, 25, 300);

		return true;
	}

private:

	void FloodFill4(int x, int y, olc::Pixel new_color, olc::Pixel old_color)
	{
		if (x >= 0 && x < ScreenWidth() && y >= 0 && y < ScreenHeight() && m_DrawBuffer->GetPixel(x,y) == old_color && m_DrawBuffer->GetPixel(x, y) != new_color)
		{
			Draw(x, y, new_color);

			FloodFill4(x + 1, y, new_color, old_color);
			FloodFill4(x - 1, y, new_color, old_color);
			FloodFill4(x, y + 1, new_color, old_color);
			FloodFill4(x, y - 1, new_color, old_color);
		}
	}

	void FloodFill4Stack(int x, int y, olc::Pixel new_color, olc::Pixel old_color)
	{
		if (new_color == old_color) return; //avoid infinite loop

		static const int dx[4] = { 0, 1, 0, -1 }; // relative neighbor x coordinates
		static const int dy[4] = { -1, 0, 1, 0 }; // relative neighbor y coordinates
		int w = ScreenWidth();
		int h = ScreenHeight();
		std::vector<olc::vi2d> stack;
		stack.emplace_back(x, y);
		while (!stack.empty())
		{
			olc::vi2d l_v = stack.back();
			stack.pop_back();

			Draw(l_v,new_color);
			for (int i = 0; i < 4; i++) 
			{
				int nx = l_v.x + dx[i];
				int ny = l_v.y + dy[i];
				if (nx >= 0 && nx < w && ny >= 0 && ny < h && m_DrawBuffer->GetPixel(nx, ny) == old_color) 
				{
					stack.emplace_back(nx, ny);
				}
			}
		}
	}
	void FloodFill8(int x, int y, olc::Pixel new_color, olc::Pixel old_color)
	{
		if (x >= 0 && x < ScreenWidth() && y >= 0 && y < ScreenHeight() && m_DrawBuffer->GetPixel(x, y) == old_color && m_DrawBuffer->GetPixel(x, y) != new_color)
		{
			Draw(x, y, new_color);

			FloodFill8(x + 1, y, new_color, old_color);
			FloodFill8(x - 1, y, new_color, old_color);
			FloodFill8(x, y + 1, new_color, old_color);
			FloodFill8(x, y - 1, new_color, old_color);
			FloodFill8(x + 1, y + 1, new_color, old_color);
			FloodFill8(x - 1, y - 1, new_color, old_color);
			FloodFill8(x - 1, y + 1, new_color, old_color);
			FloodFill8(x + 1, y - 1, new_color, old_color);
		}
	}

	void FloodFillScanLine(int x, int y, olc::Pixel newColor, olc::Pixel oldColor)
	{
		if (oldColor == newColor) return;
		if (m_DrawBuffer->GetPixel(x,y) != oldColor) return;

		int x1;
		int w = ScreenWidth();
		int h = ScreenHeight();
		//draw current scanline from start position to the right
		x1 = x;
		while (x1 < w && m_DrawBuffer->GetPixel(x1, y) == oldColor)
		{
			Draw(x1, y, newColor);
			x1++;
		}

		//draw current scanline from start position to the left
		x1 = x - 1;
		while (x1 >= 0 && m_DrawBuffer->GetPixel(x1, y) == oldColor)
		{
			Draw(x1, y, newColor);
			x1--;
		}

		//test for new scanlines above
		x1 = x;
		while (x1 < w && m_DrawBuffer->GetPixel(x1, y) == newColor)
		{
			if (y > 0 && m_DrawBuffer->GetPixel(x1, y-1) == oldColor)
			{
				FloodFillScanLine(x1, y - 1, newColor, oldColor);
			}
			x1++;
		}
		x1 = x - 1;
		while (x1 >= 0 && m_DrawBuffer->GetPixel(x1, y) == newColor)
		{
			if (y > 0 && m_DrawBuffer->GetPixel(x1, y-1) == oldColor)
			{
				FloodFillScanLine( x1, y - 1, newColor, oldColor);
			}
			x1--;
		}

		//test for new scanlines below
		x1 = x;
		while (x1 < w && m_DrawBuffer->GetPixel(x1, y) == newColor)
		{
			if (y < h - 1 && m_DrawBuffer->GetPixel(x1, y+1) == oldColor)
			{
				FloodFillScanLine(x1, y + 1, newColor, oldColor);
			}
			x1++;
		}
		x1 = x - 1;
		while (x1 >= 0 && m_DrawBuffer->GetPixel(x1, y) == newColor)
		{
			if (y < h - 1 && m_DrawBuffer->GetPixel(x1, y+1) == oldColor)
			{
				FloodFillScanLine(x1, y + 1, newColor, oldColor);
			}
			x1--;
		}
	}
private:
	std::string m_CurrentString = "Fill4";
	std::function<void(int x, int y, olc::Pixel newColor, olc::Pixel oldColor)> m_Algo;
	olc::Sprite* m_DrawBuffer;
};