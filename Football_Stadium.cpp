#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<math.h>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
using namespace std;
#define PI 3.14159;

const GLchar* VertexShaderSrc = "#version 440 core\n"
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 color;"
"layout (location = 2) in vec2 aTexCoord;"

"out vec3 MyColor;"
"out vec2 TexCoord;"

"void main()"
"{"
"	gl_Position = vec4(pos, 1.0f);"
"	MyColor = color;"
"	TexCoord = vec2(aTexCoord.x,aTexCoord.y);"
"}";

const GLchar* VertexUniShaderSrc = "#version 440 core\n"
"layout (location = 0) in vec3 pos;"
"void main()"
"{"
"	gl_Position = vec4(pos, 1.0f);"
"}";

const GLchar* FragmentShaderSrc = "#version 440 core\n"
"out vec4 FragColor;"
"in vec3 MyColor;"
"in vec2 TexCoord;"

"uniform sampler2D texture1;"

"void main()"
"{"
"	FragColor = texture(texture1, TexCoord)*vec4(MyColor,1.0);"
"}";

const GLchar* FragmentTexShaderSrc = "#version 440 core\n"
"out vec4 FragColor;"
"in vec3 MyColor;"
"in vec2 TexCoord;"

"uniform sampler2D texture2;"

"void main()"
"{"
"	FragColor = texture(texture2, TexCoord)*vec4(MyColor,1.0);"
"}";

const GLchar* FragmentDefaultShaderSrc = "#version 440 core\n"
"out vec4 FragColor;"
"void main()"
"{"
"	FragColor = vec4(0.0f,0.0f,0.0f,0.0f);"
"}";

const GLchar* FragmentUniShaderSrc = "#version 440 core\n"
"out vec4 FragColor;"
"uniform vec4 MyColor;"
"void main()"
"{"
"	FragColor = MyColor;"
"}";

