#include <iostream>
#include <chrono>
#include <fstream>
#include <cfloat>
#include <thread>
#include <cmath>
#include "Camera/include/camera.h"
#include "Math/include/Vec3.h"
#include "Math/include/Utilities.h"
#include "Raytracer/include/Ray.h"
#include "Objects/include/Sphere.h"
#include "Objects/include/HitableList.h"
#include "Materials/include/lambertian.h"
#include "Materials/include/metal.h"
#include "Materials/include/dielectric.h"

#define OUTPUT_TO_FILE

Vec3 **MyPPMFile;

Vec3 color(const Ray& r, std::shared_ptr<Hitable> world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		Ray scattered;
		Vec3 attenuation;

		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return Vec3(0.f, 0.f, 0.f);
		}
	}
	else
	{
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * Vec3(1.f, 1.f, 1.f) + t * Vec3(0.5f, 0.7f, 1.f);
	}
}

std::shared_ptr<Hitable> RandomWorld()
{
	int n = 500;

	std::list<std::shared_ptr<Hitable>> list;
	list.push_back(std::make_shared<Sphere>(Vec3(0.f, -1000.f, 0.f), 1000.f, std::make_shared<Lambertian>(Vec3(0.5f, 0.5f, 0.5f))));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = Utilities::random_float();
			Vec3 center(a + 0.9f * Utilities::random_float(), 0.2f, b + 0.9f * Utilities::random_float());

			if ((center - Vec3(4.f, 0.2f, 0.f)).length() > 0.9)
			{
				if (choose_mat < 0.8f) //diffuse
				{
					Vec3 albedo = Vec3(
						Utilities::random_float() * Utilities::random_float(),
						Utilities::random_float() * Utilities::random_float(),
						Utilities::random_float() * Utilities::random_float()
					);

					list.push_back(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Lambertian>(albedo)));
				}
				else if (choose_mat < 0.95f) //metal
				{
					Vec3 albedo = Vec3(
						0.5f * (1 + Utilities::random_float()),
						0.5f * (1 + Utilities::random_float()),
						0.5f * (1 + Utilities::random_float())
					);

					float fuzziness = 0.5f * Utilities::random_float();

					list.push_back(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Metal>(albedo, fuzziness)));
				}
				else //glass
				{
					list.push_back(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Dielectric>(1.f)));
				}
			}
		}
	}

	list.push_back(std::make_shared<Sphere>(Vec3(0.f, 1.f, 0.f), 1.f, std::make_shared<Dielectric>(1.5f)));
	list.push_back(std::make_shared<Sphere>(Vec3(-4.f, 1.f, 0.f), 1.f, std::make_shared<Lambertian>(Vec3(0.4f, 0.2f, 0.1f))));
	list.push_back(std::make_shared<Sphere>(Vec3(4.f, 1.f, 0.f), 1.f, std::make_shared<Metal>(Vec3(0.7f, 0.6f, 0.5f), 0.f)));

	return std::make_shared<HitableList>(list);
}

void EmitRay(std::shared_ptr<Hitable> world, Camera cam, int start_x, int end_x, int ny, int ns, int nx)
{	
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = start_x; i < end_x; i++)
		{
			Vec3 col(0.f, 0.f, 0.f);
			for (int s = 0; s < ns; s++)
			{
				float u = static_cast<float>(i + Utilities::random_float()) / static_cast<float>(nx);
				float v = static_cast<float>(j + Utilities::random_float()) / static_cast<float>(ny);

				Ray r = cam.get_ray(u, v);
				Vec3 p = r.point_at_parameter(2.f);
				col += color(r,world, 0);
			}
			
			col /= static_cast<float>(ns);

			MyPPMFile[i][j] = Vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
		}
	}
}

int main()
{
	auto start_time = std::chrono::high_resolution_clock::now();

	int nx = 1920;
	int ny = 1080;
	int ns = 100;

	//print header
#ifdef OUTPUT_TO_FILE
	std::ofstream outFile("test.ppm");
	outFile << "P3" << std::endl << nx << " " << ny << std::endl << 255 << std::endl;
#else
	std::cout << "P3" << std::endl << nx << " " << ny << std::endl << 255 << std::endl;
#endif

	//Camera setup
	Vec3 lookFrom(13.f, 2.f, 3.f);
	Vec3 lookAt(0.f, 0.f, 0.f);
	float dist_to_focus = 10.f;
	float aperture = .1f;
	Camera cam(lookFrom, lookAt, Vec3(0.f, 1.f, 0.f), 45.f, static_cast<float>(nx) /  static_cast<float>(ny), aperture, dist_to_focus);

	//create random scene here
	std::shared_ptr<Hitable> world = RandomWorld();

	MyPPMFile = new Vec3*[nx];
	for(int x = 0; x < nx; x++)
		MyPPMFile[x] = new Vec3[ny];

	const int numberOfThreads = 5;

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

	std::ofstream debugFile("debug.log");
	debugFile << "Duration: " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

	return 0;
}