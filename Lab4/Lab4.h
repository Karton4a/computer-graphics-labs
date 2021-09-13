#pragma once
#define OLC_PGE_APPLICATION
#include "../graphic/olcPixelGameEngine.h"
#include <functional>
#include <string>
#include <stdlib.h>
#include <time.h>  

class Lab4 : public olc::PixelGameEngine
{
public:
	Lab4()
	{
		sAppName = "Lab4";
	}

public:
	bool OnUserCreate() override
	{
        Clear(olc::DARK_BLUE);
        Bransley(500000);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        if (GetKey(olc::Key::Q).bHeld)
        {
            Clear(olc::DARK_BLUE);
            SnowFlake({ 25, 75 }, { 250, 75 }, 4);
            SnowFlake({ 135, 245 }, { 25, 75 }, 4);
            SnowFlake({ 250, 75 }, { 135, 245 }, 4);

        }
        if (GetKey(olc::Key::W).bHeld)
        {
            Clear(olc::DARK_BLUE);
            Mandelbrot(-1.5, 0.7, -1., 1.);
        }
        if (GetKey(olc::Key::E).bHeld)
        {
            Clear(olc::DARK_BLUE);
            Bransley(500000);
        }
        DrawString(0, 10, "q w e to change");
		return true;
	}

private:
    void SnowFlake(olc::vf2d A, olc::vf2d B, int depth)
    {
        if (depth < 0) 
        {
            return;
        }
        auto C = (A * 2 + B) / 3;
        auto D = (B * 2 + A) / 3;
        auto F = (A + B) / 2;
        auto V1 = (F - A) / (F - A).mag();
        olc::vf2d V2{ V1.y,-V1.x };
        auto E = V2 * sqrt(3) / 6 * (B - A).mag() + F;
        DrawLine(A, B,olc::YELLOW);
        if (depth != 0)
        {
            DrawLine(C, D);
        }
        SnowFlake(A, C, depth - 1);
        SnowFlake(C, E, depth - 1);
        SnowFlake(E, D, depth - 1);
        SnowFlake(D, B, depth - 1);
    }
	void Mandelbrot(double minR, double maxR, double minI, double maxI)
	{
        for (int y = 0; y < ScreenHeight(); y++)
        {
            for (int x = 0; x < ScreenWidth(); x++)
            {

                double cr = mapToReal(x, ScreenWidth(), minR, maxR);
                double ci = mapToImaginary(y, ScreenHeight(), minI, maxI);

                int n = findMandelbrot(cr, ci, 256);

                int r = ((int)(n * cosf(n)) % 256);
                int g = ((n * 10) % 256);
                int b = (n % 256);
                Draw(x, y, olc::PixelF(r, g, b));
            }

        }
	}
    int findMandelbrot(double cr, double ci, double max_iterations) 
    {
        int i = 0;
        double zr = 0.0, zi = 0.0;
        while (i < max_iterations && zr * zr + zi * zi < 4.0)

        {
            double temp = zr * zr - zi * zi + cr;
            zi = 2.0 * zr * zi + ci;
            zr = temp;
            i++;
        }
        return i;

    }
    double mapToReal(int x, int imageWidth, double minR, double maxR)
    {
        double range = maxR - minR;
        return x * (range / imageWidth) + minR;

    }
    double mapToImaginary(int y, int imageHeight, double minI, double maxI)
    {
        double range = maxI - minI;
        return y * (range / imageHeight) + minI;

    }
    void Bransley(int iteration_count)
    {
        srand(time(NULL));
        double x = 1.0;
        double y = 0.0;
        for (size_t i = 0; i < iteration_count; i++)
        {
            double p = (double) rand() / RAND_MAX; // генерируем случайное число от 0 до 1 (вероятность)
            double oldx = x; // запоминаем старое значение х
            if (p <= 0.85) // верхняя часть
            {
                x = 0.85 * x + 0.04 * y;
                y = -0.04 * oldx + 0.85 * y + 1.6;
            }
            else
            {
                if (p <= 0.92) // левый лист
                {
                    x = 0.25 * x - 0.26 * y;
                    y = 0.23 * oldx + 0.25 * y + 1.6;
                }
                else if (p <= 0.99) // правый лист
                {
                    x = -0.15 * x + 0.3 * y;
                    y = 0.26 * oldx + 0.2 * y + 0.44;
                }
                else // стебель
                {
                    x = 0.0;
                    y = 0.16 * y;
                }
            }
            
            int green = (double)(rand()) / RAND_MAX * (255 - 125) + 125;
            Draw(200 + round(40 * x), 400 - round(40 * y), olc::PixelF(0,green,0));
        }
    }

	
};