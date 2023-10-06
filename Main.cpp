#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include <vector>

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"

#include<ft2build.h>
#include <map>
#include FT_FREETYPE_H

// Vertex Shader

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"uniform mat4 proj;\n"
"void main()\n"
"{\n"
"    gl_Position = proj * vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"}\0";

// Vertex Shader for text
const char* vertexShaderTextSource = "#version 330 core\n"
"layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex_coord>\n"
"out vec2 TexCoords;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
"    TexCoords = vertex.zw;\n"
"}\0";

//Fragment Shader

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0f, 1.0f, 0f, 1.0f);\n"
"}\n\0";

float func(float x, float a) {
	return sqrt(x * x * (a - x) / (a + x));
}

GLuint shaderProgram;
GLint projlocation;

// Update view when resizing window
void frame_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	float asp = (float)width / float(height);
	glm::mat4 proj = glm::ortho(-asp, asp, -1.0f, 1.0f);
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(projlocation, 1, GL_FALSE, glm::value_ptr(proj));
}


// Param
float a = .0f;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Lab1", NULL, NULL);

	if (window == NULL) {
		std::cout << "Fail" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 800, 800);

	// Creating Shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Creating program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	// Del unused
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	projlocation = glGetUniformLocation(shaderProgram, "proj");
	glfwSetFramebufferSizeCallback(window, frame_callback);


	///////////
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//////after
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//////


	glClearColor(0.07f, 0.017f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	glUseProgram(shaderProgram);


	frame_callback(window, 800, 800);
	std::vector<float> verticies;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// create vertex data
		//coordinate lines
		verticies.push_back(-1.0f); verticies.push_back(0.0f);
		verticies.push_back(1.0f); verticies.push_back(0.0f);
		verticies.push_back(0.0f); verticies.push_back(-1.0f);
		verticies.push_back(0.0f); verticies.push_back(1.0f);

		// 0.5 marks
		verticies.push_back(0.5f); verticies.push_back(-0.03f); //+x
		verticies.push_back(0.5f); verticies.push_back(0.03f);

		verticies.push_back(-0.5f); verticies.push_back(-0.03f);//-x
		verticies.push_back(-0.5f); verticies.push_back(0.03f);

		verticies.push_back(-0.03f); verticies.push_back(0.5f);//+y
		verticies.push_back(0.03f); verticies.push_back(0.5f);

		verticies.push_back(-0.03f); verticies.push_back(-0.5f);//-y
		verticies.push_back(0.03f); verticies.push_back(-0.5f);

		// strelki
		verticies.push_back(0.97f); verticies.push_back(0.03f);
		verticies.push_back(1.0f); verticies.push_back(0.0f);
		verticies.push_back(0.97f); verticies.push_back(-0.03f);
		verticies.push_back(1.0f); verticies.push_back(0.0f);

		verticies.push_back(-0.03f); verticies.push_back(0.97f);
		verticies.push_back(0.0f); verticies.push_back(1.0f);
		verticies.push_back(0.03f); verticies.push_back(0.97f);
		verticies.push_back(0.0f); verticies.push_back(1.0f);

		// simple numbers

		//draw 0.5 X
		verticies.push_back(0.47f); verticies.push_back(-0.02f); //.
		verticies.push_back(0.49f); verticies.push_back(-0.02f); //.-.
		verticies.push_back(0.49f); verticies.push_back(-0.02f);
		verticies.push_back(0.49f); verticies.push_back(-0.04f); // .-.-
		verticies.push_back(0.49f); verticies.push_back(-0.04f);
		verticies.push_back(0.47f); verticies.push_back(-0.04f);
		verticies.push_back(0.47f); verticies.push_back(-0.04f);
		verticies.push_back(0.47f); verticies.push_back(-0.02f);
		verticies.push_back(0.5f); verticies.push_back(-0.04f);
		verticies.push_back(0.5f); verticies.push_back(-0.06f);

		verticies.push_back(0.53f); verticies.push_back(-0.02f);
		verticies.push_back(0.51f); verticies.push_back(-0.02f);
		verticies.push_back(0.51f); verticies.push_back(-0.02f);
		verticies.push_back(0.51f); verticies.push_back(-0.03f);
		verticies.push_back(0.51f); verticies.push_back(-0.03f);
		verticies.push_back(0.53f); verticies.push_back(-0.03f);
		verticies.push_back(0.53f); verticies.push_back(-0.03f);
		verticies.push_back(0.53f); verticies.push_back(-0.04f);
		verticies.push_back(0.53f); verticies.push_back(-0.04f);
		verticies.push_back(0.51f); verticies.push_back(-0.04f);
		
		//  draw 1 X
		verticies.push_back(0.99f); verticies.push_back(-0.02f);
		verticies.push_back(0.99f); verticies.push_back(-0.05f);
		verticies.push_back(0.98f); verticies.push_back(-0.05f);
		verticies.push_back(1.0f); verticies.push_back(-0.05f);
		
		//draw -0.5
		verticies.push_back(-0.51f); verticies.push_back(-0.02f); //.
		verticies.push_back(-0.53f); verticies.push_back(-0.02f); //.-.
		verticies.push_back(-0.53f); verticies.push_back(-0.02f);
		verticies.push_back(-0.53f); verticies.push_back(-0.04f); // .-.-
		verticies.push_back(-0.53f); verticies.push_back(-0.04f);
		verticies.push_back(-0.51f); verticies.push_back(-0.04f);
		verticies.push_back(-0.51f); verticies.push_back(-0.04f);
		verticies.push_back(-0.51f); verticies.push_back(-0.02f);
		verticies.push_back(-0.5f); verticies.push_back(-0.04f);
		verticies.push_back(-0.5f); verticies.push_back(-0.06f);

		verticies.push_back(-0.49f); verticies.push_back(-0.02f);
		verticies.push_back(-0.47f); verticies.push_back(-0.02f);
		verticies.push_back(-0.49f); verticies.push_back(-0.02f);
		verticies.push_back(-0.49f); verticies.push_back(-0.03f);
		verticies.push_back(-0.47f); verticies.push_back(-0.03f);
		verticies.push_back(-0.49f); verticies.push_back(-0.03f);
		verticies.push_back(-0.47f); verticies.push_back(-0.03f);
		verticies.push_back(-0.47f); verticies.push_back(-0.04f);
		verticies.push_back(-0.49f); verticies.push_back(-0.04f);
		verticies.push_back(-0.47f); verticies.push_back(-0.04f);
		//draw -
		verticies.push_back(-0.56); verticies.push_back(-0.03f);
		verticies.push_back(-0.54); verticies.push_back(-0.03f);

		// draw -1 X
		verticies.push_back(-0.96f); verticies.push_back(-0.02f);
		verticies.push_back(-0.96f); verticies.push_back(-0.05f);
		verticies.push_back(-0.94f); verticies.push_back(-0.05f);
		verticies.push_back(-0.97); verticies.push_back(-0.05f);

		verticies.push_back(-0.99); verticies.push_back(-0.04f);
		verticies.push_back(-0.98); verticies.push_back(-0.04f);

		//draw 1 Y
		verticies.push_back(-0.02f); verticies.push_back(0.96f);
		verticies.push_back(-0.05f); verticies.push_back(0.96f);
		verticies.push_back(-0.03f); verticies.push_back(0.96f);
		verticies.push_back(-0.03f); verticies.push_back(0.99f);

		//draw -1 Y

		verticies.push_back(-0.04f); verticies.push_back(-0.97f);
		verticies.push_back(-0.05f); verticies.push_back(-0.97f);

		verticies.push_back(-0.02f); verticies.push_back(-0.99f);
		verticies.push_back(-0.05f); verticies.push_back(-0.99f);
		verticies.push_back(-0.03f); verticies.push_back(-0.99f);
		verticies.push_back(-0.03f); verticies.push_back(-0.96f);


		// 0.5 Y

		verticies.push_back(-0.04f); verticies.push_back(0.47f);  //.
		verticies.push_back(-0.04f); verticies.push_back(0.49f);  //.-.
		verticies.push_back(-0.04f); verticies.push_back(0.49f);
		verticies.push_back(-0.06f); verticies.push_back(0.49f); // .-.-
		verticies.push_back(-0.06f); verticies.push_back(0.49f);
		verticies.push_back(-0.06f); verticies.push_back(0.47f);
		verticies.push_back(-0.06f); verticies.push_back(0.47f);
		verticies.push_back(-0.04f); verticies.push_back(0.47f);
		verticies.push_back(-0.03f); verticies.push_back(0.47f);
		verticies.push_back(-0.03f); verticies.push_back(0.45f);
		
		verticies.push_back(-0.025f); verticies.push_back(0.49f);
		verticies.push_back(-0.009f); verticies.push_back(0.49f);
		verticies.push_back(-0.02f); verticies.push_back(0.49f);
		verticies.push_back(-0.02f); verticies.push_back(0.48f);
		verticies.push_back(-0.02f); verticies.push_back(0.48f);
		verticies.push_back(-0.009f); verticies.push_back(0.48f);
		verticies.push_back(-0.009f); verticies.push_back(0.48f);
		verticies.push_back(-0.009f); verticies.push_back(0.47f);
		verticies.push_back(-0.009f); verticies.push_back(0.47f);
		verticies.push_back(-0.025f); verticies.push_back(0.47f);

		// - 0.5 Y

		verticies.push_back(-0.04f); verticies.push_back(-0.47f);  //.
		verticies.push_back(-0.04f); verticies.push_back(-0.49f);  //.-.
		verticies.push_back(-0.04f); verticies.push_back(-0.47f);
		verticies.push_back(-0.06f); verticies.push_back(-0.47f); // .-.-
		verticies.push_back(-0.06f); verticies.push_back(-0.49f);
		verticies.push_back(-0.06f); verticies.push_back(-0.47f);
		verticies.push_back(-0.06f); verticies.push_back(-0.49f);
		verticies.push_back(-0.04f); verticies.push_back(-0.49f);
		verticies.push_back(-0.03f); verticies.push_back(-0.49f);
		verticies.push_back(-0.03f); verticies.push_back(-0.51f);
		
		verticies.push_back(-0.025f); verticies.push_back(-0.47f);
		verticies.push_back(-0.009f); verticies.push_back(-0.47f);
		verticies.push_back(-0.02f); verticies.push_back(-0.47f);
		verticies.push_back(-0.02f); verticies.push_back(-0.48f);
		verticies.push_back(-0.02f); verticies.push_back(-0.48f);
		verticies.push_back(-0.009f); verticies.push_back(-0.48f);
		verticies.push_back(-0.009f); verticies.push_back(-0.48f);
		verticies.push_back(-0.009f); verticies.push_back(-0.49f);
		verticies.push_back(-0.009f); verticies.push_back(-0.49f);
		verticies.push_back(-0.025f); verticies.push_back(-0.49f);

		verticies.push_back(-0.09f); verticies.push_back(-0.48);
		verticies.push_back(-0.07f); verticies.push_back(-0.48);

		for (float x = 0; x < 1.1; x += 0.001f) {
			float y = func(x, a);
			verticies.push_back(x);
			verticies.push_back(y);

		}
		for (float x = 0; x < 1.1; x += 0.001f) {
			float y = func(x, a);
			verticies.push_back(x);
			verticies.push_back(-y);

		}
		for (float x = 0; x < 1.1; x += 0.001f) {
			float y = func(x, a);
			verticies.push_back(-x);
			verticies.push_back(y);

		}
		for (float x = 0; x < 1.1; x += 0.001f) {
			float y = func(x, a);
			verticies.push_back(-x);
			verticies.push_back(-y);

		}

		////////
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		///////

		// imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);


		ImGui::Begin("Settings");
		ImGui::Text("Setup the A below:");
		ImGui::SliderFloat("A", &a, 0.0f, 1.0f);
		ImGui::End();

		// Draw coordiante system
		glDrawArrays(GL_LINES, 0, 4);

		// Draw coordinate lines
		glDrawArrays(GL_LINES, 4, 16);

		// Draw strelki
		glDrawArrays(GL_LINES, 16, 32);

		// Draw numbers
		glDrawArrays(GL_LINES, 32, 125);

		// Draw function
		glDrawArrays(GL_LINE_STRIP, 125, (verticies.size() / 2) - 125);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
		glfwPollEvents();
		verticies.clear();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}