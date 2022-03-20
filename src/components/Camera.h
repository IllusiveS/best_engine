//
// Created by patwys on 5/9/2021.
//

#ifndef BEST_ENGINE_CAMERA_H
#define BEST_ENGINE_CAMERA_H

#include <array>

#include <glm/mat4x4.hpp>

struct Camera{
	enum class CamType
	{
		ORTOGRAPHIC,
		PERSPECTIVE
	};

	glm::mat4 view;
	glm::mat4 projection;
	CamType cameraType;
	float FOV{70.0f};
	std::array<glm::vec3, 4> extends;


	void setFov(const float fov);
	void updateExtends();
};


#endif //BEST_ENGINE_CAMERA_H
