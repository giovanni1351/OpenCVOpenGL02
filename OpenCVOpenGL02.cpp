#define _CRT_SECURE_NO_WARNINGS
#include <GL/freeglut.h> 
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>

#define LAR 700 // largula da sua janela Windows
#define ALT 700 // altura da sua janela Windows
#define dimTx 256
using namespace std;
using namespace cv;
struct T_Ponto {
	double x, y, tempo;
};
struct T_PontoX {
	double Vx, Vy, tempo;
};
struct T_PontoXx {
	double Ax, Ay, tempo;
};
vector <T_Ponto> Desloc1;
vector <T_Ponto> Desloc_Robo;
vector <T_PontoX> Veloc1;
vector <T_PontoX> Veloc_Robo;
vector <T_PontoXx> Acel1;
vector <T_PontoXx> Acel_Robo;
int Atualiza_lista_Robo = 0;
cv::Mat imXporY; //define uma tipo imagem
cv::Mat imXporT;
cv::Mat imVxporT;
cv::Mat imAxporT;
cv::Mat imYporT;
cv::Mat imVyporT;
cv::Mat imAyporT;
cv::Mat imRobXporT;
cv::Mat imRobYporT;
cv::Mat imRobXporY;
cv::Mat imRobVxporT;
cv::Mat imRobVyporT; 
cv::Mat imRobAxporT;
cv::Mat imRobAyporT;
// variÃ¡veis para o Ã¢ngulo de perspectiva e para o aspecto da projeÃ§Ã£o
// sÃ£o usadas na funÃ§Ã£o gluPerspective
//A variavel rangle Ã© alterada pelo teclado se vc clicar em 'a', jÃ¡ a variÃ¡vel fAspect Ã© alterada altomaticamente
//sempre que vc muda o aspecto da janela
int index = 0;
int selecionado = 0;
GLfloat rangle, fAspect;
static GLuint texName;
int altura = 281;
int largura = 447;
GLfloat win, r, g, b;
// variÃ¡veis que controlam o clique do mouse para operaÃ§Ãµes geomÃ©tricas
// sÃ£o atualizadas nas funÃ§Ãµes de callback de mouse (GerenciaMouse) e
// operaÃ§Ãµes geomÃ©tricas como glTranslate
GLfloat dx = 0, dy = 0, dz = 0;
double velocidadeRobo = 0.2;
int Graphic_Rob_is_open = 0;
float Pos_inicial_Roboy;
float Pos_inicial_RoboX;
GLfloat Robox_Antes;
GLfloat Roboy_Antes;
GLfloat RoboX= Pos_inicial_RoboX, RoboY= Pos_inicial_Roboy;
int pesx=-200, pesy=-200, pesz=200;
GLfloat rotx=0 ,roty=0 ,rotz=0;
GLfloat x_1, y_1, x_2, y_2;
// essas variÃ¡veis aqui sÃ£o controladas pela Callback de mouse
// servem para converter as coordenadas do Windows em coordenadas do OpenGL
GLfloat cx = 0, cy = 0;
//Lista de vectors para criar o robo 3D
uchar imPS[dimTx][dimTx][4];
double posicaoX = 0;
double posicaoY = 0;

