/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 12/05/2023
 * Adaptado por Luiz Felipe Giacobbo para a Tarefa - Instanciando objetos na cena do M�dulo 2 de Computa��o Gr�fica
 * Utilizei "<" e ">" para diminuir e aumentar a escala, respectivamente 
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <vector>// Para usar a classe vector para atender a parte "Instanciar mais de um cubo na cena" da Tarefa - Instanciando objetos na cena 3D - Luiz Felipe Giacobbo
using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f); // Inicialmente, n�o h� transla��o
float scale = 1.0f; // Escala inicial � 1 (sem escala)

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupShader();
int setupGeometry();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;

// C�digo fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 model;\n"
"out vec4 finalColor;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de c�digo aqui!
"gl_Position = model * vec4(position, 1.0);\n"
"finalColor = vec4(color, 1.0);\n"
"}\0";

//C�difo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 450\n"
"in vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

bool rotateX=false, rotateY=false, rotateZ=false;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola 3D -- Luiz Felipe Giacobbo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();


	glUseProgram(shaderID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	//
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);

	// Modifica��o 3 para Tarefa - Instanciando objetos na cena - Luiz Felipe Giacobbo:
	// Instanciar mais de um cubo na cena 
	std::vector<glm::vec3> cubePositions = {
	glm::vec3(0.0f,  0.0f,  0.0f), // Primeiro cubo (central)
	glm::vec3(-0.6f,  0.0f,  0.0f), // Segundo cubo (� esquerda)
	glm::vec3(0.6f,  0.0f,  0.0f)  // Terceiro cubo (� direita)
	};
	

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < cubePositions.size(); ++i) {
			glm::vec3 cubePos = cubePositions[i];

			model = glm::mat4(1);
			// Adicionando transla��o e escala � matriz model
			model = glm::translate(model, cubePos + translation);
			model = glm::scale(model, glm::vec3(scale, scale, scale));

			if (rotateX)
			{
				model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			}
			else if (rotateY)
			{
				model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else if (rotateZ)
			{
				model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
			}

			glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36); // Desenha o cubo
		}
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}


// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
// Modifica��o 2 para Tarefa - Instanciando objetos na cena 3D - Luiz Felipe Giacobbo:
// "No projeto de base, ao pressionar as teclas x, y e z, a pir�mide rotaciona nos respectivos eixos. 
// Adicione controle via teclado para:(...)"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	// "Mover (transladar) o cubo nos 3 eixos (sugest�o de teclas WASD para os eixos x e z, IJ para o eixo y)"
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		translation.z -= 0.1f;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		translation.z += 0.1f;
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		translation.x -= 0.1f;
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		translation.x += 0.1f;
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
		translation.y += 0.1f;
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
		translation.y -= 0.1f;

	// "Promover a escala uniforme do cubo (sugest�o de teclas [ para diminuir e ] para aumentar)"
	// Utilizei "<" e ">" para diminuir e aumentar a escala, respectivamente.
	if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
		scale *= 0.9f; // diminui a escala
	if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS)
		scale *= 1.1f; // aumenta a escala
}

