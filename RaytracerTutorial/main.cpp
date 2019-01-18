#include <iostream>
#include <chrono>
#include <fstream>
#include <cfloat>
#include <random>
#include <thread>
#include "Camera/include/camera.h"
#include "Math/include/Vec3.h"
#include "Raytracer/include/Ray.h"
#include "Objects/include/Sphere.h"
#include "Objects/include/HitableList.h"

#define OUTPUT_TO_FILE

Vec3 **MyPPMFile;

Vec3 random_in_unit_sphere()
{
	//init random
	std::random_device random_device;
	std::default_random_engine random_engine(random_device());
	std::uniform_real_distribution<> random_distribution;

	Vec3 p;

	do
	{
		p = 2.f * Vec3(random_distribution(random_engine), random_distribution(random_engine), random_distribution(random_engine)) - Vec3(1.f, 1.f, 1.f);
	} while(p.squared_length() >= 1.f);

	return p;
}

Vec3 color(const Ray& r, std::shared_ptr<Hitable> world)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5f * color (Ray(rec.p, target-rec.p), world);
	}
	else
	{
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * Vec3(1.f, 1.f, 1.f) + t * Vec3(0.5f, 0.7f, 1.f);
	}
}

void EmitRay(std::shared_ptr<Hitable> world, Camera cam, int start_x, int end_x, int ny, int ns, int nx)
{	
	//init random
	std::random_device random_device;
	std::default_random_engine random_engine(random_device());
	std::uniform_real_distribution<> random_distribution;

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = start_x; i < end_x; i++)
		{
			Vec3 col(0.f, 0.f, 0.f);
			for (int s = 0; s < ns; s++)
			{
				float u = static_cast<float>(i + random_distribution(random_engine)) / static_cast<float>(nx);
				float v = static_cast<float>(j + random_distribution(random_engine)) / static_cast<float>(ny);

				Ray r = cam.get_ray(u, v);
				Vec3 p = r.point_at_parameter(2.f);
				col += color(r,world);
			}
			
			col /= static_cast<float>(ns);

			MyPPMFile[i][j] = Vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
		}
	}
}

int main()
{
	auto start_time = std::chrono::high_resolution_clock::now();

	int nx = 200;
	int ny = 100;
	int ns = 100;

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

	std::list<std::shared_ptr<Hitable>> list;

	list.push_back(std::make_shared<Sphere>(Vec3(0.f, 0.f, -1.f), 0.5f));
	list.push_back(std::make_shared<Sphere>(Vec3(0.f, -100.5f, -1.f), 100.f));

	std::shared_ptr<Hitable> world = std::make_shared<HitableList>(list);

	Camera cam;

	MyPPMFile = new Vec3*[nx];
	for(int x = 0; x < nx; x++)
		MyPPMFile[x] = new Vec3[ny];

	const int numberOfThreads = 10;

	//insert thread creation here
	std::vector<std::thread> threads;
	for (int i = 0; i < numberOfThreads; i++)
	{
		threads.push_back(std::thread(EmitRay, world, cam, i * (nx / numberOfThreads), (i + 1) * (nx / numberOfThreads), ny, ns, nx));
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			int ir = static_cast<int>(255.99f * MyPPMFile[i][j].r());
			int ig = static_cast<int>(255.99f * MyPPMFile[i][j].g());
			int ib = static_cast<int>(255.99f * MyPPMFile[i][j].b());

#ifdef OUTPUT_TO_FILE
			outFile << ir << " " << ig << " " << ib << std::endl;
#else
			std::cout << ir << " " << ig << " " << ib << std::endl;
#endif
		}
	}

	for (int x = 0; x < nx; x++)
		delete[] MyPPMFile[x];
	delete[] MyPPMFile;

	auto end_time = std::chrono::high_resolution_clock::now();

	std::cout << "Duration: " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

	return 0;
}