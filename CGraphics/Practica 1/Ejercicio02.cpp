#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ROTATE 0
#define RENDER_SPHERES_INSTEAD_OF_VERTICES 0

using namespace std;

GLuint  prog_hdlr;
GLint location_attribute_0, location_viewport;
static int winwidth,winheight;
static int mx,my;
static int flag=0;
static float rotx=0.0f, roty=0.0f;	


GLuint VAOs[2];	
GLuint VBOs[2];	


// For glut
const int SCREEN_WIDTH  = 1024;
const int SCREEN_HEIGHT = 1024;

const float camera[]           = {.6,0,1};
const float light0_position[4] = {1,1,1,0};


float angle = 0.72;


unsigned int VBO, VAO;

float rand_minus_one_one() {
	return (float)rand()/(float)RAND_MAX*(rand()>RAND_MAX/2?1:-1);
}

float rand_zero_one() {
	return (float)rand()/(float)RAND_MAX;
}

float cur_camera[] = {0,0,0};

void process_keys(unsigned char key, int x, int y) {
	if (27==key) {
		exit(0);
	}
}

void change_size(int w, int h) {
    float ratio = (1.0*w)/(!h?1:h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, ratio, .1, 100);
    glOrtho(-1,1,-1,1,-2,2);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}
bool read_n_compile_shader(const char *filename, GLuint &hdlr, GLenum shaderType) {
	std::ifstream is(filename, std::ios::in|std::ios::binary|std::ios::ate);
	if (!is.is_open()) {
		std::cerr << "Unable to open file " << filename << std::endl;
		return false;
	}
	long size = is.tellg();
	char *buffer = new char[size+1];
	is.seekg(0, std::ios::beg);
	is.read (buffer, size);
	is.close();
	buffer[size] = 0;

	hdlr = glCreateShader(shaderType);
	glShaderSource(hdlr, 1, (const GLchar**)&buffer, NULL);
	glCompileShader(hdlr);
	delete [] buffer;
	return true;
}
void setShaders(GLuint &prog_hdlr, const char *vsfile, const char *fsfile) {
	GLuint vert_hdlr, frag_hdlr;
	read_n_compile_shader(vsfile, vert_hdlr, GL_VERTEX_SHADER);
	read_n_compile_shader(fsfile, frag_hdlr, GL_FRAGMENT_SHADER);

	prog_hdlr = glCreateProgram();
	glAttachShader(prog_hdlr, frag_hdlr);
	glAttachShader(prog_hdlr, vert_hdlr);
	glLinkProgram(prog_hdlr);
}


void render_scene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	//cur_camera[0] = cos(angle)*camera[0]+sin(angle)*camera[2];
	//cur_camera[1] = camera[1];
	//cur_camera[2] = cos(angle)*camera[2]-sin(angle)*camera[0];
	//gluLookAt(cur_camera[0], cur_camera[1], cur_camera[2], 0,  0, 0, 0, 1, 0);


	// shader updating
	
	
	glm::mat4 transform;
    glm::mat4 model;
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
	glm::mat4 view;
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -30.0f)); 
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(0.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);


	glUseProgram(prog_hdlr);
	
	int mLoc=glGetUniformLocation(prog_hdlr,"model");
	glUniformMatrix4fv(mLoc,1,GL_FALSE, glm::value_ptr(model));

	int vLoc=glGetUniformLocation(prog_hdlr,"view");
	glUniformMatrix4fv(vLoc,1,GL_FALSE, glm::value_ptr(view));

	int pLoc=glGetUniformLocation(prog_hdlr,"projection");
	glUniformMatrix4fv(pLoc,1,GL_FALSE, glm::value_ptr(projection));
	



	glUseProgram(prog_hdlr);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);							// Set a specific color
	glClear(GL_COLOR_BUFFER_BIT);
	
	//glUseProgram(prog_hdlr);
	// draw first triangle using the data from the first VAO
	glBindVertexArray(VAOs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 39);

	glBindVertexArray(VAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 18);

	//glEnd();
	glFlush();
	glUseProgram(0);
	glutSwapBuffers();

}