//Esta fun��o est� basntante hardcoded - objetivo � compilar e "buildar" um programa de
// shader simples e �nico neste exemplo de c�digo
// O c�digo fonte do vertex e fragment shader est� nos arrays vertexShaderSource e
// fragmentShader source no ini�io deste arquivo
// A fun��o retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compila��o (exibi��o via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compila��o (exibi��o via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	// Modifica��o 1 para Tarefa - Instanciando objetos na cena - Luiz Felipe Giacobbo:
	// "Alterar a geometria da pir�mide, transformando-a em um cubo (adicionar os v�rtices e tri�ngulos necess�rios). 
	// Sugere-se fazer cada lado do cubo (composto de 2 tri�ngulos, similar � base da pir�mide) de uma cor diferente, 
	// para que facilite nossa visualiza��o neste momento que ainda n�o utilizamos texturas e ilumina��o adequada."
	GLfloat scale = 1.0f / 3.0f; // Fator de escala para reduzir o tamanho do cubo em 1/3, para aparecerem os 3 na tela

	GLfloat vertices[] = {
		// Base do cubo (face inferior)
		-0.5f * scale, -0.5f * scale, -0.5f * scale, 1.0f, 0.0f, 0.0f,  // v�rtice 0
		 0.5f * scale, -0.5f * scale, -0.5f * scale, 1.0f, 0.0f, 0.0f,  // v�rtice 1
		 0.5f * scale, -0.5f * scale,  0.5f * scale, 1.0f, 0.0f, 0.0f,  // v�rtice 2
		-0.5f * scale, -0.5f * scale, -0.5f * scale, 1.0f, 0.0f, 0.0f,  // v�rtice 0
		 0.5f * scale, -0.5f * scale,  0.5f * scale, 1.0f, 0.0f, 0.0f,  // v�rtice 2
		-0.5f * scale, -0.5f * scale,  0.5f * scale, 1.0f, 0.0f, 0.0f,  // v�rtice 3

		// Topo do cubo (face superior)
		-0.5f * scale,  0.5f * scale, -0.5f * scale, 0.0f, 1.0f, 0.0f,  // v�rtice 4
		 0.5f * scale,  0.5f * scale, -0.5f * scale, 0.0f, 1.0f, 0.0f,  // v�rtice 5
		 0.5f * scale,  0.5f * scale,  0.5f * scale, 0.0f, 1.0f, 0.0f,  // v�rtice 6
		-0.5f * scale,  0.5f * scale, -0.5f * scale, 0.0f, 1.0f, 0.0f,  // v�rtice 4
		 0.5f * scale,  0.5f * scale,  0.5f * scale, 0.0f, 1.0f, 0.0f,  // v�rtice 6
		-0.5f * scale,  0.5f * scale,  0.5f * scale, 0.0f, 1.0f, 0.0f,  // v�rtice 7

		// Face dianteira
		-0.5f * scale, -0.5f * scale,  0.5f * scale, 0.0f, 0.0f, 1.0f,  // v�rtice 3
		 0.5f * scale, -0.5f * scale,  0.5f * scale, 0.0f, 0.0f, 1.0f,  // v�rtice 2
		 0.5f * scale,  0.5f * scale,  0.5f * scale, 0.0f, 0.0f, 1.0f,  // v�rtice 6
		-0.5f * scale, -0.5f * scale,  0.5f * scale, 0.0f, 0.0f, 1.0f,  // v�rtice 3
		 0.5f * scale,  0.5f * scale,  0.5f * scale, 0.0f, 0.0f, 1.0f,  // v�rtice 6
		-0.5f * scale,  0.5f * scale,  0.5f * scale, 0.0f, 0.0f, 1.0f,  // v�rtice 7

		// Face traseira
		-0.5f * scale, -0.5f * scale, -0.5f * scale, 1.0f, 1.0f, 0.0f,  // v�rtice 0
		 0.5f * scale, -0.5f * scale, -0.5f * scale, 1.0f, 1.0f, 0.0f,  // v�rtice 1
		 0.5f * scale,  0.5f * scale, -0.5f * scale, 1.0f, 1.0f, 0.0f,  // v�rtice 5
		-0.5f * scale, -0.5f * scale, -0.5f * scale, 1.0f, 1.0f, 0.0f,  // v�rtice 0
		 0.5f * scale,  0.5f * scale, -0.5f * scale, 1.0f, 1.0f, 0.0f,  // v�rtice 5
		-0.5f * scale,  0.5f * scale, -0.5f * scale, 1.0f, 1.0f, 0.0f,  // v�rtice 4

		// Face esquerda
		-0.5f * scale, -0.5f * scale, -0.5f * scale, 0.0f, 1.0f, 1.0f,  // v�rtice 0
		-0.5f * scale, -0.5f * scale,  0.5f * scale, 0.0f, 1.0f, 1.0f,  // v�rtice 3
		-0.5f * scale,  0.5f * scale,  0.5f * scale, 0.0f, 1.0f, 1.0f,  // v�rtice 7
		-0.5f * scale, -0.5f * scale, -0.5f * scale, 0.0f, 1.0f, 1.0f,  // v�rtice 0
		-0.5f * scale,  0.5f * scale,  0.5f * scale, 0.0f, 1.0f, 1.0f,  // v�rtice 7
		-0.5f * scale,  0.5f * scale, -0.5f * scale, 0.0f, 1.0f, 1.0f,  // v�rtice 4

		// Face direita
		 0.5f * scale, -0.5f * scale, -0.5f * scale, 1.0f, 0.0f, 1.0f,  // v�rtice 1
		 0.5f * scale, -0.5f * scale,  0.5f * scale, 1.0f, 0.0f, 1.0f,  // v�rtice 2
		 0.5f * scale,  0.5f * scale,  0.5f * scale, 1.0f, 0.0f, 1.0f,  // v�rtice 6
		 0.5f * scale, -0.5f * scale, -0.5f * scale, 1.0f, 0.0f, 1.0f,  // v�rtice 1
		 0.5f * scale,  0.5f * scale,  0.5f * scale, 1.0f, 0.0f, 1.0f,  // v�rtice 6
		 0.5f * scale,  0.5f * scale, -0.5f * scale, 1.0f, 0.0f, 1.0f,  // v�rtice 5
	};



	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);



	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

