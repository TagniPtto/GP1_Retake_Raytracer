#pragma once
#include <stdexcept>
#include <vector>
#include "MathUtils/Math.h"
#include "MathUtils/Vector3.h"


namespace dae
{
#pragma region GEOMETRY
	struct Sphere final
	{
		Vector3 origin{};
		float radius{};

		unsigned char materialIndex{ 0 };
	};

	struct Plane final
	{
		Vector3 origin{};
		Vector3 normal{};

		unsigned char materialIndex{ 0 };
	};

	enum class TriangleCullMode
	{
		FrontFaceCulling,
		BackFaceCulling,
		NoCulling
	};

	struct Triangle final
	{
		Triangle() = default;
		Triangle(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2, const Vector3& _normal) :
			v0{ _v0 }, v1{ _v1 }, v2{ _v2 }, normal{ _normal.Normalized() } {}

		Triangle(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2) :
			v0{ _v0 }, v1{ _v1 }, v2{ _v2 }
		{
			const Vector3 edgeV0V1 = v1 - v0;
			const Vector3 edgeV0V2 = v2 - v0;
			normal = Vector3::Cross(edgeV0V1, edgeV0V2).Normalized();
		}

		Vector3 v0{};
		Vector3 v1{};
		Vector3 v2{};

		Vector3 normal{};

		TriangleCullMode cullMode{};
		unsigned char materialIndex{};
	};

	struct TriangleMesh final
	{
		TriangleMesh() = default;
		TriangleMesh(const std::vector<Vector3>& _positions, const std::vector<int>& _indices, TriangleCullMode _cullMode) :
			positions(_positions), indices(_indices), cullMode(_cullMode)
		{
			//Calculate Normals
			CalculateNormals();

			//Update Transforms
			UpdateTransforms();
		}

		TriangleMesh(const std::vector<Vector3>& _positions, const std::vector<int>& _indices, const std::vector<Vector3>& _normals, TriangleCullMode _cullMode) :
			positions(_positions), indices(_indices), normals(_normals), cullMode(_cullMode)
		{
			UpdateTransforms();
		}

		std::vector<Vector3> positions{};
		std::vector<Vector3> normals{};
		std::vector<int> indices{};
		unsigned char materialIndex{};

		TriangleCullMode cullMode{ TriangleCullMode::BackFaceCulling };

		Matrix rotationTransform{};
		Matrix translationTransform{};
		Matrix scaleTransform{};

		std::vector<Vector3> transformedPositions{};
		std::vector<Vector3> transformedNormals{};
		Vector3 boundingBox_AABB_Min{};
		Vector3 boundingBox_AABB_Max{};

		Vector3 transformedBoundingBox_AABB_Min{};
		Vector3 transformedBoundingBox_AABB_Max{};