void init(){

	glewExperimental = GL_TRUE;											//  checking if a function is available.
	if (glewInit() != GLEW_OK) {
		cout << "Failes to initialize GLEW" << endl;
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
		std::cout << "Ready for GLSL - vertex, fragment, and geometry units" << std::endl;
	else {
		std::cout << "No GLSL support" << std::endl;
		exit(1);
	}
	setShaders(prog_hdlr, "shaders/vert_shader.glsl", "shaders/frag_shader.glsl");

	location_attribute_0   = glGetAttribLocation(prog_hdlr, "R");          // radius
	location_viewport = glGetUniformLocation(prog_hdlr, "viewport"); // viewport

	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);



	GLfloat firstLetter[] = {
		0.0f,0.0f,0.0f,
		1.0f,5.0f,0.0f,
		1.0f,0.0f,0.0f,

		0.0f,0.0f,0.0f,
		1.0f,5.0f,0.0f,
		0.0f,5.0f,0.0f,

		0.0f,5.0f,0.0f,
		2.0f,5.0f,0.0f,
		0.0f,6.0f,0.0f,

		0.0f,6.0f,0.0f,
		2.0f,5.0f,0.0f,
		2.0f,6.0f,0.0f,

		2.0f,6.0f,0.0f,
		2.0f,5.0f,0.0f,
		3.0f,5.0f,0.0f,

		2.0f,5.0f,0.0f,
		3.0f,5.0f,0.0f,
		3.0f,4.0f,0.0f,

		3.0f,4.0f,0.0f,
		4.0f,4.0f,0.0f,
		3.0f,5.0f,0.0f,

		3.0f,4.0f,0.0f,
		4.0f,4.0f,0.0f,
		3.0f,3.0f,0.0f,

		3.0f,4.0f,0.0f,
		2.0f,3.0f,0.0f,
		3.0f,3.0f,0.0f,

		1.0f,2.0f,0.0f,
		3.0f,3.0f,0.0f,
		3.0f,2.0f,0.0f,

		1.0f,2.0f,0.0f,
		1.0f,3.0f,0.0f,
		3.0f,3.0f,0.0f,

		2.0f,2.0f,0.0f,
		3.0f,2.0f,0.0f,
		3.0f,0.0f,0.0f,

		3.0f,0.0f,0.0f,
		3.0f,2.0f,0.0f,
		4.0f,0.0f,0.0f,

	};

	GLfloat SecondLetter[]={
		0.0f,6.0f,0.0f,
		1.0f,6.0f,0.0f,
		3.0f,0.0f,0.0f,

		1.0f,6.0f,0.0f,
		3.0f,0.0f,0.0f,
		3.0f,2.0f,0.0f,

		3.0f,2.0f,0.0f,
		3.0f,0.0f,0.0f,
		4.0f,0.0f,0.0f,

		3.0f,2.0f,0.0f,
		4.0f,2.0f,0.0f,
		4.0f,0.0f,0.0f,

		4.0f,0.0f,0.0f,
		4.0f,2.0f,0.0f,
		6.0f,6.0f,0.0f,

		6.0f,6.0f,0.0f,
		4.0f,0.0f,0.0f,
		7.0f,6.0f,0.0f,
	};


	for(int i=0;i<=116;i++){
		firstLetter[i]=(firstLetter[i]-5)/10;
	}
	for(int i=0;i<=54;i++){
		SecondLetter[i]=(SecondLetter[i])/10;
	}
	
	// Vertex Buffer Object
	glGenBuffers(2, VBOs);												
	glGenVertexArrays(2, VAOs);

    glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstLetter), firstLetter, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SecondLetter), SecondLetter, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);


    //glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glLightfv(GL_LIGHT0, GL_POSITION, light0_position); 
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 


}


void reshapefunc(int width,int height)
{
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/height,1.0,10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
}

void mousefunc(int button,int state,int x,int y)
{
	if (button==GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {			
			flag=1;
		} else {
			flag=0;
		}
	}
}

void motionfunc(int x,int y)
{
	if (flag>0) {
		if (flag>1) {
			rotx+=360.0f*(x-mx)/winwidth;
			roty+=360.0f*(y-my)/winheight;
			//cout << "hola"<< endl;
		}

		mx=x;
		my=y;

		//drawgraphix();
		render_scene();

		flag=2;
	}
}

void keyboardfunc(unsigned char key,int x,int y)
{
	if (key=='q' || key==27) exit(0);
}


int main(int argc, char **argv) {


	glutInit(&argc, argv);
	glewExperimental = GL_TRUE;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("2da Pregutna Practica 1");
	
	//glClearColor(0.0,0.0,1.0,1.0);
	//glutReshapeFunc(change_size);
	//glutKeyboardFunc(process_keys);
	
	init();
	//glutIdleFunc(idle);
	glutDisplayFunc(render_scene);
	//glutReshapeFunc(reshapefunc);
    //glutMouseFunc(mousefunc);
    //glutMotionFunc(motionfunc);
    //glutKeyboardFunc(keyboardfunc);

	glutMainLoop();


	return 0;
}

