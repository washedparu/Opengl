#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Abstracted
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "BufferLayout.h"
#include "Texture.h"

// Maths
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// TODO:
// 1-/ Rendering multiple objects - WORKING ON
// 2-/ Test framework - 
// 3-/ Batch rendering -
// 4-/ Getting to 3D - 


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
		 -50.0f,  -50.0f,		0.0f, 0.0f, // 0
		  50.0f,   -50.0f,		1.0f, 0.0f, // 1
		  50.0f,   50.0f,		1.0f, 1.0f, // 2
		 -50.0f,  50.0f,		0.0f, 1.0f  // 3
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
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));


	// Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	// Creating and using the shader
	Shader shader("res/shaders/shader.vert", "res/shaders/shader.frag");
	shader.Bind();

	// Creating and loading the texture
	Texture texture("res/textures/paru.png");
	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0);

	// Creating the renderer
	Renderer renderer;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
		glViewport(0, 0, width, height);
	});
	
	// Creating ImGui context
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 330");

	float time = glfwGetTime();
	bool imGui_isOpen = true;

	glm::vec3 imGui_TranslationS(200.0f, 400.0f, 0.0f);
	glm::vec3 imGui_TranslationT(300.0f, 400.0f, 0.0f);

	while (!glfwWindowShouldClose(window)) {
		renderer.Clear();

		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();


		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), imGui_TranslationT);
			glm::mat4 mvp = proj * view * model;
			shader.Bind();	
			shader.SetUniformMat4("u_MVP", mvp);
			renderer.DrawElements(vao, ibo, shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), imGui_TranslationS);
			glm::mat4 mvp = proj * view * model;
			shader.Bind();
			shader.SetUniformMat4("u_MVP", mvp);
			renderer.DrawElements(vao, ibo, shader);
		}


		{
			ImGui::NewFrame();
			ImGui::Begin("Translation", &imGui_isOpen);

			ImGui::Text("Frame rate: %.1f\n\n", ImGui::GetIO().Framerate);

			ImGui::DragFloat2("Translation S", &imGui_TranslationS.x, 1.0f, 0.0f, static_cast<float>(screenWidth));
			ImGui::DragFloat2("Translation T", &imGui_TranslationT.x, 1.0f, 0.0f, static_cast<float>(screenWidth));
			ImGui::End();
		}


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}