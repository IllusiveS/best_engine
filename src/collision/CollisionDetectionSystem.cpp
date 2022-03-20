#include "CollisionDetectionSystem.h"\

#include "BoxCollisionComponent.h"
#include "components/Transform.h"
#include "components/Camera.h"


Ray CollisionDetectionSystem::generateRayFromCamera(flecs::world& world, glm::vec2 screenPos)
{
	Ray returnRay;
	static auto cameraQuery = world.query_builder<const Transform, const Camera>().build();

	glm::vec2 _windowExtent{ 1700 , 900 };

	cameraQuery.each([&](const Transform& trans, const Camera& cam) {
		float x = (2.0f * screenPos.x) / _windowExtent.x - 1.0f;
		float y = 1.0f - (2.0f * screenPos.y) / _windowExtent.y;
		float z = 1.0f;

		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
		glm::vec4 ray_eye = glm::inverse(cam.projection) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

		glm::vec3 ray_wor = (glm::inverse(cam.view) * ray_eye);
		ray_wor = glm::normalize(ray_wor);

		returnRay.direction = ray_wor;
		returnRay.origin = trans.globalPosition;
	});

	

	return returnRay;
}



flecs::entity CollisionDetectionSystem::raycastSingleBox(flecs::world& world, const Ray& ray)
{
#define NUMDIM	3
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

	static auto collidersQuery = world.query_builder<const Transform, const BoxCollisionComponent>()
		.build();

	flecs::entity found_ent = flecs::entity::null();

	collidersQuery.iter(
		[ray, &found_ent](flecs::iter& iter, const Transform* trans, const BoxCollisionComponent* box)
		{
			float tmin, tmax, tymin, tymax, tzmin, tzmax;

			tmin = (bounds[r.sign[0]].x - r.orig.x) * r.invdir.x;
			tmax = (bounds[1 - r.sign[0]].x - r.orig.x) * r.invdir.x;
			tymin = (bounds[r.sign[1]].y - r.orig.y) * r.invdir.y;
			tymax = (bounds[1 - r.sign[1]].y - r.orig.y) * r.invdir.y;

			if((tmin > tymax) || (tymin > tmax))
				return false;
			if(tymin > tmin)
				tmin = tymin;
			if(tymax < tmax)
				tmax = tymax;

			tzmin = (bounds[r.sign[2]].z - r.orig.z) * r.invdir.z;
			tzmax = (bounds[1 - r.sign[2]].z - r.orig.z) * r.invdir.z;

			if((tmin > tzmax) || (tzmin > tmax))
				return false;
			if(tzmin > tmin)
				tmin = tzmin;
			if(tzmax < tmax)
				tmax = tzmax;

			return true;
		});
	return found_ent;
}

flecs::entity CollisionDetectionSystem::raycastSingleHit(flecs::world& world, const Ray& ray)
{
	static auto collidersQuery = world.query_builder<const Transform, const BoxCollisionComponent>()
		.build();
	
	flecs::entity found_ent = flecs::entity::null();

	collidersQuery.iter([ray, &found_ent](flecs::iter& iter, const Transform* trans, const BoxCollisionComponent* box) {

		if(iter.count() == 0) return;

		float smallestDistanceFound = FLT_MAX;

		float tMin = FLT_MAX;
		float tMax = 0.0f;

		for(const auto& it : iter)
		{
			glm::vec3 OBBposition_worldspace(trans[it].globalPosition.x, trans[it].globalPosition.y, trans[it].globalPosition.z);

			glm::vec3 delta = OBBposition_worldspace - ray.origin;

			{
				glm::vec3 xaxis(trans[it].transform[0].x, trans[it].transform[0].y, trans[it].transform[0].z);
				float e = glm::dot(xaxis, delta);
				float f = glm::dot(ray.direction, xaxis);

				// Beware, don't do the division if f is near 0 ! See full source code for details.
				float t1 = (e + box[it].min.x) / f; // Intersection with the "left" plane
				float t2 = (e + box[it].max.x) / f; // Intersection with the "right" plane

				if(t1 > t2)
				{ // if wrong order
					float w = t1; t1 = t2; t2 = w; // swap t1 and t2
				}

				// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
				if(t2 < tMax) tMax = t2;
				// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
				if(t1 > tMin) tMin = t1;

				if(tMax < tMin)
					continue;
			}
			
			{
				glm::vec3 xaxis(trans[it].transform[1].x, trans[it].transform[1].y, trans[it].transform[1].z);
				float e = glm::dot(xaxis, delta);
				float f = glm::dot(ray.direction, xaxis);

				// Beware, don't do the division if f is near 0 ! See full source code for details.
				float t1 = (e + box[it].min.x) / f; // Intersection with the "left" plane
				float t2 = (e + box[it].max.x) / f; // Intersection with the "right" plane

				if(t1 > t2)
				{ // if wrong order
					float w = t1; t1 = t2; t2 = w; // swap t1 and t2
				}

				// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
				if(t2 < tMax) tMax = t2;
				// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
				if(t1 > tMin) tMin = t1;

				if(tMax < tMin)
					continue;
			}

			{
				glm::vec3 xaxis(trans[it].transform[2].x, trans[it].transform[2].y, trans[it].transform[2].z);
				float e = glm::dot(xaxis, delta);
				float f = glm::dot(ray.direction, xaxis);

				// Beware, don't do the division if f is near 0 ! See full source code for details.
				float t1 = (e + box[it].min.x) / f; // Intersection with the "left" plane
				float t2 = (e + box[it].max.x) / f; // Intersection with the "right" plane

				if(t1 > t2)
				{ // if wrong order
					float w = t1; t1 = t2; t2 = w; // swap t1 and t2
				}

				// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
				if(t2 < tMax) tMax = t2;
				// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
				if(t1 > tMin) tMin = t1;

				if(tMax < tMin)
					continue;
			}
			if(tMin < smallestDistanceFound)
			{
				smallestDistanceFound = tMin;
				found_ent = iter.entity(it);
				return;
			}
		}
	});

	return found_ent;
}