#include "Includes/LoadPoints.h"

Points::Points(const char* filepath) : m_FilePath(filepath)
{}

Points::~Points()
{}

void Points::Load(PointType type)
{
	std::ifstream stream(m_FilePath);
	if (!stream)
	{
		std::cout << "File open did not worked!\n";
		return;
	}
	int i = 0, n = 3, j=0;
	if (type == PointType::Cloud)
	{
		while (stream)
		{
			double a;
			stream >> a;
			if (a != 0)
			{
				i += 1;
				if (i % 3 == 0)
					cloudBuffer[j].x = a;
				else if (i % 3 == 1)
					cloudBuffer[j].y = a;
				else if (i % 3 == 2)
					cloudBuffer[j].z = a;
				if (i == n)
				{
					n += 3;
					j += 1;
				}
			}
		}
	}
	else if (type == PointType::Frustum)
	{
		while (stream)
		{
			double a;
			stream >> a;
			if (a != 0 && a != 255)
			{
				i += 1;
				if (i % 3 == 0)
					frustumBuffer[j].x = a;
				else if (i % 3 == 1)
					frustumBuffer[j].y = a;
				else if (i % 3 == 2)
					frustumBuffer[j].z = a;
				if (i == n)
				{
					n += 3;
					j += 1;
				}
			}
		}
	}
	else
	{
		std::cout << "Type does not matches the provided buffer types!\n";
		return;
	}
}

void Points::ReadBuffer(PointType type)
{
	if (type == PointType::Cloud)
	{
		for (int i = 0; i < 228; i++)
		{
			std::cout << cloudBuffer[i].x << ", " <<
				cloudBuffer[i].y << ", " <<
				cloudBuffer[i].z << "\n";
		}
	}
	else if (type == PointType::Frustum)
	{
		for (int i = 0; i < 4; i++)
		{
			std::cout << frustumBuffer[i].x << ", " <<
				frustumBuffer[i].y << ", " <<
				frustumBuffer[i].z << "\n";
		}
	}
}
