/**************************************************************************************************************
****	UNIVERSIDAD NACIONAL AUTÓNOMA DE MÉXICO
****			FACULTAD DE INGENIERÍA
****
****	Grupo: 01
****	Laboratorio de Computación Gráfica e Interacción Humano Computadora
****	Reporte PRÁCTICA 8
****	ALUMNO: RAMÍREZ CASTILLO MIGUEL ÁNGEL
****	VERSIÓN FINAL- Proyecto
****	DESCRIPCIÓN:
****		El presente código  implementa el diseño y construcción de la casa de 
****		Coraje, El perro cobarde a través de carga de modelos y uso de primitivas.
****
**************************************************************************************************************/




#include <iostream>
#include <cmath>
#include <string.h>

#pragma warning(disable : 4996)

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"    //PARA LA TRANSPARENCIA Y TEXTURAS. MUY IMPORTANTE

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"   //PARA MODELOS EN 3D


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(25.0f, 3.0f, 40.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float spotAngle = 0.0f;



//Animación de coraje
glm::vec3 PosIni(3.0f, 5.0f, 3.59f);
float rot = 0.0f;
float movKitX = 0.0;
float movKitY = 0.0;
float movKitZ = 0.0;
float rotKit = -90.0;

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;



//Animación camioneta
glm::vec3 PosIni2(35.0f, -0.15f, 12.0f);
float rot2 = 0.0f;
float movKitX2 = 0.0;
float movKitY2 = 0.0;
float movKitZ2 = 0.0;
float rotKit2 = 45.0;
float rotKit_X = 0;

bool circuito2 = false;
bool recorrido12 = true;
bool recorrido22 = false;
bool recorrido32 = false;
bool recorrido42 = false;
bool recorrido52 = false;




//			FUNCION PARA CARGAR TEXTURAS
GLuint cargaTextura(const char* nombreFoto);

//			FUNCION para revisar qué extensión tiene la imagen
int verificaExtensionImagen(char* nombreFoto);






// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 LightP1;



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, " REPORTE Practica 8", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	/*Carga de modelos*/
	Model Mymodel((char*)"Models/escalera/escalera_nueva.obj"); //Escalera
	Model Mymodel1((char*)"Models/ventana/ventana_final.obj"); //Ventanas
	Model Mymodel2((char*)"Models/trianguloFachada/triangulo.obj"); //Triangulo fachada
	Model Mymodel3((char*)"Models/puerta/puerta_final.obj"); //puerta
	Model Mymodel4((char*)"Models/antenaConejo/antena.obj"); //antena
	Model Mymodel5((char*)"Models/ornamento/ornamento.obj"); //ornamento
	Model Mymodel6((char*)"Models/alfombra/alfombra.obj"); //alfombra
	Model Mymodel7((char*)"Models/sillon/sillon.obj"); //sillon
	Model Mymodel8((char*)"Models/silla/silla.obj"); //silla
	Model Mymodel9((char*)"Models/Coraje/version4/coraje_final.obj"); //coraje
	Model Mymodel10((char*)"Models/lampara/lampara.obj"); //silla
	Model Mymodel11((char*)"Models/camioneta/Truck_obj.obj"); //camioneta
	Model Mymodel12((char*)"Models/molino/molino_final/molino.obj"); //molino
	Model Mymodel13((char*)"Models/molino/molino_final/aspas.obj"); //aspas
	Model Mymodel14((char*)"Models/computadora/computadora.obj"); //computadora
	Model Mymodel15((char*)"Models/roca/roca_final.obj"); //roca
	Model Piso((char*)"Models/Carro/Piso.obj");


	






	// Build and compile our shader program
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);






	GLuint t_exterior, t_techo,t_triangulo, t_pisoEstrado,t_fachada,t_tapiz,t_techoInterno, t_pisoInterno,t_ventana, t_television, t_television_atras, t_pantallaTv,
		t_coltrolesTv, t_cajaReloj, t_reloj, t_campana, t_tubo, t_pisoExterior;
	/*					CREAR LAS TEXTURAS				 */			
	//Manda a llamar las funciones de cargar textura
	t_exterior = cargaTextura("images/exterior15_v2.jpg");
	t_techo = cargaTextura("images/exterior4_2.png");
	t_triangulo = cargaTextura("images/exterior15_v2_triang.png");
	t_pisoEstrado = cargaTextura("images/piso_estrado_v2.jpg");
	t_fachada = cargaTextura("images/exterior15_v3.jpg");
	t_tapiz = cargaTextura("images/tapiz2_v2.jpg");
	t_techoInterno = cargaTextura("images/techo_interno_v2.png");
	t_pisoInterno = cargaTextura("images/piso_interno2_v6.jpg");
	t_ventana = cargaTextura("images/ventana_v2.png");
	t_television = cargaTextura("images/tele1_v3.jpg");
	t_television_atras = cargaTextura("images/tv1.jpg"); //Textura para la parte trasera de la TV
	t_pantallaTv = cargaTextura("images/sanAndreas_v4.jpg"); //Textura para la pantalla
	t_coltrolesTv = cargaTextura("images/controlTV_v2.jpg"); 
	t_cajaReloj = cargaTextura("images/tv1.jpg"); 
	t_reloj = cargaTextura("images/reloj_v2.jpg");
	t_campana = cargaTextura("images/reloj2.jpg");
	t_tubo = cargaTextura("images/tubo.png");
	t_pisoExterior = cargaTextura("images/tierra.jpg");

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures => PARA EL CUBO DE FONDO
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/rapture_lf.tga");
	faces.push_back("SkyBox/rapture_rt.tga");
	faces.push_back("SkyBox/rapture_up.tga");
	faces.push_back("SkyBox/rapture_dn.tga");
	faces.push_back("SkyBox/rapture1_bk.tga");
	faces.push_back("SkyBox/rapture_ft.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);


	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    //Load Model
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f,1.0f,1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(VAO);
		glm::mat4 model(1);
		glm::mat4 tmp(1);




	
		/*************FACHADA**************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_exterior);
		//Pared trasera
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 12.5f, -27.5f));
		model = glm::scale(model, glm::vec3(45.0f, 25.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pared Frontal
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 12.5f, 27.5f));
		model = glm::scale(model, glm::vec3(45.0f, 25.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Pared lateral
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 12.5f, -22.5f));
		model = glm::scale(model, glm::vec3(55.0f, 25.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pared lateral
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 12.5f, 22.5f));
		model = glm::scale(model, glm::vec3(55.0f, 25.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Estrado entrada 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 35.0f));
		model = glm::scale(model, glm::vec3(35.0f, 3.0f, 15.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_pisoEstrado);
		//Piso Estrado entrada
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 3.15f, 35.25f));
		model = glm::scale(model, glm::vec3(36.5f, 0.3f, 15.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_techo);
		//Teja lado derecho techo
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 30.0f));
		model = glm::scale(model, glm::vec3(55.0f, 40.0f, 0.5f));
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, 8.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Teja lado izquierdo techo
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, -30.0f));
		model = glm::scale(model, glm::vec3(55.0f, 40.0f, 0.5f));
		model = glm::translate(model, glm::vec3(0.0f, 0.099f, -7.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_fachada);
		//Techo entrada
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 41.0f, 21.5f));
		model = glm::scale(model, glm::vec3(42.0f, 0.3f, 17.5f));
		model = glm::translate(model, glm::vec3(0.0f, -30.0f, 0.17f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_pisoEstrado);
		//Polín derecho entrada 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(13.0f, 10.25f, 38.0f));
		model = glm::scale(model, glm::vec3(1.0f, 14.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Polín izquierdo entrada 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-13.0f, 10.25f, 38.0f));
		model = glm::scale(model, glm::vec3(1.0f, 14.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_exterior);
		//escalon grande entrada  
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 43.75f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 2.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_pisoEstrado);
		//Piso escalon grande entrada  
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.05f, 43.87f));
		model = glm::scale(model, glm::vec3(12.5f, 0.1f, 2.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_exterior);
		//escalon chico entrada  
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 46.25f));
		model = glm::scale(model, glm::vec3(12.0f, 1.0f, 2.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_pisoEstrado);
		//Piso escalon chico entrada  
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.05f, 46.36f));
		model = glm::scale(model, glm::vec3(12.5f, 0.1f, 2.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_tapiz);
		/*Paredes Internas de la casa -Tapiz*/
		//Pared trasera interna
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 12.5f, -25.25f));
		model = glm::scale(model, glm::vec3(45.0f, 25.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pared Frontal interna
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 12.5f, 27.25f));
		model = glm::scale(model, glm::vec3(45.0f, 25.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Pared lateral interna
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 12.5f, -22.25f));
		model = glm::scale(model, glm::vec3(55.0f, 25.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pared lateral interna
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 12.5f, 22.25f));
		model = glm::scale(model, glm::vec3(55.0f, 25.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_pisoInterno);
		//Piso interno casa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(45.0f, 3.0f, 55.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_techoInterno);
		//Loza primer piso
		model = glm::mat4(1.0f);//Parte 1
		model = glm::translate(model, glm::vec3(-7.2f, 24.75f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 0.5f, 55.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);//Parte 2, tiene el hueco de la escalera
		model = glm::translate(model, glm::vec3(15.0f, 24.75f, 13.8f));
		model = glm::scale(model, glm::vec3(14.4f, 0.5f, 27.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);//Parte 3, tiene el hueco de la escalera
		model = glm::translate(model, glm::vec3(15.0f, 24.75f, -24.8f));
		model = glm::scale(model, glm::vec3(14.4f, 0.5f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_exterior);
		//piso 2do piso
		model = glm::mat4(1.0f);//Parte 1
		model = glm::translate(model, glm::vec3(-7.2f, 25.25f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 0.5f, 55.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);//Parte 2, tiene el hueco de la escalera
		model = glm::translate(model, glm::vec3(15.0f, 25.25f, 13.8f));
		model = glm::scale(model, glm::vec3(14.4f, 0.5f, 27.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);//Parte 3, tiene el hueco de la escalera
		model = glm::translate(model, glm::vec3(15.0f, 25.25f, -24.8f));
		model = glm::scale(model, glm::vec3(14.4f, 0.5f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);








		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_tubo);
		//	Ducto Ventilacion
		//Tubo acostado 1
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(20.0f, 30.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Tubo parado 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(21.5f, 32.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Tubo acostado 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(24.0f, 34.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Tubo parado 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.5f, 36.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);




		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_television);
		/*Television*/
		model = glm::mat4(1.0f);//Caja.
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(-9.4f, 0.0f, -8.5f));
		tmp=model = glm::translate(model, glm::vec3(-9.4f, 5.5f, -8.5f));
		model = glm::scale(model, glm::vec3(4.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		model = tmp; //Parte superior
		model = glm::translate(model, glm::vec3(0.0f, 1.55f, 0.0f));
		model = glm::scale(model, glm::vec3(4.7f, 0.3f, 3.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = tmp;//Pata1 delantera izquierda
		model = glm::translate(model, glm::vec3(-1.5f, -2.0f, 0.7f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
		model = tmp;//Pata2 delantera derecha
		model = glm::translate(model, glm::vec3(1.5f, -2.0f, 0.7f));
		model = glm::scale(model, glm::vec3(-0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
		model = tmp;//Pata2 trasera izquierda
		model = glm::translate(model, glm::vec3(1.5f, -2.0f, -0.7f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		model = tmp;//Pata2 trasera derecha
		model = glm::translate(model, glm::vec3(-1.5f, -2.0f, -0.7f));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = tmp;//antena conejo
		model = glm::translate(model, glm::vec3(0.0f, 1.625f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 0.4f, 0.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_television_atras);
		model = tmp; // caja de atras
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, -2.15f));
		model = glm::scale(model, glm::vec3(1.30f, 1.3f, 1.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_pantallaTv);
		model = tmp;//pantalla televisor
		model = glm::translate(model, glm::vec3(-0.36f, 0.0f, 1.5f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_coltrolesTv);
		model = tmp;//Tablero controles
		model = glm::translate(model, glm::vec3(1.33f, 0.0f, 1.5f));
		model = glm::scale(model, glm::vec3(0.7f, 2.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_cajaReloj);
		/*Reloj*/
		model = glm::mat4(1.0f);//Caja.
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::translate(model, glm::vec3(24.5f, 11.0f, -21.0f));
		model = glm::scale(model, glm::vec3(5.0f, 16.0f, 2.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_campana);
		model = tmp; //cara frontal del reloj
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 1.4f));
		model = glm::scale(model, glm::vec3(4.0f, 8.5f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//model = tmp; //Ornamento
		//model = glm::translate(model, glm::vec3(0.0f, 9.1f, 1.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 2.3f, 0.35f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_reloj);
		model = tmp; //reloj
		model = glm::translate(model, glm::vec3(0.0f, 7.6f, 1.4f));
		model = glm::scale(model, glm::vec3(3.7f, 3.7f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);












		

		/*Carga modelo escalera*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(22.25f, 2.5f, -14.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.073f, 0.075f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel.Draw(lightingShader);



		/*Carga modelo ventanas*/
		//Ventana derecha inferior	 Frontal
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.5f, 5.8f, 27.5f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0053f, 0.00462f, 0.022f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel1.Draw(lightingShader);
		
		//Ventana izquierda inferior	 Frontal
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.5f, 5.8f, 27.5f));
		model = glm::scale(model, glm::vec3(0.0053f, 0.00462f, 0.022f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel1.Draw(lightingShader);

		//Ventana izquierda superior	 Frontal
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 26.0f, 27.4f));
		
		model = glm::scale(model, glm::vec3(0.0053f, 0.00462f, 0.022f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel1.Draw(lightingShader);

		//Ventana derecha superior	 Frontal
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.0f, 35.5f, 27.4f));
		model = glm::scale(model, glm::vec3(0.0053f, 0.00462f, 0.022f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel1.Draw(lightingShader);

		//Ventana izquieda superior	 => Lateral Derecha
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-3.0f, 15.5f, 22.5f));
		model = glm::scale(model, glm::vec3(0.0053f, 0.00462f, 0.022f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel1.Draw(lightingShader);

		//Ventana derecha inferior	 => Lateral Derecha
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(10.0f, 9.0f, 22.5f));
		model = glm::scale(model, glm::vec3(0.0053f, 0.00462f, 0.022f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel1.Draw(lightingShader);

		//Ventana izquieda superior	 => Lateral Izquierda
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-3.0f, 15.5f, -22.5f));
		model = glm::scale(model, glm::vec3(0.0053f, 0.00462f, 0.022f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel1.Draw(lightingShader);

		//Ventana derecha inferior	 => Lateral Izquierda
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(10.0f, 9.0f, -22.5f));
		model = glm::scale(model, glm::vec3(0.0053f, 0.00462f, 0.022f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel1.Draw(lightingShader);




		/*Carga modelo triangulo fachada*/
		//Frontal
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.2f, 35.3f, 26.90f));
		model = glm::scale(model, glm::vec3(2.48f, 1.23f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel2.Draw(lightingShader);
		//Trasera
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.2f, 35.3f, -26.90f));
		model = glm::scale(model, glm::vec3(2.48f, 1.23f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel2.Draw(lightingShader);
		


		/*Carga modelo puerta*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 9.57f, 27.35f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel3.Draw(lightingShader);


		/*Carga modelo antena de conejo*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.5f, 7.3f, 0.70f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel4.Draw(lightingShader);


		/*Carga modelo ornamento*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(26.8f, 19.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.25f, 0.38f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel5.Draw(lightingShader);


		/*Carga modelo alfombra*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(42.0f, 3.15f, 33.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.15f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel6.Draw(lightingShader);


		/*Carga modelo sillon*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(12.0f, 4.5f, -8.75f));
		model = glm::scale(model, glm::vec3(0.110f, 0.14f, 0.110f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel7.Draw(lightingShader);

		/*Carga modelo silla*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(-175.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.5f, 3.2f, -5.2f));
		model = glm::scale(model, glm::vec3(0.099f, 0.098f, 0.099f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel8.Draw(lightingShader);

		/*Carga modelo Coraje*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, movKitY, movKitZ));
		//model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel9.Draw(lightingShader);

		/*Carga modelo lampara*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.9f, 3.4f, 19.0f));
		model = glm::scale(model, glm::vec3(0.022f, 0.025f, 0.022f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel10.Draw(lightingShader);

		/*Carga modelo camioneta*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		if (rotKit_X == 0) {
			model = glm::translate(model, PosIni2 + glm::vec3(movKitX2, movKitY2, movKitZ2));
			model = glm::rotate(model, glm::radians(rotKit2), glm::vec3(0.0f, 1.0f, 0.0));
		}
		else {
			model = glm::translate(model, PosIni2 + glm::vec3(movKitX2, movKitY2, movKitZ2));
			model = glm::rotate(model, glm::radians(rotKit_X), glm::vec3(1.0f, 0.0f, 0.0));
		}
		//model = glm::translate(model, glm::vec3(25.0f, 2.5f, -45.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel11.Draw(lightingShader);


		/*Carga modelo molino*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(20.0f, -25.0f, 45.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.35f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel12.Draw(lightingShader);


		/*Carga modelo aspas*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(42.53f, 65.0f, -12.5f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.001f, 00.001f, 0.00095f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel13.Draw(lightingShader);

		/*Carga modelo computadora*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-16.3f, 25.2f, -15.0f));
		model = glm::scale(model, glm::vec3(0.0012f, 00.0012f, 0.0012f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel14.Draw(lightingShader);

		/*Carga modelo roca*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-95.3f, 3.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.11f, 00.11f, 0.11f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel15.Draw(lightingShader);



		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, t_pisoExterior);
		//Piso
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);





		glBindVertexArray(0);
		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		//for (GLuint i = 0; i < 4; i++)
		//{
		//	model = glm::mat4(1);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		glBindVertexArray(0);



		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1); //PORQUE ya hay textura
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture); //Tiene que ser de esta textura
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default





		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


int verificaExtensionImagen(char* nombreFoto) {
	/*Funcion para verificar la extensión que tiene la imagen.
		Retorna
			0 =>  No puso ninguna extensión a la ruta
			1 => es JPG
			2 => Cualquier otro formato
	*/
	int i, inicioExt, j; //i es para recorrer arreglos, inicioExt guarda la posicion donde inicia la extensión
	char extension[4], punto = '.';

	for (i = 0; i < strlen(nombreFoto); i++) {
		if (nombreFoto[i] == punto) {
			inicioExt = i; //Guarda la posición del punto
			break;
		}
	}
	if (i >= strlen(nombreFoto)) {
		return 0; //Signfica que no puso la extensión de la imagen en la ruta
	}

	i = inicioExt + 1;
	for (j = 0; j < 4; j++) {
		extension[j]= nombreFoto[i]; //Se copia la extensión a la variable con el mismo nombre
		i += 1;
	}

	if (strcmp(extension, "jpg\0") == 0 || strcmp(extension, "JPG\0") == 0) { //Comparación. Se hace con 4 digitos porque así aseguramos que es .jpg y no un .jpgq  u otra combinación
		return 1; //Tiene extensión .JPG
	}
	else {
		return 2;//Tiene cualquiera otra extensión
	}
}


/*FUNCION PARA CARGAR TEXTURAS*/
GLuint cargaTextura(const char* nombreFoto) {
	/*
		Función que realiza la carga de texturas 
	PARAMETROS:
			=> char* nombreFoto: Cadena para el nombre y ruta donde está la imagen a utilizar
			*/
	GLuint texture;
	glGenTextures(1, &texture); //Genera textura
	char *nombreFoto2=(char *)nombreFoto; //Inicializa la variable con el nombre de la foto que es un const char
	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true); //Como si volteara una hoja
	unsigned char *image;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load(nombreFoto, &textureWidth, &textureHeight, &nrChannels, 0); //Carga la imagen
	glBindTexture(GL_TEXTURE_2D, texture);

	switch(verificaExtensionImagen(nombreFoto2)){
			/*Revisa el tipo de extensión que tiene la imagen*/
			case 0: //No coloco extension
				std::cout << "Failed to load texture. Falta extensión de la imagen." << std::endl;
				//stbi_image_free(image);
				break;

			case 1: //Es JPG
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
					glGenerateMipmap(GL_TEXTURE_2D);
					if (image)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
						glGenerateMipmap(GL_TEXTURE_2D);
					}
					else
					{
						std::cout << "Failed to load texture" << std::endl;
					}
					stbi_image_free(image);
				break;
			case 2: //Tiene otra extensión
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
					glGenerateMipmap(GL_TEXTURE_2D);
					if (image)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
						glGenerateMipmap(GL_TEXTURE_2D);
					}
					else
					{
						std::cout << "Failed to load texture" << std::endl;
					}
					stbi_image_free(image);
				break;
			default:
				std::cout << "Failed to load texture." << std::endl;
				break;
		}
	return texture;
}


void animacion()
{

	//Movimiento coraje
	if (circuito)
	{
		if (recorrido1)
		{
			rotKit = 90;
			movKitX += 0.1f;
			if (movKitX > 10)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			rotKit = 180;
			movKitY += 0.1; 
			movKitZ -= 0.1f;
			if (movKitY > 22.2)
			{
				recorrido2 = false;
				recorrido3 = true;

			}
		}

		if (recorrido3)
		{
			rotKit = 270;
			movKitX -= 0.1f;
			if (movKitX < 0)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}

		if (recorrido4)
		{
			rotKit = -45;
			movKitZ += 0.1f;
			movKitX -= 0.035f;
			if (movKitZ > 16)
			{
				recorrido4 = false;
				recorrido1 = false;
			}
		}
		if (recorrido5)
		{
			rotKit = 225;
			//movKitX += 0.05f;
			//if (movKitX > -1.0)
			//{
			//	recorrido5 = false;
				//recorrido1 = true;
			//}
			movKitZ = movKitZ;
			movKitX = movKitX;
			recorrido1 = false;
			recorrido2 = false;
			recorrido3 = false;
			recorrido4 = false;
			recorrido5 = false;
		}
	}
	else {
		rotKit = -90;
		movKitX = 0;
		movKitY = 0;
		movKitZ = 0;
		 recorrido1 = true;
		 recorrido2 = false;
		 recorrido3 = false;
		 recorrido4 = false;
		 recorrido5 = false;
	}






	/*ANIMACION CAMIONETA*/
	if (circuito2)
	{
		if (recorrido12)
		{
			//rotKit2 = -90;
			movKitZ2 += 0.1f;
			if (movKitZ2 > 55.0)
			{
				recorrido12 = false;
				recorrido22 = true;
			}
		}
		if (recorrido22)
		{
			rotKit2 = 180;
			//movKitY2 += 0.1;
			movKitX2 -= 0.1f;
			if (movKitX2 < -70)
			{
				recorrido22 = false;
				recorrido32 = true;

			}
		}

		if (recorrido32)
		{
			rotKit2 = 135;
			movKitX2 -= 0.1f;
			movKitZ2 -= 0.1f;
			if (movKitZ2 < -2.5)
			{
				recorrido32 = false;
				recorrido42 = true;
			}
		}

		if (recorrido42)
		{
			
			//rotKit2 = -45;
			movKitZ2 -= 0.01f;
			movKitX2 -= 0.01f;
			if (movKitZ2 <4)
			{
				recorrido42 = false;
				recorrido52 = true;
			}
		}
		if (recorrido52)
		{
			movKitZ2 -= 0.01f;
			movKitX2 += 0.01f;
			rotKit_X += 1;
			if (rotKit_X > 90)
			{
				//recorrido42 = false;
				recorrido52 = false; 
				recorrido12 = false;
				recorrido22 = false;
				recorrido32 = false;
				recorrido42 = false;
				recorrido52 = false;
			}

		}
	}
	else {
		rotKit2 = -90;
		rotKit_X = 0;
		movKitX2 = 0;
		movKitY2 = 0;
		movKitZ2 = 0;
		recorrido12 = true;
		recorrido22 = false;
		recorrido32 = false;
		recorrido42 = false;
		recorrido52 = false;
	}
}










// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		range += 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_2])
	{
		range -= 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_3])
	{
		range += 0.1;
		printf("El spotangle es %f\n", range);
	}

	if (keys[GLFW_KEY_4])
	{
		range -= 0.1;
		printf("El spotangle es %f\n", range);
	}


	


	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z += 0.1f;
	}
	if (keys[GLFW_KEY_G])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z -= 0.1f;
	}

	if (keys[GLFW_KEY_Y])//Animacion coraje
	{
		pointLightPositions[1].x += 0.1f;
		pointLightPositions[1].y += 0.1f;
		pointLightPositions[1].z += 0.1f;
		circuito = true;
	}

	if (keys[GLFW_KEY_H])//Apagar animacion coraje
	{
		pointLightPositions[1].x -= 0.1f;
		pointLightPositions[1].y -= 0.1f;
		pointLightPositions[1].z -= 0.1f;
		circuito = false;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[2].x += 0.1f;
		pointLightPositions[2].y += 0.1f;
		pointLightPositions[2].z += 0.1f;
		circuito2 = true;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[2].x -= 0.1f;
		pointLightPositions[2].y -= 0.1f;
		pointLightPositions[2].z -= 0.1f;
		circuito2 = false;
	}
	if (keys[GLFW_KEY_I])
	{
		pointLightPositions[3].x += 0.1f;
		pointLightPositions[3].y += 0.1f;
		pointLightPositions[3].z += 0.1f;
	}
	if (keys[GLFW_KEY_K])
	{
		pointLightPositions[3].x -= 0.1f;
		pointLightPositions[3].y -= 0.1f;
		pointLightPositions[3].z -= 0.1f;
	}

	

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}