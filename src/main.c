//#include <glad/glad.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include <cglm/cglm.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
#define WINDOW_TITLE	"Platform"
#define	WALL_TEXTURE	(DATA_PATH "/images/wall.jpg")
#define	FONT_FACE		(DATA_PATH "/fonts/FreeMono.ttf")

struct Character
{
	GLuint TextureID;
	vec2 Size;
	vec2 Bearing;
	GLuint Advance;
};

static struct Character Characters[128];

//static void framebuffer_size_callback(GLFWwindow *win, int width, int height);
//static void processInput(GLFWwindow *window);
//static void mouse_click_callback(GLFWwindow *window, int btn, int act, int mod);

static void RenderText(int shader, const char *str, GLfloat x, GLfloat y,
	GLfloat scale);

/*static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static vec3 cameraPos = {0.0f, 0.0f, 3.0f};
static vec3 cameraFront = {0.0f, 0.0f, -1.0f};
static vec3 cameraUp = {0.0f, 1.0f, 0.0f};*/
static GLuint VBOt, VAOt;

/*static const char *vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec2 aTexCoord;\n"
	"out vec2 TexCoord;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	"	TexCoord = aTexCoord;\n"
	"}\0";
static const char *fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D ourTexture;\n"	
	"void main()\n"
	"{\n"
	"	FragColor = texture(ourTexture, TexCoord);\n"
	"}\0";*/
static const char *vertexShaderTexSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec4 vertex;\n"
	"out vec2 TexCoords;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
	"	TexCoords = vertex.zw;\n"
	"}\0";
static const char *fragmentShaderTexSource = 
	"#version 330 core\n"
	"in vec2 TexCoords;\n"
	"out vec4 color;\n"
	"uniform sampler2D text;\n"
	"uniform vec3 textColor;\n"	
	"void main()\n"
	"{\n"
	"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
	"	color = vec4(textColor, 1.0) * sampled;\n"
	"}\0";

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
		WINDOW_TITLE, 0, 0);
	if (window == 0)
	{
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, 800, 600);
	/*if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD\n");
		return 2;
	}*/
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_DEPTH_TEST);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetMouseButtonCallback(window, mouse_click_callback);

	/*int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glCompileShader(vertexShader);
*/
	int success;
	char infoLog[512];
	/*glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
		fprintf(stderr, "Error: vertex shader compilation failed\n%s\n",
			infoLog);
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
		fprintf(stderr, "Error: fragment shader compilation failed\n%s\n",
			infoLog);
	}

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
		fprintf(stderr, "Error: program shader linking failed\n%s\n",
			infoLog);		
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
*/
	int vertexShaderTex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderTex, 1, &vertexShaderTexSource, 0);
	glCompileShader(vertexShaderTex);

	glGetShaderiv(vertexShaderTex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderTex, 512, 0, infoLog);
		fprintf(stderr, "Error: vertex shader tex compilation failed\n%s\n",
			infoLog);
	}
	int fragmentShaderTex = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderTex, 1, &fragmentShaderTexSource, 0);
	glCompileShader(fragmentShaderTex);
	glGetShaderiv(fragmentShaderTex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderTex, 512, 0, infoLog);
		fprintf(stderr, "Error: fragment shader tex compilation failed\n%s\n",
			infoLog);
	}

	int shaderProgramTex = glCreateProgram();
	glAttachShader(shaderProgramTex, vertexShaderTex);
	glAttachShader(shaderProgramTex, fragmentShaderTex);
	glLinkProgram(shaderProgramTex);
	glGetProgramiv(shaderProgramTex, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramTex, 512, 0, infoLog);
		fprintf(stderr, "Error: program shader tex linking failed\n%s\n",
			infoLog);		
	}
	glDeleteShader(vertexShaderTex);
	glDeleteShader(fragmentShaderTex);

	mat4 projection_tex;

	glm_ortho_default((float) SCREEN_WIDTH / SCREEN_HEIGHT, projection_tex);
	glUseProgram(shaderProgramTex);
	unsigned int projectTexLoc = glGetUniformLocation(shaderProgramTex,
		"projection");
	glUniformMatrix4fv(projectTexLoc, 1, GL_FALSE, (float*) projection_tex);

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		fprintf(stderr, "Could not init FreeType Library\n");

	FT_Face face;
	if (FT_New_Face(ft, FONT_FACE, 0, &face))
		fprintf(stderr, "Failed to load font\n");

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (int c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
			fprintf(stderr, "Failed to load glyph\n");
            continue;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		vec2 ssize = {face->glyph->bitmap.width, face->glyph->bitmap.rows};
		vec2 bbear = {face->glyph->bitmap_left, face->glyph->bitmap_top};
        struct Character character = {
            texture,
            {ssize[0], ssize[1]},
            {bbear[0], bbear[1]},
            face->glyph->advance.x
        };
      	Characters[c] = character;
    }
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAOt);
	glGenBuffers(1, &VBOt);
	glBindVertexArray(VAOt);
	glBindBuffer(GL_ARRAY_BUFFER, VBOt);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/*float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(WALL_TEXTURE, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		fprintf(stderr, "Failed to load texture %s\n", WALL_TEXTURE);
	}
	stbi_image_free(data);

	vec3 cubePositions[] = {
		{0.0f,  0.0f,  0.0f},
		{2.0f,  5.0f, -15.0f},
		{-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f},
		{2.4f, -0.4f, -3.5f},
		{-1.7f,  3.0f, -7.5f},
		{1.3f, -2.0f, -2.5f},
		{1.5f,  2.0f, -2.5f},
		{1.5f,  0.2f, -1.5f},
		{-1.3f,  1.0f, -1.5f}
	};
*/
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	/*	float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
*/
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*glBindTexture(GL_TEXTURE_2D, texture);

		mat4 view = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		mat4 projection = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		vec3 cameraTarget;

		glUseProgram(shaderProgram);
		glm_vec3_add(cameraPos, cameraFront, cameraTarget);
		glm_lookat(cameraPos, cameraTarget, cameraUp, view);
		glm_perspective(glm_rad(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT,
			0.1f, 100.0f, projection);

		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		unsigned int projectLoc = glGetUniformLocation(shaderProgram,
			"projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*) view);
		glUniformMatrix4fv(projectLoc, 1, GL_FALSE, (float*) projection);

		for (int i = 0; i < sizeof cubePositions / sizeof cubePositions[0]; i++)
		{
			mat4 model = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};

			glm_translate(model, cubePositions[i]);
			glm_rotate(model, (float)glfwGetTime(), (vec3){1.0f, 0.3f, 0.5f});
			
			unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*) model);
			
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/

		RenderText(shaderProgramTex, "ABCDEF", 25.0f, 25.0f, 1.0f);

		glfwSwapBuffers(window);
	}
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAOt);
	glDeleteBuffers(1, &VBOt);

	glfwTerminate();
}