int main()
{
	if (!glfwInit())
	{
		cout << "fail to initilize\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Football Stadium", NULL, NULL);

	if (window == NULL)
	{
		cout << " Fail to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, 800, 800);

	//Staduim Vertices & Color
	GLfloat Rectangle_vertices[] = {
		 0.7f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		 0.7f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-0.7f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		-0.7f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f
	};
	GLuint Rectangle_indices[] = {
	0, 1,  2 ,
	0, 2, 3
	};
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Rectangle_vertices), Rectangle_vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Rectangle_indices), Rectangle_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Lines around the staduim vertices
	GLfloat Line1_vertices[] = {
		 0.7f,  0.5f, 0.0f,
		 0.7f, -0.2f, 0.0f,
		-0.7f, -0.2f, 0.0f,
		-0.7f,  0.5f, 0.0f
	};
	GLuint VBO1, VAO1;
	//LINE around the staduim
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Line1_vertices), Line1_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//Staduim Mid Vertical Line
	GLfloat Line2_vertices[] = {
		0.0f,   0.5f, 0.0f,
		0.0f,  -0.2f, 0.0f
	};
	GLuint VBO2, VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Line2_vertices), Line2_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Mid Circle 
	const GLint numofvertices = 360;
	GLfloat r = 0.25;
	GLfloat TwoPI = 2.0 * PI;
	GLfloat x_point = 0.0;
	GLfloat y_point = 0.15;
	GLfloat CircleVertices[3 * numofvertices];
	for (int i = 0; i < numofvertices; i++) {
		CircleVertices[i * 3] = x_point + r * cos(i * TwoPI / numofvertices);
		CircleVertices[(i * 3)+1] = y_point + r * sin(i * TwoPI / numofvertices);
		CircleVertices[(i * 3) + 2] = 0.0;
	}
	GLuint VBO3, VAO3;
	glGenVertexArrays(1, &VAO3);
	glBindVertexArray(VAO3);
	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CircleVertices), CircleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//Right Goal Vertices
	GLfloat Line3_vertices[] = {
		 0.7f,  0.3f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, //Top right
		 0.5f,  0.3f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		 0.7f,  0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f //Bottom right
	};
	GLuint Goal1_indices[] = {
	0, 1,  2 ,
	0, 2, 3
	};
	GLuint VBO4, VAO4, EBO4;
	glGenVertexArrays(1, &VAO4);
	glBindVertexArray(VAO4);
	glGenBuffers(1, &VBO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Line3_vertices), Line3_vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Goal1_indices), Goal1_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//left Goal Vertices
	GLfloat Line4_vertices[] = {
		-0.5f,  0.3f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,// Top right
		-0.7f,  0.3f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,// Top Left
		-0.7f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,// Bottom Left
		-0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f// Bottom right
	};
	GLuint Goal2_indices[] = {
	0, 1,  2 ,
	0, 2, 3
	};
	GLuint VBO5, VAO5, EBO5;
	glGenVertexArrays(1, &VAO5);
	glBindVertexArray(VAO5);
	glGenBuffers(1, &VBO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Line4_vertices), Line4_vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO5);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Goal2_indices), Goal2_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//Aroud Right Goal Lines
	GLfloat Line5_vertices[] = {
		 0.7f,   0.4f, 0.0f, //Top right
		 0.47f,  0.4f, 0.0f, //Top Left
		 0.47f, -0.1f, 0.0f, //Bottom Left
		 0.7f,  -0.1f, 0.0f //Bottom right
	};
	GLuint VBO6, VAO6;
	glGenVertexArrays(1, &VAO6);
	glBindVertexArray(VAO6);
	glGenBuffers(1, &VBO6);
	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Line5_vertices), Line5_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Around Left Goal Lines
	GLfloat Line6_vertices[] = {
		 -0.7f,   0.4f, 0.0f, //Top right
		 -0.47f,  0.4f, 0.0f, //Top Left
		 -0.47f, -0.1f, 0.0f, //Bottom Left
		 -0.7f,  -0.1f, 0.0f //Bottom right
	};
	GLuint VBO7, VAO7;
	glGenVertexArrays(1, &VAO7);
	glBindVertexArray(VAO7);
	glGenBuffers(1, &VBO7);
	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Line6_vertices), Line6_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Create Vertex Shader
	GLint result = 1;
	GLchar infoLog[512];
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &VertexShaderSrc, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		cout << "Vertex Shader Compilation Error! " << infoLog << endl;
	}

	//Create Vertex Uniform Shader
	result = 1;
	GLuint vus = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vus, 1, &VertexUniShaderSrc, NULL);
	glCompileShader(vus);
	glGetShaderiv(vus, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vus, 512, NULL, infoLog);
		cout << "Vertex Uniform Shader Compilation Error! " << infoLog << endl;
	}

	//Create Fragment Shader
	result = 1;
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &FragmentShaderSrc, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		cout << "Fragment Shader Compilation Error! " << infoLog << endl;
	}

	//Create Fragment Shader for Default Color
	result = 1;
	GLuint fds = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fds, 1, &FragmentDefaultShaderSrc, NULL);
	glCompileShader(fds);
	glGetShaderiv(fds, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fds, 512, NULL, infoLog);
		cout << "Fragment Default Shader Compilation Error! " << infoLog << endl;
	}

	//Create Fragment Shader for Uniform Color
	result = 1;
	GLuint fus = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fus, 1, &FragmentUniShaderSrc, NULL);
	glCompileShader(fus);
	glGetShaderiv(fus, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fus, 512, NULL, infoLog);
		cout << "Fragment Uniform Shader Compilation Error! " << infoLog << endl;
	}

	//Create Fragment Shader for Texture
	result = 1;
	GLuint fts = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fts, 1, &FragmentTexShaderSrc, NULL);
	glCompileShader(fts);
	glGetShaderiv(fts, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fts, 512, NULL, infoLog);
		cout << "Fragment Uniform Shader Compilation Error! " << infoLog << endl;
	}

	//Create Program for Default Color Shader
	GLint ShaderDefaultProgram = glCreateProgram();
	glAttachShader(ShaderDefaultProgram, vus);
	glAttachShader(ShaderDefaultProgram, fds);
	glLinkProgram(ShaderDefaultProgram);
	result = 1;
	glGetProgramiv(ShaderDefaultProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderDefaultProgram, 512, NULL, infoLog);
		cout << "Shader Program Linker Compilation Error! " << infoLog << endl;
	}

	//Create Program for Default Color Shader
	GLint ShaderTexProgram = glCreateProgram();
	glAttachShader(ShaderTexProgram, vs);
	glAttachShader(ShaderTexProgram, fts);
	glLinkProgram(ShaderTexProgram);
	result = 1;
	glGetProgramiv(ShaderTexProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderTexProgram, 512, NULL, infoLog);
		cout << "Shader Program Linker Compilation Error! " << infoLog << endl;
	}

	//Create Program for Vertices Color Shader
	GLint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vs);
	glAttachShader(ShaderProgram, fs);
	glLinkProgram(ShaderProgram);
	result = 1;
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		cout << "Shader Program Linker Compilation Error! " << infoLog << endl;
	}

	//Create program for Uniform Color Shader
	GLint ShaderUniProgram = glCreateProgram();
	glAttachShader(ShaderUniProgram, vus);
	glAttachShader(ShaderUniProgram, fus);
	glLinkProgram(ShaderUniProgram);
	result = 1;
	glGetProgramiv(ShaderUniProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderUniProgram, 512, NULL, infoLog);
		cout << "Shader Uniform Program Linker Compilation Error! " << infoLog << endl;
	}

	GLuint texture1, texture2;;
	// Creating Texture 1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Stadium_Grass.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//Creating Texture 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("white.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glUseProgram(ShaderProgram);
	glUniform1i(glGetUniformLocation(ShaderProgram, "texture1"), 0);
	glUseProgram(ShaderTexProgram);
	glUniform1i(glGetUniformLocation(ShaderTexProgram, "texture2"), 0);

	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(fts);
	glDeleteShader(vus);
	glDeleteShader(fus);
	glDeleteShader(fds);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(1.0f, 0.5f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Rectangle
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUseProgram(ShaderProgram);
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//LINE Around the Stadium
		glUseProgram(ShaderDefaultProgram);
		glBindVertexArray(VAO1);
		glLineWidth(5.0);
		glDrawArrays(GL_LINE_LOOP,0, 4);

		//Mid Line
		glUseProgram(ShaderDefaultProgram);
		glBindVertexArray(VAO2);
		glLineWidth(5.0);
		glDrawArrays(GL_LINES, 0, 2);

		//Mid Circle
		glUseProgram(ShaderDefaultProgram);
		glBindVertexArray(VAO3);
		glLineWidth(5.0);
		glPolygonMode(GL_FRONT, GL_LINE);
		glDrawArrays(GL_LINE_LOOP, 0, 360);

		// Right Goal
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUseProgram(ShaderTexProgram);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Left Goal
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUseProgram(ShaderTexProgram);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Around Right Goal Lines
		glUseProgram(ShaderDefaultProgram);
		glBindVertexArray(VAO6);
		glLineWidth(5.0);
		glDrawArrays(GL_LINE_STRIP, 0, 4);

		//Around Left Goal Lines
		glUseProgram(ShaderDefaultProgram);
		glBindVertexArray(VAO7);
		glLineWidth(5.0);
		glDrawArrays(GL_LINE_STRIP, 0, 4);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);
	glDeleteVertexArrays(1, &VAO6);
	glDeleteBuffers(1, &VBO6);
	glDeleteVertexArrays(1, &VAO7);
	glDeleteBuffers(1, &VBO7);

	glfwDestroyWindow(window);
	glfwTerminate();
}