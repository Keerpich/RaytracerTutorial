#include <iostream>
#include <fstream>
#include "Math/include/Vec3.h"
#include "Raytracer/include/Ray.h"

#define OUTPUT_TO_FILE

Vec3 color(const Ray& r)
{
	Vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

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

	Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vec3 horizontal(4.0f, 0.0f, 0.0f);
	Vec3 vertical(0.0f, 2.0f, 0.0f);
	Vec3 origin(0.0f, 0.0f, 0.0f);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(nx);
			float v = static_cast<float>(j) / static_cast<float>(ny);

			Ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			Vec3 col = color(r);
			
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