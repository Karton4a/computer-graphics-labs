#pragma once
#define OLC_PGE_APPLICATION
#include "../graphic/olcPixelGameEngine.h"

class Lab2 : public olc::PixelGameEngine
{
public:
	Lab2()
	{
		sAppName = "Lab2";
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		auto mouse_pos = GetMousePos();
		olc::vi2d translate {};
		olc::vf2d scale {1.f,1.f};
		float angle = 0;
		float angle_speed = 0.01f;
		int translate_speed = 10;
		float scale_speed = 1.1f;
		if (GetKey(olc::Key::SPACE).bPressed)
		{
			for (auto& point : m_Points)
			{
				point = MirrorByY(point);
			}
		}
		if (GetKey(olc::Key::B).bPressed)
		{
			for (auto& point : m_Points)
			{
				point = MirrorByX(point);
			}
		}

		if (GetKey(olc::Key::W).bHeld)
		{
			scale.y = scale_speed;
		}
		else if (GetKey(olc::Key::S).bHeld)
		{
			scale.y = 1 / scale_speed;
		}

		if (GetKey(olc::Key::A).bHeld)
		{
			scale.x = 1 / scale_speed;
		}
		else if (GetKey(olc::Key::D).bHeld)
		{
			scale.x = scale_speed;
		}

		if (GetKey(olc::Key::Q).bHeld)
		{
			angle =  angle_speed;
		}
		else if (GetKey(olc::Key::E).bHeld)
		{
			angle = -1 * angle_speed;
		}
		if (GetKey(olc::Key::LEFT).bHeld)
		{
			translate.x = -1 * translate_speed;
		}
		else if (GetKey(olc::Key::RIGHT).bHeld)
		{
			translate.x = translate_speed;
		}
		if (GetKey(olc::Key::UP).bHeld)
		{
			translate.y = -1*translate_speed;
		}
		else if (GetKey(olc::Key::DOWN).bHeld)
		{
			translate.y = translate_speed;
		}

		for (auto& point : m_Points)
		{
			point = Scale(point, scale);
		}
		for (auto& point : m_Points)
		{
			point = Rotate(point, angle);
		}
		for (auto& point : m_Points)
		{
			point = Shift(point, translate);
		}

		if (GetMouse(1).bPressed)
		{
			auto res = std::find_if(m_Points.begin(), m_Points.end(),
				[&mouse_pos,this](auto& el) { return inCircle(mouse_pos, el, 20); });

			if (res != m_Points.end())
			{
				m_Points.erase(res);
			}
		}
		if (GetMouse(0).bPressed)
		{
			auto res = std::find_if(m_Points.begin(), m_Points.end(),
				[&mouse_pos, this](auto& el) { return inCircle(mouse_pos, el, 20); });
			if (res == m_Points.end())
			{
				m_Points.push_back(mouse_pos);
			}
		}
		if (GetMouse(0).bHeld)
		{
			for (auto& point : m_Points)
			{
				if (inCircle(mouse_pos, point, 30))
				{
					point = mouse_pos;
					break;
				}
			}
		}

		Clear(olc::DARK_BLUE);
		DrawString(0, 0, "Left to add. Right to remove. WASD to scale");
		DrawString(0, 15, "Keys to move. QE to rotate. Space to mirror Y.");
		DrawString(0, 30, "B to mirror X.");//B to mirror X
		//WASD to scale. Keys to move.QE to rotate
		BezierCurve(m_Points);

		for (auto point : m_Points)
		{
			FillCircle(point, 5,olc::RED);
		}

		if (m_Points.size() > 2)
		{
			for (size_t i = 0; i < m_Points.size() - 1; i++)
			{
				DrawLine(m_Points[i], m_Points[i + 1], olc::RED, 0b1011101110101110);
			}
		}
		return true;
		
	}
private:
	void BezierCurve(std::vector<olc::vi2d>& points)
	{
		double u = 0;
		int i = 0;
		auto n = points.size() - 1;
		for (u = 0.0; u <= 1.0; u += 0.0001)
		{
			double res_x = 0;
			double res_y = 0;
			for (size_t i = 0; i < points.size(); i++)
			{
				res_x += Combinations(n, i) * std::pow(1 - u, n - i) * std::pow(u,i) * points[i].x;
				res_y += Combinations(n, i) * std::pow(1 - u, n - i) * std::pow(u, i) * points[i].y;
			}
			Draw((int)res_x, (int)res_y);
		}

	}
	bool inCircle(olc::vi2d point, olc::vi2d& center,int32_t radius)
	{
		return (std::pow(point.x - center.x , 2) + std::pow(point.y - center.y, 2)) <= std::pow(radius , 2);
	}
	int Combinations(int n,int k)
	{
		return Factorial(n) / (Factorial(k) * Factorial(n - k));
	}
	int Factorial(int n)
	{
		int prod = 1;
		for (size_t i = 2; i <= n; i++)
		{
			prod *= i;
		}
		return prod;
	}
	olc::vi2d Rotate(olc::vi2d point,float angle)
	{
		return olc::vi2d( point.x * std::cos(angle) - point.y * std::sin(angle),
			point.x * std::sin(angle) + point.y * std::cos(angle) );
	}
	olc::vi2d Shift(olc::vi2d point, olc::vi2d shift)
	{
		return olc::vi2d(point + shift);
	}
	olc::vi2d Scale(olc::vi2d point, olc::vf2d scale)
	{
		return olc::vi2d(point.x * scale.x, point.y * scale.y);
	}
	olc::vi2d MirrorByY(olc::vi2d original) 
	{
		return olc::vi2d(original.x, ScreenHeight() - original.y);
	}
	olc::vi2d MirrorByX(olc::vi2d original) 
	{
		return olc::vi2d(ScreenWidth() - original.x, original.y);
	}
private:
	std::vector<olc::vi2d> m_Points{ {45,142}, {78,221}, {124,345}, {200,50} };
};