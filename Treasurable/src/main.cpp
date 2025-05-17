#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "BufferLayout.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



// TODO:
// 1-/ Learn about math in opengl - DONE
// 2-/ More math (matrix and model view projection) - DONE
// 3-/ Dear Imgui with opengl - (..) - SOON

// I don't know where should i put this.


int main() {
	GLFWwindow* window;
	int screenWidth = 1000, screenHeight = 800;
	if (!glfwInit()) {
		std::cout << "Couldn't initialize GLFW!\n";
		return -1;
	}
	std::cout << "Initialized GLFW!\n";

	window = glfwCreateWindow(screenWidth, screenHeight, "Treasurable", nullptr, nullptr);

	if (!window) {
		std::cout << "Couldn't initialize the window!\n";
	}
	std::cout << "initialized the window!\n";


	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Couldn't initialize GLEW!\n";
		return -1;
	}
	std::cout << "initialized GLEW!\n";


	float vertices[] = {
		 100.0f,  100.0f,		0.0f, 0.0f, // 0
		 400.0f,  100.0f,		1.0f, 0.0f, // 1
		 400.0f,  300.0f,		1.0f, 1.0f, // 2
		 100.0f,  400.0f,		0.0f, 1.0f  // 3
	};

	unsigned int indices[6] = { 
		0, 1, 2, 
		2, 3, 0
	};

	// Vertex Array object
	VertexArray vao;

	// Vertex Buffer object
	VertexBuffer vbo(vertices, 16 * sizeof(float));

	// Buffer layout
	BufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
			
	// Passing buffer to the vertex array
	vao.addBuffer(vbo, layout);

	// Index Buffer object
	IndexBuffer ibo(indices, 6);

	// Projection matrix (orthographic projection)
	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight), 1.0f, -1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, .0f, 0.0f));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 500.0f, 0.0f));

	glm::mat4 mvp = proj * view * model;

	// Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	// Creating and using the shader
	Shader shader("res/shaders/shader.vert", "res/shaders/shader.frag");
	shader.Bind();
	shader.SetUniformMat4("u_MVP", mvp);


	// Creating and loading the texture
	Texture texture("res/textures/img.png");
	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0);

	// Creating the renderer
	Renderer renderer;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
		glViewport(0, 0, width, height);
	});
	float time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

		shader.SetUniform2f("u_Offset", std::sin(time) * 0.1f, 0.0f);
		renderer.DrawElements(vao, ibo, shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}