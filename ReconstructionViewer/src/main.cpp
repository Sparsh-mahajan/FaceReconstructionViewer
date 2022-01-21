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
#include "Vendor/happly/happly.h"
#include "Includes/VertexBuffer.h"
#include "Includes/IndexBuffer.h"
#include "Includes/VertexArray.h"
#include "Includes/VertexBufferLayout.h"
#include "Includes/Shader.h"

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
    std::vector<double> PCVert;
	std::ifstream stream{ "Res/PointCloud/Points.txt" };
	if (!stream)
		std::cout << "not working!\n";
	double a;
	while (stream)
	{
        int i = 0;
		stream >> a;
        if (a != 0)
        {
            PCVert.push_back(a);
        }
	}
    //double* finalVert = new double[(unsigned int)PCVert.size()];
    for (int i = 0; i < PCVert.size();i++) {
        std::cout << PCVert[i] << ", ";
     }
    
    float v[] = {
		0.162381, -0.215219, 3.40892, -0.584492, -0.18297, 3.49842, -0.260736, 0.714722, 3.23932, -0.738563, -0.520498, 3.55827, 0.508413, -0.482931, 3.45299, -0.108421, 0.564063, 3.19199, -0.382094, -0.532074, 3.30907, -0.172486, 0.642459, 3.22809, 0.360405, 0.0776866, 3.34621, -0.326023, 0.59587, 3.28722, -0.0772682, -0.86315, 3.36955, -0.414307, -1.053, 3.57152, 0.365038, -0.340657, 3.37756, -0.302048, 0.698924, 3.26922, -0.235964, 0.115757, 2.94056, -0.462699, 0.246456, 3.40543, -0.0336785, 0.653416, 3.23005, 0.0143371, 0.614886, 3.27062, 0.0796181, -0.562328, 3.2633, -0.248011, 0.583732, 3.23862, -0.221789, -0.187972, 3.18903, -0.0178151, -0.656574, 3.29958, -0.0602923, 0.410513, 3.20357, 0.0267314, -0.562631, 3.2606, -0.204933, 0.656956, 3.22559, -0.581297, -0.552568, 3.43, -0.772151, 0.0278881, 3.58307, -0.250738, 0.603274, 3.24538, -0.222846, 0.654686, 3.23564, -0.227578, 0.695212, 3.2263, -0.0389497, 0.613439, 3.24685, -0.0233642, 0.206564, 3.10685, 0.0668631, -0.5567, 3.26118, 0.170671, -0.507409, 3.30472, 0.344435, -0.354874, 3.3845, -0.179085, 0.607228, 3.2505, -0.256972, 0.655758, 3.24131, 0.328694, -0.225101, 3.39776, -0.704697, -0.583838, 3.52704, 0.415148, -0.482125, 3.36874, -0.366272, -0.534363, 3.30379, -0.265642, -0.438584, 3.27305, -0.589515, -0.542744, 3.42908, -0.58083, -0.537816, 3.41884, -0.0547413, 0.290126, 3.17937, 0.0702599, 0.627598, 3.32449, 0.00723934, -0.524057, 3.25088, -0.345383, 0.210879, 3.15787, -0.22916, 0.571505, 3.21912, -0.435635, -0.603397, 3.34237, -0.187482, 0.433476, 3.20093, 0.0879449, 0.634835, 3.31752, -0.638807, -0.541546, 3.46399, -0.0124696, -0.463764, 3.26039, -0.316383, -0.513846, 3.28841, 0.314956, -0.364352, 3.37119, -0.659261, -0.568646, 3.49342, -0.506191, -0.515669, 3.39369, -0.320474, 0.611589, 3.30946, -0.111819, 0.668795, 3.22393, -0.617675, -0.227167, 3.5163, 0.11565, -0.473142, 3.30614, -0.112874, 0.169922, 2.99506, -0.0225778, -0.733481, 3.31496, 0.423506, -0.508422, 3.39678, -0.0770216, 0.946843, 3.29432, -0.284252, 0.650671, 3.23844, 0.379688, -0.449641, 3.36715, -0.538063, 0.0728687, 3.4437, -0.312493, -0.655753, 3.33355, -0.0825547, 0.683719, 3.21572, -0.0432278, -0.421878, 3.25671, 0.140377, 0.924074, 3.40711, -0.0751895, 0.565764, 3.20618, -0.278442, -0.395841, 3.28538, -0.516651, -0.21188, 3.48045, -0.470318, -0.532013, 3.36812, -0.713453, -0.303176, 3.59571, 0.382597, -0.336942, 3.40755, -0.615378, -0.524806, 3.44499, -0.0133519, -0.204072, 3.38895, -0.168371, 0.574701, 3.22284, 0.0198435, -0.511079, 3.25554, 0.341399, -0.183094, 3.41274, 0.332491, -0.368778, 3.38184, 0.423982, -0.169483, 3.45674, 0.324015, -0.162481, 3.35194, -0.142205, -0.0113694, 3.04873, 0.00217131, 0.612206, 3.27226, 0.0926205, 0.170999, 3.30546, -0.202801, 0.584327, 3.22297, -0.323556, 0.618504, 3.29291, -0.662492, -0.546876, 3.48233, 0.186167, 0.128329, 3.32757, -0.349741, -0.609044, 3.32981, -0.494826, 0.377988, 3.43011, 0.159955, 0.732234, 3.37647, 0.361493, 0.141162, 3.35176, -0.597381, -0.029024, 3.49613, 0.183689, -0.539515, 3.27639, -0.56343, 0.210679, 3.43065, 0.44381, -0.489142, 3.40543, -0.332871, 0.629561, 3.30241, -0.360507, -0.543752, 3.30494, -0.416639, -0.947978, 3.49207, -0.541093, -0.558447, 3.39811, -0.344092, 0.45281, 3.26879, -0.349615, -0.178649, 3.45431, 0.00185455, -0.556497, 3.25939, -0.410147, 0.50097, 3.31997, -0.302978, 0.640845, 3.26189, 0.302476, -0.368239, 3.37524, -0.358869, 0.569626, 3.2815, 0.382703, -0.514326, 3.36268, -0.73002, 0.0390779, 3.54386, -0.535073, -0.542554, 3.40141, -0.0825134, 0.873655, 3.31042, -0.263354, 0.586967, 3.24466, -0.346355, 0.0131791, 3.33809, -0.222281, 0.586475, 3.23588, -0.307317, -0.515255, 3.28476, -0.259388, 0.569917, 3.22708, -0.276977, -0.444841, 3.27758, -0.289873, -0.481603, 3.2784, -0.190534, 0.631477, 3.23608, -0.491054, -0.0749516, 3.48843, 0.365595, -0.354282, 3.39493, -0.219619, 0.684382, 3.22506, -0.631262, -0.0766582, 3.55206, 0.542886, 0.124799, 3.47915, -0.240655, 0.678005, 3.23326, -0.735895, -0.505351, 3.55992, 0.0123563, 0.659009, 3.25875, 0.0515351, -0.602859, 3.2691, 0.0246344, -0.55373, 3.25899, -0.36716, 0.624196, 3.34914, 0.46616, -1.12033, 3.73568, -0.254691, 0.163932, 2.95567, -0.669196, -0.56567, 3.48764, -0.303031, 0.581476, 3.26466, -0.0677773, 0.650245, 3.2227, -0.613991, -0.553465, 3.45969, -0.720506, -0.598344, 3.53806, -0.0985023, -0.78586, 3.33361, 0.39161, -0.340651, 3.40757, -0.79174, 0.0479892, 3.61156, -0.54881, -0.611786, 3.41104, -0.655538, -0.225043, 3.5396, -0.082071, 0.64435, 3.22082, 0.381849, -0.503711, 3.36161, 0.310074, -0.303418, 3.38125, 0.0432167, -0.725635, 3.32886, -0.714774, -0.391518, 3.54478, -0.64762, 0.157726, 3.46942, -0.222214, 0.668752, 3.22843, -0.584405, -0.560714, 3.42677, -0.292249, 0.63588, 3.26173, 0.124754, -0.511643, 3.2746, -0.773422, -0.118472, 3.66188, -0.189041, 0.857636, 3.3228, -0.570285, -0.554142, 3.42016, 0.144662, -0.579888, 3.25918, -0.448794, 0.779814, 3.43126, 0.145163, -0.172569, 3.40352, -0.616712, -0.489456, 3.4508, 0.410777, -0.317972, 3.42861, -0.346111, -0.51358, 3.30021, -0.434491, -0.281029, 3.48561, -0.699468, -0.078372, 3.59135, -0.0639286, -0.477613, 3.24704, -0.585745, -0.33809, 3.42706, 0.0194372, -0.480521, 3.25148, -0.331647, 0.719223, 3.30731, -0.233981, -0.489409, 3.26268, -0.540311, 0.000142077, 3.47009, -0.194417, 0.00555815, 3.02593, 0.0324003, -0.485604, 3.25084, -0.335093, 0.614694, 3.31627, 0.159778, 0.00230161, 3.36773, -0.3908, -0.280727, 3.47888, -0.43875, -0.188555, 3.48207, -0.325909, 0.638424, 3.29148, -0.326807, -0.527993, 3.29012, 0.0863834, 0.864585, 3.3577, -0.288283, 1.02305, 3.31865, -0.244637, 0.654488, 3.23531, -0.774032, 0.0421188, 3.57769, 0.275216, -0.157819, 3.34754, -0.0479539, 0.296748, 3.18508, -0.0309009, -0.53691, 3.25182, -0.555178, -0.547128, 3.40155, 0.530013, -0.427073, 3.47199, 0.422993, -0.336699, 3.42399, 0.299099, -0.374562, 3.36126, 0.48459, -0.473992, 3.4245, -0.496035, 0.292826, 3.41003, -0.284334, -0.215643, 3.42172, -0.761515, 0.0518814, 3.57463, 0.0663597, 0.564822, 3.26942, -0.261404, 0.054317, 3.02771, -0.0678669, 0.0701902, 3.11375, -0.62547, 0.12399, 3.4514, -0.602111, 0.244834, 3.45802, 0.291458, -0.413493, 3.33793, -0.280471, -0.475292, 3.27394, -0.0556215, -0.454659, 3.24008, -0.186783, 0.584698, 3.22693, -0.0459855, 0.654681, 3.22916, -0.170149, 0.782687, 3.26531, -0.19511, -0.443542, 3.25482, -0.279121, 0.626921, 3.26106, -0.162857, 0.864666, 3.32276, -0.220446, 0.615935, 3.24939, 0.0732031, 0.705919, 3.28981, -0.296545, -0.522202, 3.27955, -0.406319, 0.636923, 3.37851, -0.524157, -0.347535, 3.44255, -0.323543, 0.617136, 3.28932, 0.0231124, 0.941553, 3.32996, -0.0736502, 0.656828, 3.21775, -0.358422, 0.719564, 3.3324, 0.581965, -0.430502, 3.5475, 0.0258823, 0.730034, 3.27889, -0.647196, -0.193132, 3.55361, 0.137316, -0.963556, 3.4308, -0.353466, -0.531685, 3.29709, 0.217108, 0.676678, 3.40262, -0.403905, -0.523358, 3.32283,
	};

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfw window creation
    // --------------------
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    //glEnable(GL_POINT_SMOOTH_HINT);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float frustumVertices[] = {
        // positions            // texture coords
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
        glm::vec3(-3.0f, 0.0f, 0.5f),
        glm::vec3(-1.0f, 0.0f, 0.5f),
        glm::vec3( 1.0f, 0.0f, 0.5f),
        glm::vec3( 3.0f, 0.0f, 0.5f)
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

	VertexBuffer PointsBuffer(v,684);
	vertexBufferLayout layout2;
    VertexArray ArrayObject2;

    Shader program2(path2);

    PointsBuffer.Bind();
    ArrayObject2.Bind();

    layout2.Push(GL_FLOAT, 3);

    ArrayObject2.AddBuffer(PointsBuffer, layout2);
    ArrayObject2.UnBind();


    stbi_set_flip_vertically_on_load(true);
    unsigned int Texture1; 
    glGenTextures(1, &Texture1);
    glBindTexture(GL_TEXTURE_2D, Texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("Res/Textures/View3.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load Texture! \n";
    }
    stbi_image_free(data);

   
   // std::cout << Source.VertexSource << std::endl;
   // std::cout<<Source.FragmentSource<<std::endl;
    
   // transformMatrix = glm::scale(transformMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    ////unsigned int transformLoc = glGetUniformLocation(program.GetProgramID(), "Transform");
    //unsigned int modelLoc = glGetUniformLocation(programID, "Model");
    ////unsigned int viewLoc = glGetUniformLocation(program.GetProgramID(), "View");
    ////unsigned int projectionLoc = glGetUniformLocation(program.GetProgramID(), "Projection");

	glm::mat4 transformMatrix2 = glm::mat4(1.0f);
	transformMatrix2 = glm::translate(transformMatrix2,glm::vec3(-3.0f, 0.0f, 0.5f));
    transformMatrix2 = glm::scale(transformMatrix2, glm::vec3(5.0f, 5.0f, 5.0f));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
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
        viewMatrix = glm::rotate(view, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        ////glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        ////glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        program.SetUniformMatrix4fv("View", viewMatrix);
        program.SetUniformMatrix4fv("Projection", projectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture1);
        ArrayObject.Bind();
        frustumIndex.Bind();

        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 transformMatrix = glm::mat4(1.0f);
            transformMatrix = glm::translate(transformMatrix, frustumPositions[i]);
            //float angle = 20.0f * i;
            //transformMatrix = glm::rotate(transformMatrix, (float)glfwGetTime() + glm::radians(angle),
               // glm::vec3(1.0f, 0.3f, 0.5f));
            //transformMatrix = glm::scale(transformMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            ////glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMatrix));
            program.SetUniformMatrix4fv("Transform", transformMatrix);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }
         // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        ArrayObject.UnBind();
        program.UnBind();

        ArrayObject2.Bind();
        program2.Bind();
        program2.SetUniformMatrix4fv("View", viewMatrix);
        program2.SetUniformMatrix4fv("Projection", projectionMatrix);
        program2.SetUniformMatrix4fv("Transform",transformMatrix2);
        program2.SetUniform4f("PointsColor", 0.0f, 1.0f, 0.0f, 1.0f);

        glDrawArrays(GL_POINTS, 0, 227);

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