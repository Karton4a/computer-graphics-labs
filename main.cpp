#include "Lab1/Lab1.h"
#include "Lab2/Lab2.h"
#include "Lab3/Lab3.h"
#include "Lab4/Lab4.h"

int main()
{
	std::cout << "Anton Paschenko KV-83" << std::endl;
	int number = 0;
	while (number < 1 || number > 4)
	{
		std::cout << "Write lab number (1-4)" << std::endl;
		std::cin >> number;
	}

	olc::PixelGameEngine* lab = nullptr;
	switch (number)
	{
	case 1:
		lab = new Lab1();
		break;
	case 2:
		lab = new Lab2();
		break;
	case 3:
		lab = new Lab3();
		break;
	case 4:
		lab = new Lab4();
		break;
	default: return 0;
	}
	if (lab->Construct(400, 400, 2, 2))
		lab->Start();
	delete lab;
	return 0;
}