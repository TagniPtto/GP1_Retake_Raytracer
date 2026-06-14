#pragma once
#include <fstream>
#include "Math.h"
#include "DataTypes.h"

namespace dae
{
	namespace GeometryUtils
	{
#pragma region Sphere HitTest
		//SPHERE HIT-TESTS
		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			const Vector3 SphereToRay = ray.origin - sphere.origin;

			const float c = Vector3::Dot(SphereToRay, SphereToRay) - Square(sphere.radius);
			const float b = Vector3::Dot(SphereToRay, ray.direction) * 2.0f;
			const float a	= Vector3::Dot(ray.direction, ray.direction);
			
			const float D = Square(b) - 4 * a * c;
			
			if (D < 0)
			{
				return false;
			}
			const float t1 = (-b + sqrtf(D)) / (2 * a);
			const float t2 = (-b - sqrtf(D)) / (2 * a);
			
			if (!ignoreHitRecord) {
				hitRecord.t = std::min(t1, t2);
				hitRecord.origin = hitRecord.t * ray.direction + ray.origin;
				hitRecord.normal = (hitRecord.origin - sphere.origin).Normalized();
				hitRecord.materialIndex = sphere.materialIndex;
				hitRecord.didHit = true;
			}
		
			return true;
		}

		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Sphere(sphere, ray, temp, true);
		}
#pragma endregion
#pragma region Plane HitTest
		//PLANE HIT-TESTS
		inline bool HitTest_Plane(const Plane& plane, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{

			float denominator = Vector3::Dot(plane.normal, ray.direction);
			if (denominator < .001f) {
				return false;
			}

			if (!ignoreHitRecord) {
				float numerator = Vector3::Dot(plane.origin - ray.origin , plane.normal); 
				hitRecord.t = numerator/denominator;
				hitRecord.materialIndex = plane.materialIndex;
				hitRecord.normal = plane.normal;
				hitRecord.didHit = true;
				hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
			}
			return true;
		}

		inline bool HitTest_Plane(const Plane& plane, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Plane(plane, ray, temp, true);
		}
#pragma endregion
#pragma region Triangle HitTest
		//TRIANGLE HIT-TESTS
		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{

			const float result = Vector3::Dot(ray.direction, triangle.normal);
			if (triangle.cullMode == TriangleCullMode::BackFaceCulling)
			{
				if (result < 0) return false;
			}
			if (triangle.cullMode == TriangleCullMode::FrontFaceCulling)
			{
				if (result > 0) return false;
			}

			Plane trianglePlane{
				.origin = triangle.v0,
				.normal = triangle.normal,
				.materialIndex = triangle.materialIndex
			};

			if (HitTest_Plane(trianglePlane, ray, hitRecord, ignoreHitRecord)) {

				Vector3  v0TOHit = hitRecord.origin - triangle.v0;
				Vector3  v1TOHit = hitRecord.origin - triangle.v1;
				Vector3  v2TOHit = hitRecord.origin - triangle.v2;

				Vector3 side01Normal = Vector3::Cross(triangle.v1 - triangle.v0, triangle.normal);
				Vector3 side12Normal = Vector3::Cross(triangle.v2 - triangle.v1, triangle.normal);
				Vector3 side20Normal = Vector3::Cross(triangle.v0 - triangle.v2, triangle.normal);

				if (Vector3::Dot(side01Normal , v0TOHit) > 0 &&
					Vector3::Dot(side12Normal , v1TOHit) > 0 &&
					Vector3::Dot(side20Normal , v2TOHit) > 0 )
				{
					return true;
				}
				else {
					hitRecord.didHit = false;
				}
			}
			return false;
		}

		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Triangle(triangle, ray, temp, true);
		}
#pragma endregion
#pragma region TriangeMesh HitTest
		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			//todo W5
			throw std::runtime_error("Not Implemented Yet");
			return false;
		}

		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_TriangleMesh(mesh, ray, temp, true);
		}
#pragma endregion
	}

	namespace LightUtils
	{
		//Direction from target to light
		inline Vector3 GetDirectionToLight(const Light& light, const Vector3 origin)
		{
			//todo W3
			throw std::runtime_error("Not Implemented Yet");
			return {};
		}

		inline ColorRGB GetRadiance(const Light& light, const Vector3& target)
		{
			//todo W3
			throw std::runtime_error("Not Implemented Yet");
			return {};
		}
	}

	namespace Utils
	{
		//Just parses vertices and indices
#pragma warning(push)
#pragma warning(disable : 4505) //Warning unreferenced local function
		static bool ParseOBJ(const std::string& filename, std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<int>& indices)
		{
			std::ifstream file(filename);
			if (!file)
				return false;

			std::string sCommand;
			// start a while iteration ending when the end of file is reached (ios::eof)
			while (!file.eof())
			{
				//read the first word of the string, use the >> operator (istream::operator>>) 
				file >> sCommand;
				//use conditional statements to process the different commands	
				if (sCommand == "#")
				{
					// Ignore Comment
				}
				else if (sCommand == "v")
				{
					//Vertex
					float x, y, z;
					file >> x >> y >> z;
					positions.push_back({ x, y, z });
				}
				else if (sCommand == "f")
				{
					float i0, i1, i2;
					file >> i0 >> i1 >> i2;

					indices.push_back((int)i0 - 1);
					indices.push_back((int)i1 - 1);
					indices.push_back((int)i2 - 1);
				}
				//read till end of line and ignore all remaining chars
				file.ignore(1000, '\n');

				if (file.eof())
					break;
			}

			//Precompute normals
			for (uint64_t index = 0; index < indices.size(); index += 3)
			{
				uint32_t i0 = indices[index];
				uint32_t i1 = indices[index + 1];
				uint32_t i2 = indices[index + 2];

				Vector3 edgeV0V1 = positions[i1] - positions[i0];
				Vector3 edgeV0V2 = positions[i2] - positions[i0];
				Vector3 normal = Vector3::Cross(edgeV0V1, edgeV0V2);

				if (std::isnan(normal.x))
				{
					int k = 0;
				}

				normal.Normalize();
				if (std::isnan(normal.x))
				{
					int k = 0;
				}

				normals.push_back(normal);
			}

			return true;
		}
#pragma warning(pop)
	}
}