		void UpdateAABB() {
			if (positions.size() < 1)return;
			Vector3 min{ positions[0] };
			Vector3 max{ positions[0] };

			for (int i{}; i < positions.size(); i++) {
				min = Vector3::Min(min, positions[i]);
				max = Vector3::Max(max, positions[i]);
			}
			boundingBox_AABB_Max = max;
			boundingBox_AABB_Min = min;
		}
		void UpdateTransformedAABB(const Matrix& finalTransform) {
			Vector3 tMaxAAB = finalTransform.TransformPoint(boundingBox_AABB_Min);
			Vector3 tMinAAB = tMaxAAB;


			Vector3 tAABB{ finalTransform.TransformPoint(boundingBox_AABB_Max.x,boundingBox_AABB_Min.y,boundingBox_AABB_Min.z) };
			tMinAAB = Vector3::Min(tMinAAB, tAABB);
			tMaxAAB = Vector3::Max(tMaxAAB, tAABB);

			tAABB = finalTransform.TransformPoint(boundingBox_AABB_Max.x, boundingBox_AABB_Min.y, boundingBox_AABB_Max.z);
			tMinAAB = Vector3::Min(tMinAAB, tAABB);
			tMaxAAB = Vector3::Max(tMaxAAB, tAABB);

			tAABB = finalTransform.TransformPoint(boundingBox_AABB_Min.x, boundingBox_AABB_Min.y, boundingBox_AABB_Max.z);
			tMinAAB = Vector3::Min(tMinAAB, tAABB);
			tMaxAAB = Vector3::Max(tMaxAAB, tAABB);


			tAABB = finalTransform.TransformPoint(boundingBox_AABB_Min.x, boundingBox_AABB_Max.y, boundingBox_AABB_Min.z);
			tMinAAB = Vector3::Min(tMinAAB, tAABB);
			tMaxAAB = Vector3::Max(tMaxAAB, tAABB);

			tAABB = finalTransform.TransformPoint(boundingBox_AABB_Max.x, boundingBox_AABB_Max.y, boundingBox_AABB_Min.z);
			tMinAAB = Vector3::Min(tMinAAB, tAABB);
			tMaxAAB = Vector3::Max(tMaxAAB, tAABB);


			tAABB = finalTransform.TransformPoint(boundingBox_AABB_Max.x, boundingBox_AABB_Max.y, boundingBox_AABB_Max.z);
			tMinAAB = Vector3::Min(tMinAAB, tAABB);
			tMaxAAB = Vector3::Max(tMaxAAB, tAABB);

			tAABB = finalTransform.TransformPoint(boundingBox_AABB_Min.x, boundingBox_AABB_Max.y, boundingBox_AABB_Max.z);
			tMinAAB = Vector3::Min(tMinAAB, tAABB);
			tMaxAAB = Vector3::Max(tMaxAAB, tAABB);

			transformedBoundingBox_AABB_Max = tMaxAAB;
			transformedBoundingBox_AABB_Min = tMinAAB;
		}
		void Translate(const Vector3& translation)
		{
			translationTransform = Matrix::CreateTranslation(translation);
		}

		void RotateY(float yaw)
		{
			rotationTransform = Matrix::CreateRotationY(yaw);
		}

		void Scale(const Vector3& scale)
		{
			scaleTransform = Matrix::CreateScale(scale);
		}

		void AppendTriangle(const Triangle& triangle, bool ignoreTransformUpdate = false)
		{
			int startIndex = static_cast<int>(positions.size());

			positions.push_back(triangle.v0);
			positions.push_back(triangle.v1);
			positions.push_back(triangle.v2);

			indices.push_back(startIndex);
			indices.push_back(++startIndex);
			indices.push_back(++startIndex);

			normals.push_back(triangle.normal);

			//Not ideal, but making sure all vertices are updated
			if (!ignoreTransformUpdate)
				UpdateTransforms();
		}

		void CalculateNormals()
		{
			for (int i{ 0 }; i < indices.size(); i += 3) {
				const Vector3& v0{ positions[indices[i + 0]] };
				const Vector3& v1{ positions[indices[i + 1]] };
				const Vector3& v2{ positions[indices[i + 2]] };
				Vector3 normal{ Vector3::Cross(v2 - v0,v1 - v0).Normalized() };
				normals.emplace_back(normal);
			}

		}

		void UpdateTransforms()
		{
			Matrix transform{ translationTransform * rotationTransform * scaleTransform };

			transformedPositions.clear();
			transformedNormals.clear();
			for (int i{}; i < positions.size(); i++) {
				transformedPositions.emplace_back(transform.TransformPoint(positions[i]));
			}
			for (int i{}; i < normals.size(); i++) {
				transformedNormals.emplace_back(transform.TransformVector(normals[i]));
			}
			UpdateTransformedAABB(transform);
		}
	};
#pragma endregion
#pragma region LIGHT
	enum class LightType
	{
		Point,
		Directional
	};

	struct Light final
	{
		Vector3 origin{};
		Vector3 direction{};
		ColorRGB color{};
		float intensity{};

		LightType type{};
	};
#pragma endregion
#pragma region MISC
	struct Ray final
	{
		Vector3 origin{};
		Vector3 direction{};

		float min{ 0.0001f };
		float max{ FLT_MAX };
	};

	struct HitRecord final
	{
		Vector3 origin{};
		Vector3 normal{};
		float t = FLT_MAX;

		bool didHit{ false };
		unsigned char materialIndex{ 0 };
	};
#pragma endregion
}