struct Vertex {
	float x, y, z;
};
struct Texture {
	float u, v, t;
};
struct Normal {
	float nx, ny, nz;
};
struct FaceVertex {
	int vertexIndex;
	int textureIndex;
	int normalIndex;
};
struct Face {
	std::vector<FaceVertex> vertices;
};
std::vector<Vertex> vertices;
std::vector<Texture> textures;
std::vector<Normal> normals;
std::vector<Face> faces;
GLfloat rot = 0;
//variaveis para animar A BOLA
GLfloat jx=-60,jy=0,jz=0;
// funÃ§Ã£o que desenha uma malha 3D
void DesenhaMalha();
void InicializaTextura();
void CarregaTextura();
void DesenhaJOGO();
void DesenhaMalhaTexturizada();
void PosicaoBola();
void CarregaGraficos();
void XporY(double posx, double posy, GLfloat velocidade);
void ProjetaXporT(double posx, double posy, GLfloat velocidade);
void maraRob();
void projetaGraficoBola();
void ProjetaGrafico(double posx, double posy, Mat imagem, string texto, int iniciox, int inicioy, double MultiX, double MultiY);
void CriaMenu();
void LerOBJ();
void projetaGraficoRobo();
void carregaVecRobo();
void fechaGraficosRobo();
uchar imTex[dimTx][dimTx][4];
void InicializaTextura() {

    // Carrega a textura e envia para OpenGL 
    CarregaTextura();

    glGenTextures(1, &texName);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	
}
void CarregaTextura() {

	cv::Mat imTexture, imTexture256,imagemPS, imPS256; //define uma tipo imagem
	imTexture = cv::imread("campodefutebol.jpg", 1);  // lÃª uma imagem
	imagemPS = cv::imread("PSMOLDADO.png", 1);
	//cv::Mat img_Grafico; //define uma tipo imagem
	//img_Grafico = cv::imread("campodefutebol.jpg", 1);  // lÃª uma imagem

	// redimensiona a imagem carregada para 256 x 256 (textura tem que ser potÃªncia de 2)
	cv::resize(imTexture, imTexture256, cv::Size(256, 256));
	cv::resize(imagemPS, imPS256, cv::Size(256, 256));

	// cria uma janela OpenCV para exibir a imagem
	//cv::namedWindow("Teste Textura 256 x 256", 2);

	// exibe a imagem carregada e redimensionada em uma janela OpenCV
	//cv::imshow("Teste Textura 256 x 256", imTexture256);

	// for para varrer as linhas da imagem
	for (int y = 0; y < imTexture256.rows; y++)
	{
		// for para varrer as colunas da imagem
		for (int x = 0; x < imTexture256.cols; x++)
		{
			// funÃ§Ã£o que captura o valor RGB do pixel (y,x)
			cv::Vec3b cor = imTexture256.at<cv::Vec3b>(y, x);
			//esse trecho atribui Ã  matriz de textura os valores            	     //dos pixels da imagem
			imTex[y][x][0] = 255 - cor[2]; // blue
			imTex[y][x][1] = 255 - cor[1]; // green
			imTex[y][x][2] = 255 - cor[0]; // red
		}
	}

	imTexture256.release(); // desaloca a imagem
	for (int y = 0; y < imPS256.rows; y++)
	{
		// for para varrer as colunas da imagem
		for (int x = 0; x < imPS256.cols; x++)
		{
			// funÃ§Ã£o que captura o valor RGB do pixel (y,x)
			cv::Vec3b cor = imPS256.at<cv::Vec3b>(y, x);
			//esse trecho atribui Ã  matriz de textura os valores            	     //dos pixels da imagem
			imPS[y][x][0] = 255 - cor[2]; // blue
			imPS[y][x][1] = 255 - cor[1]; // green
			imPS[y][x][2] = 255 - cor[0]; // red
		}
	}

	imPS256.release(); // desaloca a imagem
};
// Essa funÃ§Ã£o, que nÃ£o Ã© callback, serve para configurar a iluminaÃ§Ã£o
// ela Ã© chamada uma Ãºnica vez na funÃ§Ã£o de Incializa, chamada pela main()
void ConfiguraIluminacao(void)
{
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 }; // definiÃ§Ã£o de luz ambiente
	GLfloat luzDifusa[4] = { 0.7,0.7,0.7,1.0 };	   // definiÃ§Ã£o de luz difusa
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };// definiÃ§Ã£o de luz especular (brilho) 
	GLfloat posicaoLuz[4] = { 0.0, 50.0, 50.0, 1.0 }; // posiÃ§Ã£o do spot de luz

	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 1.0,1.0,1.0,1.0 };
	GLint especMaterial = 60;

	// Especifica que a cor de fundo da janela serÃ¡ preta
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorizaÃ§Ã£o de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletÃ¢ncia do material 
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentraÃ§Ã£o do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parÃ¢metros da luz de nÃºmero 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// Habilita a definiÃ§Ã£o da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminaÃ§Ã£o
	glEnable(GL_LIGHTING);
	// Habilita a luz de nÃºmero 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
}
// FunÃ§Ã£o callback chamada para fazer o desenho
void Desenha(void)
{

	// pinta a cor do fundo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// define a cor dos objetos que vÃ£o ser construÃ­dos
	//glColor3f(0.0f, 0.0f, 0.0f);
	//glEnable(GL_TEXTURE_2D);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimTx, dimTx, 0, GL_RGBA, GL_UNSIGNED_BYTE, imTex);
	// Desenha o teapot com a cor corrente (wire-frame)
	DesenhaJOGO();
	//glDisable(GL_TEXTURE_2D);
	// Executa os comandos OpenGL
	glutSwapBuffers();  // troca de buffer para acelerar a cena
}
void LerOBJ() {
	std::ifstream file("RoboHumanoid.obj");

	if (!file.is_open()) {
		std::cerr << "Erro ao abrir o arquivo OBJ." << std::endl;
	}

	std::string line;
	while (std::getline(file, line)) {

		std::istringstream iss(line);
		std::string token;
		iss >> token;

		if (token == "v") {
			Vertex vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
		else if (token == "vt") {
			Texture texture;
			iss >> texture.u >> texture.v >> texture.t;
			textures.push_back(texture);
		}
		else if (token == "vn") {
			Normal normal;
			iss >> normal.nx >> normal.ny >> normal.nz;
			normals.push_back(normal);
		}
		else if (token == "f") {
			Face face;
			while (iss >> token) {
				std::istringstream indexStream(token);
				std::string vertexIndexStr, textureIndexStr, normalIndexStr;
				int vertexIndex, textureIndex, normalIndex;
				std::getline(indexStream, vertexIndexStr, '/');
				std::getline(indexStream, textureIndexStr, '/');
				std::getline(indexStream, normalIndexStr, '/');

				vertexIndex = std::stoi(vertexIndexStr); -1;
				textureIndex = textureIndexStr.empty() ? -1 : std::stoi(textureIndexStr) - 1;
				normalIndex = normalIndexStr.empty() ? -1 : std::stoi(normalIndexStr) - 1;

				FaceVertex faceVertex;
				faceVertex.vertexIndex = vertexIndex;
				faceVertex.textureIndex = textureIndex;
				faceVertex.normalIndex = normalIndex;
				face.vertices.push_back(faceVertex);
			}
			faces.push_back(face);
		}
	}

	file.close();

}
void DesenhaJOGO() {
	
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimTx, dimTx, 0, GL_RGBA, GL_UNSIGNED_BYTE, imTex);

		glColor3f(0.00f, 1.00f, 0.00f);
		glRotatef(rotx, 1, 0,0);
		glRotatef(roty, 0, 1, 0);
		glRotatef(rotz, 0, 0, 1);
		DesenhaMalhaTexturizada();
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(jx, jy, 3);
		glutSolidSphere(3.0f,200,200); 
	glPopMatrix();


	glPushMatrix();
		//glScalef(0.5, 0.5, 0.5);
		glTranslatef(RoboX, RoboY, 1);
		glRotatef(rot, 0, 0, 1);
		glBegin(GL_QUADS);
		float face3[4][3];
		float norm3[4][3];
		for (const Face& face : faces) {
			int i = 0;
			for (const FaceVertex& vertex : face.vertices) {

				face3[i][0] = vertices[vertex.vertexIndex-1].x;
				face3[i][1] = vertices[vertex.vertexIndex-1].y;
				face3[i][2] = vertices[vertex.vertexIndex-1].z;

				if (vertex.normalIndex != -1) {
					norm3[i][0] = normals[vertex.normalIndex - 1].nx;
					norm3[i][1] = normals[vertex.normalIndex - 1].ny;
					norm3[i][2] = normals[vertex.normalIndex - 1].nz;
				}
				i++;
				//if (vertex.textureIndex != -1){ vertex.textureIndex + 1;}
			}

			glColor3f(1.0, 0.0, 0.0);
			glNormal3f(norm3[0][0], norm3[0][1], norm3[0][2]);
			glVertex3f(face3[0][0], face3[0][1], face3[0][2]);

			glNormal3f(norm3[1][0], norm3[1][1], norm3[1][2]);
			glVertex3f(face3[1][0], face3[1][1], face3[1][2]);

			glNormal3f(norm3[2][0], norm3[2][1], norm3[2][2]);
			glVertex3f(face3[2][0], face3[2][1], face3[2][2]);

			glNormal3f(norm3[3][0], norm3[3][1], norm3[3][2]);
			glVertex3f(face3[3][0], face3[3][1], face3[3][2]);
		}
		glEnd();
	glPopMatrix();

	/*
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimTx, dimTx, 0, GL_RGBA, GL_UNSIGNED_BYTE, imPS);
		glColor3f(1, 1, 1);
		glRotatef(-90, 1, 0, 0);
		glRotatef(180, 1, 0, 0);
		glTranslatef(0, 45, -90);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex3f(-60, -45, 50);
		glTexCoord2f(1.0, 1.0); glVertex3f(60, -45, 50);
		glTexCoord2f(1.0, 0.0); glVertex3f(60, 45, 50);
		glTexCoord2f(0.0, 0.0); glVertex3f(-60, 45, 50);
		glEnd();
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	*/

	

};
void DesenhaMalha()
{
	//glutWireTeapot(50.0f);     // constrÃ³i uma malha Wire 
	//glutSolidTeapot(50.0f);     // constrÃ³i uma malha Solida 

	// esta macro desenha um simples quadrado
	glBegin(GL_QUADS);
	glVertex3f(-64, -64, 0);
	glVertex3f(64, -64, -50);
	glVertex3f(64, 64, -50);
	glVertex3f(-64, 64, 50);
	glEnd();
}
void DesenhaMalhaTexturizada() {

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-60, -45, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(60, -45, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(60, 45, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-60, 45, 0);
	glEnd();


}
void CarregaGraficos() {
	imXporY = cv::imread("XporTempo.jpg", 1);  // le uma imagem
	imXporT = cv::imread("XporTempo.jpg", 1);  // le uma imagem
	imVxporT = cv::imread("XporTempo.jpg", 1);  // le uma imagem
	imAxporT = cv::imread("XporTempo.jpg", 1);  // le uma imagem
	imYporT = cv::imread("XporTempo.jpg", 1);  // le uma imagem
	imVyporT = cv::imread("XporTempo.jpg", 1);  // le uma imagem
	imAyporT = cv::imread("XporTempo.jpg", 1);  // le uma imagem
	imRobXporT = cv::imread("XporTempo.jpg", 1); // le uma imagem
	imRobYporT = cv::imread("XporTempo.jpg", 1); // le uma imagem
	imRobXporY = cv::imread("XporTempo.jpg", 1); // le uma imagem
	imRobVxporT = cv::imread("XporTempo.jpg", 1); // le uma imagem
	imRobVyporT = cv::imread("XporTempo.jpg", 1); // le uma imagem
	imRobAxporT = cv::imread("XporTempo.jpg", 1); // le uma imagem
	imRobAyporT = cv::imread("XporTempo.jpg", 1); // le uma imagem
};
void projetaGraficoBola() {
	x_1 = Desloc1[0].x;
	y_1 = Desloc1[0].y;
	x_2 = Desloc1[0].x;
	y_2 = Desloc1[0].y;
	for (const T_Ponto& i : Desloc1) {
		ProjetaGrafico(i.x, i.y, imXporY, "Grafico X por Y", largura, altura,8,8);
	}
	x_1 = Desloc1[0].tempo;
	y_1 = Desloc1[0].x;
	x_2 = Desloc1[0].tempo;
	y_2 = Desloc1[0].x;
	for (const T_Ponto& i : Desloc1) {
		ProjetaGrafico(i.tempo, i.x, imXporT, "Grafico X por T", largura, altura,20,5);
	}
	x_1 = Desloc1[0].tempo;
	y_1 = Desloc1[0].y;
	x_2 = Desloc1[0].tempo;
	y_2 = Desloc1[0].y;
	for (const T_Ponto& i : Desloc1) {
		ProjetaGrafico(i.tempo, i.y, imYporT,"Grafico Y por T", largura, altura,20,15);
	}
	x_1 = Veloc1[0].tempo;
	y_1 = Veloc1[0].Vx;
	x_2 = Veloc1[0].tempo;
	y_2 = Veloc1[0].Vx;
	for (const T_PontoX& i : Veloc1) {
		ProjetaGrafico(i.tempo, i.Vx, imVxporT, "Grafico Vx por T", largura, altura,15,15);
	}

	x_1 = Veloc1[0].tempo;
	y_1 = Veloc1[0].Vy;
	x_2 = Veloc1[0].tempo;
	y_2 = Veloc1[0].Vy;
	for (const T_PontoX& i : Veloc1) {
		ProjetaGrafico(i.tempo, i.Vy, imVyporT, "Grafico Vy por T", largura, altura,15,15);
	}
	x_1 = Acel1[0].tempo;
	y_1 = Acel1[0].Ay;
	x_2 = Acel1[0].tempo;
	y_2 = Acel1[0].Ay;
	for (const T_PontoXx& i : Acel1) {
		ProjetaGrafico(i.tempo, i.Ay, imAyporT, "Grafico Ay por T", largura, altura,15,15);
	}
	x_1 = Acel1[0].tempo;
	y_1 = Acel1[0].Ax;
	x_2 = Acel1[0].tempo;
	y_2 = Acel1[0].Ax;
	for (const T_PontoXx& i : Acel1) {
		ProjetaGrafico(i.tempo, i.Ax, imAxporT, "Grafico Ax por T", largura, altura,15,15);
	}
};
void carregaVecRobo() {

	for (int x = 1; x < Desloc_Robo.size(); x++) {
		T_PontoX atual;

		atual.tempo = Desloc_Robo[x].tempo;
		atual.Vx = (Desloc_Robo[x].x - Desloc_Robo[x - 1].x) / (Desloc_Robo[x].tempo - Desloc_Robo[x - 1].tempo);
		atual.Vy = (Desloc_Robo[x].y - Desloc_Robo[x - 1].y) / (Desloc_Robo[x].tempo - Desloc_Robo[x - 1].tempo);
		Veloc_Robo.push_back(atual);
	}
	for (int x = 1; x < Veloc_Robo.size(); x++) {
		T_PontoXx atual;

		atual.tempo = Veloc1[x].tempo;
		atual.Ax = (Veloc_Robo[x].Vx - Veloc_Robo[x - 1].Vx) / (Veloc_Robo[x].tempo - Veloc_Robo[x - 1].tempo);
		atual.Ay = (Veloc_Robo[x].Vy - Veloc_Robo[x - 1].Vy) / (Veloc_Robo[x].tempo - Veloc_Robo[x - 1].tempo);
		Acel_Robo.push_back(atual);
	}
}
void projetaGraficoRobo() {
	x_1 = Desloc_Robo[0].x;
	y_1 = Desloc_Robo[0].y;
	x_2 = Desloc_Robo[0].x;
	y_2 = Desloc_Robo[0].y;
	Desloc_Robo.pop_back();
	for (const T_Ponto& i : Desloc_Robo) {
		ProjetaGrafico(i.x , i.y, imRobXporY, "Grafico Robo X por Y", largura, altura, 5, 10);
	}
	x_1 = Veloc_Robo[0].tempo;
	y_1 = Veloc_Robo[0].Vx;
	x_2 = Veloc_Robo[0].tempo;
	y_2 = Veloc_Robo[0].Vx;
	Veloc_Robo.pop_back();

	for (const T_PontoX& i : Veloc_Robo) {
		ProjetaGrafico(i.tempo , i.Vx , imRobVxporT, "Grafico Robo Vx por Tempo", largura, altura, 30, 1);
	}
	x_1 = Veloc_Robo[0].tempo;
	y_1 = Veloc_Robo[0].Vy;
	x_2 = Veloc_Robo[0].tempo;
	y_2 = Veloc_Robo[0].Vy;
	for (const T_PontoX& i : Veloc_Robo) {
		ProjetaGrafico(i.tempo , i.Vy , imRobVyporT, "Grafico Robo Vy por Tempo", largura, altura, 30, 1);
	}
	Acel_Robo.pop_back();
	x_1 = Acel_Robo[0].tempo;
	y_1 = Acel_Robo[0].Ax;
	x_2 = Acel_Robo[0].tempo;
	y_2 = Acel_Robo[0].Ax;

	for (const T_PontoXx& i : Acel_Robo) {
		ProjetaGrafico(i.tempo , i.Ax , imRobAxporT, "Grafico Robo Ax por Tempo", largura, altura, 30, 1);
	}
	x_1 = Acel_Robo[0].tempo;
	y_1 = Acel_Robo[0].Ay;
	x_2 = Acel_Robo[0].tempo;
	y_2 = Acel_Robo[0].Ay;
	for (const T_PontoXx& i : Acel_Robo) {
		ProjetaGrafico(i.tempo , i.Ay , imRobAyporT, "Grafico Robo Ay por Tempo", largura, altura, 30, 1/10.0);
	}
}
void fechaGraficosRobo() {
	destroyWindow("Grafico Robo X por Y");
	destroyWindow("Grafico Robo Vx por Tempo");
	destroyWindow("Grafico Robo Vy por Tempo");
	destroyWindow("Grafico Robo Ax por Tempo");
	destroyWindow("Grafico Robo Ay por Tempo");
};

void ProjetaGrafico(double posx, double posy, Mat imagem, string texto, int iniciox, int inicioy,double MultiX,double MultiY) {

	x_1 = x_2;
	y_1 = y_2;
	x_2 = posx;
	y_2 = posy;

	// Defina os pontos inicial e final da linha
	cv::Point startPoint(iniciox + x_1 * MultiX, inicioy - y_1 * MultiY); // (x1, y1)
	cv::Point endPoint(iniciox + x_2 * MultiX, inicioy - y_2 * MultiY);   // (x2, y2)

	// Defina a cor da linha (B, G, R)
	cv::Scalar lineColor(0, 0, 255); // Vermelho (BGR)


	// Desenhe a linha na imagem
	cv::line(imagem, startPoint, endPoint, lineColor, 2); // 2 é a espessura da linha
	cv::Point posicao(50, 50); // Posição do texto na imagem
	int fonteFace = cv::FONT_HERSHEY_SIMPLEX; // Tipo de fonte
	double escala = 1.0; // Escala da fonte
	cv::Scalar cor(0, 0, 255); // Cor do texto (no formato BGR)
	int espessura = 1; // Espessura da linha do texto
	int tipoLinha = cv::LINE_AA; // Tipo de linha (Anti-aliased)

	// Escreva o texto na imagem
	cv::putText(imagem, texto, posicao, fonteFace, escala, cor, espessura, tipoLinha);

	namedWindow(texto, 2);
	cv::imshow(texto, imagem);
}
void PosicaoBola() {
	string linha;
	ifstream F_PosBola("posicao.txt");

	while (getline(F_PosBola, linha)) {
		//cout << "entrou" << endl;
		//cout << linha << endl;
		istringstream linhaStream(linha);
		double tempo, x, y;
		if (linhaStream >> tempo >> x >> y) {
			T_Ponto atual;
			atual.x = x;
			atual.y = y;
			atual.tempo = tempo;
			Desloc1.push_back(atual);
		}
	}
	cout << "Dados da bola carregados!" << endl;
	for (int x = 1; x < Desloc1.size(); x++) {
		T_PontoX atual;

		atual.tempo = Desloc1[x].tempo;
		atual.Vx = (Desloc1[x].x - Desloc1[x - 1].x) /( Desloc1[x].tempo - Desloc1[x - 1].tempo);
		atual.Vy = (Desloc1[x].y - Desloc1[x - 1].y) / (Desloc1[x].tempo - Desloc1[x - 1].tempo);
		Veloc1.push_back(atual);
	}
	for (int x = 1; x < Veloc1.size(); x++) {
		T_PontoXx atual;

		atual.tempo = Veloc1[x].tempo;
		atual.Ax = (Veloc1[x].Vx - Veloc1[x - 1].Vx) / (Veloc1[x].tempo - Veloc1[x - 1].tempo);
		atual.Ay = (Veloc1[x].Vy - Veloc1[x - 1].Vy) / (Veloc1[x].tempo - Veloc1[x - 1].tempo);
		Acel1.push_back(atual);
	}
}	  
// Inicializa parÃ¢metros iniciais, Ã© chamada na main()
void Inicializa(void)
{

	InicializaTextura();
	LerOBJ();
	//CarregaTextura();
	CarregaGraficos();
	PosicaoBola();
	projetaGraficoBola();


	// configura iluminaÃ§Ã£o
	ConfiguraIluminacao();

	// define a cor de fundo
	glClearColor(1.0f,1.0f, 1.0f, 0.0f);

	// define o Ã¢ngulo inicial de perspectiva, na funÃ§Ã£o gluPerspective()
	//Essa variÃ¡vel Ã© alterada pelo teclado se vc clicar em 'a'
	rangle = 45;
}
// FunÃ§Ã£o usada para especificar o volume de visualizaÃ§Ã£o
// se vc alterou algum parÃ¢metro de visualizaÃ§Ã£o, essa funÃ§Ã o Ã© chamada
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeÃ§Ã£o
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeÃ§Ã£o
	glLoadIdentity();

	// Especifica a projeÃ§Ã£o perspectiva
	gluPerspective(rangle, fAspect, 0.1, 1000);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posiÃ§Ã£o do observador e do alvo
	gluLookAt(pesx,pesy, pesz, 0, 0, 0, 1, 1, 0);

}
// FunÃ§Ã£o callback chamada quando o tamanho da janela Ã© alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisÃ£o por zero
	if (h == 0) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);

	// Calcula a correÃ§Ã£o de aspecto
	fAspect = (GLfloat)w / (GLfloat)h;

	// se vc alterou algum parÃ¢metro de visualizaÃ§Ã£o, essa funÃ§Ã o Ã© chamada
	EspecificaParametrosVisualizacao();
}
// calback de teclado
void Teclado(unsigned char key, int x, int y) {

	switch (key) {

	case 'k': dx = dx + 10; break;  //desloca para a direita

	case 'l': dx = dx - 10; break;  //desloca para a esquerda

	case 'a':                      // aumenta o Ã¢ngulo de visualizaÃ§Ã£o da funÃ§Ã£o gluPerspective(), provocando ZOO-IN
		rangle = rangle + 5;
		if (rangle > 360) rangle = 0;
		break;

	case 'b':                      // diminui o Ã¢ngulo de visualizaÃ§Ã£o da funÃ§Ã£o gluPerspective(), provocando ZOO-OUT
		rangle = rangle - 5;
		if (rangle < 0) rangle = 0;
		break;

	case '1':                      // diminui o Ã¢ngulo de visualizaÃ§Ã£o da funÃ§Ã£o gluPerspective(), provocando ZOO-OUT
		pesx  -= 5;
		if (pesx < -500) {
			pesx = -500;
		}
		break;

	case '2':                      // diminui o Ã¢ngulo de visualizaÃ§Ã£o da funÃ§Ã£o gluPerspective(), provocando ZOO-OUT
		pesx  += 5;
		if (pesx > 500) {
			pesx = 500;
		}
		break;
	case '3':                      // diminui o Ã¢ngulo de visualizaÃ§Ã£o da funÃ§Ã£o gluPerspective(), provocando ZOO-OUT
		pesy  -= 5;
		if (pesy < -500) {
			pesy = -500;
		}
		break;
	case '4':                      // diminui o Ã¢ngulo de visualizaÃ§Ã£o da funÃ§Ã£o gluPerspective(), provocando ZOO-OUT
		pesy += 5;
		if (pesy > 500) {
			pesy = 500;
		}
		break;

	case '5':                      // diminui o Ã¢ngulo de visualizaÃ§Ã£o da funÃ§Ã£o gluPerspective(), provocando ZOO-OUT
		pesz -= 5;
		if (pesz < -500) {
			pesz = -500;
		}
		break;

	case '6':                      // diminui o Ã¢ngulo de visualizaÃ§Ã£o da funÃ§Ã£o gluPerspective(), provocando ZOO-OUT
		pesz += 5;
		if (pesz > 500) {
			pesz = 500;
		}
		break;
	}

	// se vc alterou algum parÃ¢metro de visualizaÃ§Ã£o, essa funÃ§Ã o Ã© chamada
	EspecificaParametrosVisualizacao();

	// dispara a callback de Desenha() para desenhar o que alterou!
	glutPostRedisplay();
}
// Gerenciamento do menu com as opções de cores           
void MenuCor(int op)
{
	switch (op) {
	case 0:
		Pos_inicial_RoboX = 60;
		Pos_inicial_Roboy = 0;
		Atualiza_lista_Robo = 1;
		selecionado = 1;
		break;
	case 1:
		Pos_inicial_RoboX = 60;
		Pos_inicial_Roboy = -45;
		Atualiza_lista_Robo = 1;
		selecionado = 1;

		break;
	case 2:
		Pos_inicial_RoboX = 60;
		Pos_inicial_Roboy = 45;
		selecionado = 1;

		Atualiza_lista_Robo = 1;
		break;
	case 3:
		Pos_inicial_RoboX = -60;
		Pos_inicial_Roboy = 45;
		selecionado = 1;

		Atualiza_lista_Robo = 1;
		break;

	case 4:
		Pos_inicial_RoboX = -60;
		Pos_inicial_Roboy = -45;
		Atualiza_lista_Robo = 1;
		selecionado = 1;

		break;

	case 5:
		Pos_inicial_RoboX = 0;
		Pos_inicial_Roboy = 0;
		Atualiza_lista_Robo = 1;
		selecionado = 1;

		break;

	case 6:
		Pos_inicial_RoboX = -60;
		Pos_inicial_Roboy = 0;
		Atualiza_lista_Robo = 1;
		selecionado = 1;

		break;
			

	
	}
	glutPostRedisplay();
}
// Gerenciamento do menu com as opções de cores           
void MenuPrimitiva(int op)
{
	switch (op) {
	case 0:
		//primitiva = QUADRADO;
		glClearColor(1, 0, 0, 1);
		break;
	case 1:
		glClearColor(0, 1, 0, 1);
		//primitiva = TRIANGULO;
		break;
	case 2:
		glClearColor(0, 0, 1, 1);
		//primitiva = LOSANGO;
		break;
	case 3:
		glClearColor(0, 0, 0, 1);
		break;
	case 4:
		glClearColor(1, 1, 1, 1);
		break;

	}
	glutPostRedisplay();
}
// Gerenciamento do menu principal           
void MenuPrincipal(int op)
{
	if (op == 1) {
		Pos_inicial_RoboX = posicaoX;
		Pos_inicial_Roboy = posicaoY;
		Atualiza_lista_Robo = 1;
		selecionado = 1;
	}
}
void MenuPosicao(int op) {
	
}
void MenuEmX(int op) {}
void MenuEmY(int op) {}
void m_X0(int op){ posicaoX = ((0.0 + op) * 13.33333) - 60; }
void m_X1(int op){ posicaoX = ((1.0 + op) * 13.33333) - 60; }
void m_X2(int op){ posicaoX = ((2.0 + op) * 13.33333) - 60; }
void m_X3(int op){ posicaoX = ((3.0 + op) * 13.33333) - 60; }
void m_X4(int op){ posicaoX = ((4.0 + op) * 13.33333) - 60; }
void m_X5(int op){ posicaoX = ((5.0 + op) * 13.33333) - 60; }
void m_X6(int op){ posicaoX = ((6.0 + op) * 13.33333) - 60; }
void m_X7(int op){ posicaoX = ((7.0 + op) * 13.33333) - 60; }
void m_X8(int op){ posicaoX = ((8.0 + op) * 13.33333) - 60; }
void m_X9(int op){ posicaoX = ((9.0 + op) * 13.33333) - 60; }
void m_Y0(int op) { posicaoY = ((0.0 + op) * 15) - 45; }
void m_Y1(int op) { posicaoY = ((1.0 + op) * 15) - 45; }
void m_Y2(int op) { posicaoY = ((2.0 + op) * 15) - 45; }
void m_Y3(int op) { posicaoY = ((3.0 + op) * 15) - 45; }
void m_Y4(int op) { posicaoY = ((4.0 + op) * 15) - 45; }
void m_Y5(int op) { posicaoY = ((5.0 + op) * 15) - 45; }
void m_Y6(int op) { posicaoY = ((6.0 + op) * 15) - 45; }

