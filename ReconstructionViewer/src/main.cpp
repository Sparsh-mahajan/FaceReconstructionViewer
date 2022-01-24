#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <malloc.h>
#include "Vendor/stb_image/stb_image.h"
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/gtc/type_ptr.hpp"
#include "Includes/VertexBuffer.h"
#include "Includes/IndexBuffer.h"
#include "Includes/VertexArray.h"
#include "Includes/VertexBufferLayout.h"
#include "Includes/Shader.h"
#include "Includes/LoadPoints.h"
#include "Includes/Texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double Xpos, double Ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastFrame = 0.0f;
float deltaTime = 0.0f;

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;



int main()
{   
    Points Cloud("Res/PointCloud/Points.txt");
    Cloud.Load(PointType::Cloud);
    Points cameraFrustum("Res/PointCloud/frustum.txt");
    cameraFrustum.Load(PointType::Frustum);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

   
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float frustumVertices[] = {
        // positions            // texture Coordinates
         0.5f,  0.5f, 0.0f,      1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,      1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,      0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,      0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

    glm::vec3 frustumPositions[] = {
        //add translation vectors for all camera frustums here.
        glm::vec3(-0.639768,-0.00425937,-0.0355112),
        glm::vec3(-0.267663,-0.00128758,-0.0993487),
        glm::vec3(0.0114874,-0.00175259, 0.0102903),
        glm::vec3(-1.04132 ,-0.010242  , 0.00996433)
    };
    glm::vec3 frustumRotations[] = {
		glm::vec3(-0.000964061, -0.136274, -0.00401575),
		glm::vec3(0.000356492, -0.0603128, -0.002191),
		glm::vec3(-0.000550294, 0.00500292, -0.000902102),
		glm::vec3(-0.00237686, -0.15328, -0.00354582)
    };

    std::string path = "Res/Shader/Basic.shader";
    std::string path2 = "Res/Shader/Try.shader";

    VertexBuffer frustumBuffer(frustumVertices, sizeof(frustumVertices));
    IndexBuffer frustumIndex(indices, 6);
    VertexArray ArrayObject;
    vertexBufferLayout layout;

    Shader program(path);

    frustumBuffer.Bind();
    frustumIndex.Bind();
    ArrayObject.Bind();

    layout.Push(GL_FLOAT, 3);
    layout.Push(GL_FLOAT, 2);

    ArrayObject.AddBuffer(frustumBuffer, layout);
    ArrayObject.UnBind();

	VertexBuffer PointsBuffer(Cloud.cloudBuffer, sizeof(Cloud.cloudBuffer));
	vertexBufferLayout layout2;
    VertexArray ArrayObject2;

    Shader program2(path2);

    PointsBuffer.Bind();
    ArrayObject2.Bind();

    layout2.Push(GL_FLOAT, 3);

    ArrayObject2.AddBuffer(PointsBuffer, layout2);
    ArrayObject2.UnBind();

    Texture Tex1("Res/Textures/View1.png");
    Texture Tex2("Res/Textures/View2.png");
    Texture Tex3("Res/Textures/View3.png");
    Texture Tex4("Res/Textures/View4.png");
 
    Texture Arr[4] = { Tex1, Tex2, Tex3, Tex4 };

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
   
	glm::mat4 transformMatrix2 = glm::mat4(1.0f);
	transformMatrix2 = glm::translate(transformMatrix2,glm::vec3(3.0f, 0.0f, 0.5f));
    transformMatrix2 = glm::rotate(transformMatrix2,glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMatrix2 = glm::rotate(transformMatrix2, glm::radians(-110.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    transformMatrix2 = glm::scale(transformMatrix2, glm::vec3(5.0f, 5.0f, 5.0f));

 
    while (!glfwWindowShouldClose(window))
    {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
   
        processInput(window);

       // glDepthMask(true);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
       
        program.Bind();

        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //viewMatrix = glm::rotate(viewMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        program.SetUniformMatrix4fv("View", viewMatrix);
        program.SetUniformMatrix4fv("Projection", projectionMatrix);

        ArrayObject.Bind();
        frustumIndex.Bind();

        for (unsigned int i = 0; i < 4; i++)
        {
            Arr[i].Bind();
            glm::mat4 transformMatrix = glm::mat4(1.0f);
            transformMatrix = glm::translate(transformMatrix, frustumPositions[i] * 2.5f);
			transformMatrix = glm::rotate(transformMatrix, frustumRotations[i].x, glm::vec3(1.0f, 0.0f, 0.0f));
			transformMatrix = glm::rotate(transformMatrix, frustumRotations[i].y, glm::vec3(0.0f, 1.0f, 0.0f));
			transformMatrix = glm::rotate(transformMatrix, frustumRotations[i].z, glm::vec3(0.0f, 0.0f, 1.0f));
            program.SetUniformMatrix4fv("Transform", transformMatrix);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            Arr[i].UnBind();
        }
        
        glPointSize(2.25f);

        ArrayObject.UnBind();
        program.UnBind();

        ArrayObject2.Bind();
        program2.Bind();
        program2.SetUniformMatrix4fv("View", viewMatrix);
        program2.SetUniformMatrix4fv("Projection", projectionMatrix);
        program2.SetUniformMatrix4fv("Transform",transformMatrix2);
        program2.SetUniform4f("PointsColor", 0.0f, 1.0f, 0.0f, 1.0f);

        glDrawArrays(GL_POINTS, 0, 228);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;

}

void processInput(GLFWwindow* window)
{
    const float cameraSpeed = 1.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}