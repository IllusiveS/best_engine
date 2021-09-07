//
// Created by patwys on 5/9/2021.
//

#ifndef BEST_ENGINE_TRANSFORM_H
#define BEST_ENGINE_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Wrenpp.h>

struct Vector3 {
	explicit Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(glm::vec3 v) : vals(v) {};

	union {
		glm::vec3 vals;
		struct {float x, y, z; };
	};

	Vector3 normalize() const {return glm::normalize(vals);};
	float magnitude() const {return glm::length(vals);};
	void add(Vector3 vec_to_add) { vals += vec_to_add.vals;};

	static void bind_methods(wrenpp::VM& vm);
};

struct Transform{
	glm::vec3 globalPosition;
	glm::quat globalRotation;
	glm::vec3 globalScale;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform(glm::vec3 position = glm::vec3{},
		glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0),
		glm::vec3 scale = glm::vec3{ 1 });

	glm::mat4 getModelMatrix() const
	{
		return transform;
	}

	void updateModelMatrix();

private:
	glm::mat4 transform;
};


#endif //BEST_ENGINE_TRANSFORM_H