// Criacao do Menu
void CriaMenu()
{
	int menu, submenu1, submenu2, MenuPosica,menuX,menuY;
	int mX0, mX1, mX2, mX3;
	int mX4 , mX5, mX6, mX7;
	int mX8,mX9;
	int mY0, mY1, mY2, mY3, mY4, mY5, mY6;
	submenu1 = glutCreateMenu(MenuCor);

	glutAddMenuEntry("¨Gol Adversario", 0);
	glutAddMenuEntry("¨Escanteio 1", 1);
	glutAddMenuEntry("¨Escanteio 2", 2);
	glutAddMenuEntry("¨Escanteio 3", 3);
	glutAddMenuEntry("¨Escanteio 4", 4);
	glutAddMenuEntry("Meio do Campo", 5);
	glutAddMenuEntry("Gol aliado", 6);

	submenu2 = glutCreateMenu(MenuPrimitiva);
	
	glutAddMenuEntry("Vermelho", 0);
	glutAddMenuEntry("Verde", 1);
	glutAddMenuEntry("Azul", 2);
	glutAddMenuEntry("Preto", 3);
	glutAddMenuEntry("Branco", 4);

	mX0 = glutCreateMenu(m_X0);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);

	mX1 = glutCreateMenu(m_X1);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mX2 = glutCreateMenu(m_X2);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mX3 = glutCreateMenu(m_X3);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mX4 = glutCreateMenu(m_X4);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mX5 = glutCreateMenu(m_X5);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mX6 = glutCreateMenu(m_X6);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mX7 = glutCreateMenu(m_X7);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mX8 = glutCreateMenu(m_X8);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mX9 = glutCreateMenu(m_X9);
	glutAddMenuEntry(".0", 0);

	mY0 = glutCreateMenu(m_Y0);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mY1 = glutCreateMenu(m_Y1);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mY2 = glutCreateMenu(m_Y2);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mY3 = glutCreateMenu(m_Y3);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mY4 = glutCreateMenu(m_Y4);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mY5 = glutCreateMenu(m_Y5);
	glutAddMenuEntry(".0", 0);
	glutAddMenuEntry(".1", 0.1);
	glutAddMenuEntry(".2", 0.2);
	glutAddMenuEntry(".3", 0.3);
	glutAddMenuEntry(".4", 0.4);
	glutAddMenuEntry(".5", 0.5);
	glutAddMenuEntry(".6", 0.6);
	glutAddMenuEntry(".7", 0.7);
	glutAddMenuEntry(".8", 0.8);
	glutAddMenuEntry(".9", 0.9);
	mY6 = glutCreateMenu(m_Y6);
	glutAddMenuEntry(".0", 0);


	menuX = glutCreateMenu(MenuEmX);
	glutAddSubMenu("0", mX0);
	glutAddSubMenu("1", mX1);
	glutAddSubMenu("2", mX2);
	glutAddSubMenu("3", mX3);
	glutAddSubMenu("4", mX4);
	glutAddSubMenu("5", mX5);
	glutAddSubMenu("6", mX6);
	glutAddSubMenu("7", mX7);
	glutAddSubMenu("8", mX8);
	glutAddSubMenu("9", mX9);
	menuY = glutCreateMenu(MenuEmY);
	glutAddSubMenu("0", mY0);
	glutAddSubMenu("1", mY1);
	glutAddSubMenu("2", mY2);
	glutAddSubMenu("3", mY3);
	glutAddSubMenu("4", mY4);
	glutAddSubMenu("5", mY5);
	glutAddSubMenu("6", mY6);
	MenuPosica = glutCreateMenu(MenuPosicao);
	glutAddSubMenu("X", menuX);
	glutAddSubMenu("Y", menuY);


	menu = glutCreateMenu(MenuPrincipal);
	glutAddSubMenu("Posicao Robo", MenuPosica);
	glutAddMenuEntry("Entra", 1);
	glutAddSubMenu("Cores", submenu2);
	glutAddSubMenu("Posicoes", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// FunÃ§Ã£o callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	// aqui tranaforma o x do windows no cx do OpenGL
	cx = x - (LAR / 2);

	// aqui tranaforma o y invertido do windows no cy do OpenGL
	cy = (ALT / 2) - y;

	/*	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {
		// mude uma variÃ¡vel controlada pelo mouse aqui
	}*/

	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {
			CriaMenu();
		}

// se vc alterou algum parÃ¢metro de visualizaÃ§Ã£o, essa funÃ§Ã o Ã© chamada
	EspecificaParametrosVisualizacao();

	// dispara a funÃ§Ã£o callback de Desenho(), caso vc tenha alterado alguma coisa no visual
	glutPostRedisplay();
}
void timer(int val){
	jx = ((Desloc1[index].x)* 13.33)-60;
	jy = (Desloc1[index].y* 15)-45;
	Robox_Antes = RoboX;
	Roboy_Antes = RoboY;
	if (Desloc1[index].x > 9||Desloc1[index].y >6) { index = 0; RoboX = Pos_inicial_RoboX; RoboY = Pos_inicial_Roboy; }
	//projetaVelocidade(Desloc1[index].x, Desloc1[index].y, Desloc1[index].tempo);
	if (jx - RoboX < 0) {
		RoboX -= velocidadeRobo;
		//RoboX -= Veloc1[index].Vx/4;
		
	}
	else {
		RoboX += velocidadeRobo;
		//RoboX += Veloc1[index].Vx/4;

	}
	if (jy - RoboY< 0) {
		RoboY -= velocidadeRobo;
		//RoboY -= Veloc1[index].Vy/4;

	}
	else {
		RoboY += velocidadeRobo;
		//RoboY += Veloc1[index].Vy/4;

	}
	//rot = ((atan2(Roboy_Antes - RoboY, Robox_Antes - RoboX) * 180) / 3.14159265359) - 90;
	rot = ((atan2(jy - RoboY, jx - RoboX) * 180) / 3.14159265359) + 90;
	if (Atualiza_lista_Robo == 2) {
		T_Ponto atualRobo;
		atualRobo.tempo = Desloc1[index].tempo;
		atualRobo.x = RoboX;
		atualRobo.y = RoboY;
		Desloc_Robo.push_back(atualRobo);
		cout << atualRobo.x << " " << atualRobo.y << " " << atualRobo.tempo << endl;
	}
	if (hypot(RoboX - jx, RoboY -jy)-9 < 0) {
		index = 0; RoboX = Pos_inicial_RoboX; RoboY = Pos_inicial_Roboy; 
		if(selecionado == 1)Atualiza_lista_Robo++;
		
	}
	if (Atualiza_lista_Robo == 3) {
		if (Graphic_Rob_is_open == 0) {

			carregaVecRobo();
			projetaGraficoRobo();
			Desloc_Robo.clear();
			Acel_Robo.clear();
			Veloc_Robo.clear();
			Graphic_Rob_is_open = 1;
			Atualiza_lista_Robo++; 
			selecionado = 0;
		}
		else {
			fechaGraficosRobo();
			carregaVecRobo();
			projetaGraficoRobo();
			Graphic_Rob_is_open = 1;
			Atualiza_lista_Robo++;
			selecionado = 0;
		}
	}
	glutTimerFunc(20, timer, 1);
	glutPostRedisplay();
	index++;
}
// Programa Principal
int main(int argc, char* argv[])
{	
	// essa funÃ§Ã£o inicializa a glut
	glutInit(&argc, argv);

	// ess funÃ§Ã£o inicializa o Display com um buffer RGB duplo
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// essa funÃ§Ã£o define as dimenÃ§Ãµes de sua janela Windows
	glutInitWindowSize(LAR, ALT);


	//aqui vc cria uma Janela Windows com as dimenÃ§Ãµes definidas acima
	glutCreateWindow("Visualizacao 3D");

	// aqui vc ativa uma callback de Desenho
	glutDisplayFunc(Desenha);

	// aqui vc ativa uma callBack para ajustar a janela de saÃ­da toda vez que vc alterar ela
	glutReshapeFunc(AlteraTamanhoJanela);

	// aqui vc ativa uma callbcak de mouse toda vez que vc clica nele
	glutMouseFunc(GerenciaMouse);

	// aqui vc ativa uma callback de teclado toda vez que vc clica nele
	glutKeyboardFunc(Teclado);

	glutTimerFunc(0, timer, 0);

	// aqui vc inicializa qualquer coisa fixa (nÃ o Ã© callback)
	Inicializa();

	// quando tudo estÃ¡ pronto, aqui dispara tudo
	glutMainLoop();
}