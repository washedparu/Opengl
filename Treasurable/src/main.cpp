#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "BufferLayout.h"
#include "Texture.h"

// TODO:
// 1-/ Abstract away te code into classes - DONE
// 2-/ Learn about Textures - WORKING ON (On blending)
// Shader path - DONE


int main() {
	GLFWwindow* window;
	if (!glfwInit()) {
		std::cout << "Couldn't initialize GLFW!\n";
		return -1;
	}
	std::cout << "Initialized GLFW!\n";

	window = glfwCreateWindow(1000, 800, "Treasurable", nullptr, nullptr);

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


	float vertices[16] = {
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f
	};
	unsigned int indices[6] = { 0,1,2,2,3,0 };

	// Vertex Array object
	VertexArray vao;

	// Vertex Buffer object
	VertexBuffer vbo(vertices, 16 * sizeof(float));

	// Buffer layout
	BufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);

	// Adding buffer to the vertex array
	vao.addBuffer(vbo, layout);

	// Index Buffer object
	IndexBuffer ibo(indices, 6);

	// Creating and using the shader
	Shader shader("res/shaders/shader.vert", "res/shaders/shader.frag");
	shader.Bind();

	// Loading texture
	Texture texture = {"res/textures/paru.png"};
	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// Creating the renderer
	Renderer renderer;
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
		glViewport(0, 0, width, height);
	});

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
		renderer.DrawElements(vao, ibo, shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}