static void framebuffer_size_callback(GLFWwindow *win, int width, int height)
{
	(void) win;
	glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window)
{
	/*vec3 temp;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm_vec3_scale(cameraFront, cameraSpeed, temp);
		glm_vec3_add(cameraPos, temp, cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm_vec3_scale(cameraFront, cameraSpeed, temp);
		glm_vec3_sub(cameraPos, temp, cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm_vec3_cross(cameraFront, cameraUp, temp);
		glm_vec3_normalize(temp);
		glm_vec3_scale(temp, cameraSpeed, temp);
		glm_vec3_sub(cameraPos, temp, cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm_vec3_cross(cameraFront, cameraUp, temp);
		glm_vec3_normalize(temp);
		glm_vec3_scale(temp, cameraSpeed, temp);
		glm_vec3_add(cameraPos, temp, cameraPos);
	}*/
}

static void mouse_click_callback(GLFWwindow *window, int btn, int act, int mod)
{
	double xpos, ypos;

	if (btn == 0 && act == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xpos, &ypos);
		fprintf(stderr, "Mouse click: x = %f y = %f\n", xpos, ypos);
	}
}

static void RenderText(int shader, const char *str, GLfloat x, GLfloat y,
	GLfloat scale)
{
	glUseProgram(shader);
	glUniform3f(glGetUniformLocation(shader, "textColor"),
		0.5f, 0.8f, 0.2f);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAOt);

    for (int i = 0; str[i] != '\0'; i++) 
    {
		char c = str[i];
        struct Character ch = Characters[c];
        GLfloat xpos = x + ch.Bearing[0] * scale;
        GLfloat ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

        GLfloat w = ch.Size[0] * scale;
        GLfloat h = ch.Size[1] * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBOt);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
