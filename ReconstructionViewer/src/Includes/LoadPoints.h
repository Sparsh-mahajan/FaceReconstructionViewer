#pragma once

#include <iostream>
#include <glad/glad.h>
#include "Vendor/glm/glm.hpp"
#include <vector>
#include <fstream>

enum class PointType
{
	Cloud = 0, Frustum = 1
};

class Points
{
private:
	const char* m_FilePath;
public:
	glm::vec3 cloudBuffer[228];
	glm::vec3 frustumBuffer[4];
	glm::vec3* bufferType[2] = { cloudBuffer, frustumBuffer };

	Points(const char* filePath);
	~Points();
	void Load(PointType type);
	void ReadBuffer(PointType type);
};