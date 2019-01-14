#include <iostream>
#include <fstream>
#include "Math/include/Vec3.h"

#define OUTPUT_TO_FILE

int main()
{
	int nx = 200;
	int ny = 100;

	//print header
#ifdef OUTPUT_TO_FILE
	std::ofstream outFile("test.ppm");
	outFile << "P3" << std::endl << nx << " " << ny << std::endl << 255 << std::endl;
#else
	std::cout << "P3" << std::endl << nx << " " << ny << std::endl << 255 << std::endl;
#endif

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			Vec3 col(static_cast<float>(i) / static_cast<float>(nx),
					static_cast<float>(j) / static_cast<float>(ny),
					0.2f);
			
			int ir = static_cast<int>(255.99f * col.r());
			int ig = static_cast<int>(255.99f * col.g());
			int ib = static_cast<int>(255.99f * col.b());

#ifdef OUTPUT_TO_FILE
			outFile << ir << " " << ig << " " << ib << std::endl;
#else
			std::cout << ir << " " << ig << " " << ib << std::endl;
#endif
		}
	}

	return 0;
}