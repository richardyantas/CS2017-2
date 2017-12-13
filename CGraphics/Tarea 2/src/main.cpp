#include <iostream>

//#include <GL/glew.h>
//#include <GLUT/glut.h>

#include "../lib/Angel-yjc.h"

typedef Angel::vec3 color3;
typedef Angel::vec3 point3;


GLuint program;       

GLfloat  fovy = 45.0; 
GLfloat  aspect;      
GLfloat  zNear = 0.5, zFar = 100.0;

GLfloat angle = 0.0; 
vec4 init_eye(6.0, 3.0, -1.0, 3.0); 
vec4 eye = init_eye;              
vec4 at;
vec4 up;

void init(){

}

void display(void){

}


void idle (void)
{
    //angle += 1.0;   
    //glutPostRedisplay();
}

void keyboard(unsigned char key,int x,int y){

}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    //aspect = (GLfloat) width  / (GLfloat) height;
    glutPostRedisplay();
}


int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(450,150); 
  glutInitWindowSize(600, 600);
  glutCreateWindow("Rolling Sphere");

  int err = glewInit();
  if(GLEW_OK != err){
    printf("Error: glewInit failed: %s\n",(char *) glewGetErrorString(err));
    exit(1);
  }
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
}
