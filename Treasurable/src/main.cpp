#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "BufferLayout.h"
#include "Texture.h"
#include "Cube.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


// TODO:
// 1-/ Rendering multiple objects - WORKING ON
// 2-/ Test framework - 
// 3-/ Batch rendering -
// 4-/ Getting to 3D -  (Now done: rendering a cube)

int main() {
	GLFWwindow* window;
	int screenWidth = 1000, screenHeight = 800;

	if (!glfwInit()) {
		std::cout << "Couldn't initialize GLFW!\n";
		return -1;
	}
	std::cout << "Initialized GLFW!\n";

	window = glfwCreateWindow(screenWidth, screenHeight, "Treasurable 3D Cube", nullptr, nullptr);

	if (!window) {
		std::cout << "Couldn't initialize the window!\n";
		return -1;
	}
	std::cout << "Initialized the window!\n";

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Couldn't initialize GLEW!\n";
		return -1;
	}
	std::cout << "Initialized GLEW!\n";

	float vertices[] = {
		// Front face
		-50.0f, -50.0f,  50.0f, 0.0f, 0.0f,
		 50.0f, -50.0f,  50.0f, 1.0f, 0.0f,
		 50.0f,  50.0f,  50.0f, 1.0f, 1.0f,
		-50.0f,  50.0f,  50.0f, 0.0f, 1.0f,

		// Back face
		-50.0f, -50.0f, -50.0f, 0.0f, 0.0f,
		 50.0f, -50.0f, -50.0f, 1.0f, 0.0f,
		 50.0f,  50.0f, -50.0f, 1.0f, 1.0f,
		-50.0f,  50.0f, -50.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Right face
		1, 5, 6, 6, 2, 1,
		// Back face
		5, 4, 7, 7, 6, 5,
		// Left face
		4, 0, 3, 3, 7, 4,
		// Top face
		3, 2, 6, 6, 7, 3,
		// Bottom face
		4, 5, 1, 1, 0, 4
	};

	// Vertex Array object
	VertexArray vao;

	// Vertex Buffer object
	VertexBuffer vbo(vertices, sizeof(vertices));

	// Buffer layout 
	BufferLayout layout;
	layout.push<float>(3); // positions
	layout.push<float>(2); //tex coords

	vao.addBuffer(vbo, layout);

	// Index Buffer object
	IndexBuffer ibo(indices, sizeof(indices) / sizeof(unsigned int));

	// Projection matrix (perspective projection for 3D)
	glm::mat4 proj = glm::perspective(glm::radians(45.0f * 1.5f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 1000.0f);

	// View matrix - move camera back so cube is visible
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -400.0f));

	// Enable depth testing so closer faces obscure farther ones
	glEnable(GL_DEPTH_TEST);

	// Enable blending for transparency if needed
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	// Creating and using the shader
	Shader shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
	shader.Bind();

	// Creating and loading the texture
	Texture texture("res/textures/img.png");
	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0);

	// Creating the renderer
	Renderer renderer;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});

	// Creating ImGui context
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 330");

	Cube cubeA = { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f };
	Cube cubeB = { glm::vec3(100.0f, 0.0f, 0.0f), 0.0f };


	while (!glfwWindowShouldClose(window)) {
		renderer.Clear();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();

		cubeA.Draw(proj, view, shader, renderer, vao, ibo);
		cubeB.Draw(proj, view, shader, renderer, vao, ibo);


		// ImGui UI
		ImGui::NewFrame();
		ImGui::Begin("Cube Controls");
		ImGui::Text("Cube A");
		ImGui::SliderFloat("Rotation##CubeA", &cubeA.GetAngle(), 0.0f, 360.0f);
		ImGui::SliderFloat3("Position##CubeA", &cubeA.GetPosition().x, -200.0f, 200.0f);

		ImGui::Text("\n\nCube B");
		ImGui::SliderFloat("Rotation##CubeB", &cubeB.GetAngle(), 0.0f, 360.0f);
		ImGui::SliderFloat3("Position##CubeB", &cubeB.GetPosition().x, -200.0f, 200.0f);

		ImGui::Text("Frame rate: %.1f", ImGui::GetIO().Framerate);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup ImGui and GLFW
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}