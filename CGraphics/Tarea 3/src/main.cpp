#include <iostream>

#include "../lib/Angel-yjc.h"
#include "../inc/Object.h"
#include "../inc/InitShader.h"
#include "../inc/Menu.h"

using namespace std;

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

int animationFlag = 1;
int index = 0;

Object Ofloor;  // Avoid Conflicts with other names 
Object Osphere;
Object Oaxis;
const char* dir_floor  = "objects/floor.2";
const char* dir_sphere = "objects/sphere.1024";
const char* dir_axis   = "objects/axis.6";

/*   FLAGS   */
int fog_flag = 0;


/*
void init_tex_ground()
{
  image_set_up();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glGenTextures(1, &texName);      // Generate texture obj name(s)

  glActiveTexture( GL_TEXTURE0 );  // Set the active texture unit to be 0
  glBindTexture(GL_TEXTURE_2D, texName); // Bind the texture to this texture unit

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight,0, GL_RGBA, GL_UNSIGNED_BYTE, Image);

  glGenBuffers(1, &f_tex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, f_tex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f_tex_vertices) + sizeof(f_tex_normals) + sizeof(f_tex_coord),NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(f_tex_vertices), f_tex_vertices);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(f_tex_vertices), sizeof(f_tex_normals), f_tex_normals);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(f_tex_vertices) + sizeof(f_tex_normals),sizeof(f_tex_coord), f_tex_coord);
}

void floor_light(){
  Index = 0;
  my_quad(1, 0, 3, 2);
  glGenBuffers( 1, &floor_light_buffer );
  glBindBuffer( GL_ARRAY_BUFFER, floor_light_buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(floor_light_points) + sizeof(floor_normals),NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(floor_light_points), floor_light_points );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(floor_light_points),sizeof(floor_normals), floor_normals );
}*/


void init(){

  //init_tex_ground();
  //floor_light();

  Ofloor.ReadObject(dir_floor,"floor");
  Osphere.ReadObject(dir_sphere,"sphere");
  Oaxis.ReadObject(dir_axis,"axis");

  Ofloor.InitObject(); // The object is build after to execute main but is declarated
  Osphere.InitObject();
  Oaxis.InitObject(); // If not is called is give segmentation fault

  program = InitShader("shaders/vshader42.glsl","shaders/fshader42.glsl");
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0,0.0,0.0,1.0);
  glLineWidth(2.0);  
}

float norm(point3 u, point3 v){
  return sqrt( pow(u.x-v.x,2)+pow(u.y-v.y,2)+pow(u.z-v.z,2) );
}

point3 A( 3.0,1.0, 5.0);  
point3 B(-1.0,1.0,-4.0);
point3 C( 3.5,1.0,-2.5);
point3 pos = A;
point3 obj = B;
point3 rot;
point3 uBA = (B-A)/norm(B,A);
point3 uCB = (C-B)/norm(C,B);
point3 uAC = (A-C)/norm(A,C);
point3 u = uBA;

int flag1 = 1;
int flag2 = 0;
int flag3 = 0;

float dangle = 1.0;
float radio  = 1.0;
float dl = dangle*radio;
int c = 0;

point3 p[3] = {B,C,A};
point3 uu[3] = {uBA,uCB,uAC};


void update_sphere(){    
  if(norm(pos,obj) < 0.1){
    c++;
    pos = obj;
    obj = p[c%3]; 
    u = uu[c%3];
    std::cout << "pos: " << pos << " ----  obj: "<< obj << std::endl;      
  }
  rot = point3( u.x,u.y,-u.z );  
  pos = pos + u*dl/20;    
}


void display(void){
  GLuint model_view;
  GLuint projection;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
  glUseProgram(program);


  /* Added two parts */
  //glUniform1i(glGetUniformLocation(program, "text_ground_flag"), 0);
  
  /* ----------------*/

  model_view = glGetUniformLocation(program,"model_view");
  projection = glGetUniformLocation(program,"projection");
  
  mat4 p = Perspective(fovy,1,zNear,zFar);
  glUniformMatrix4fv(projection,1,GL_TRUE,p);

  eye = vec3(7,3,-10);
  at  = vec3(0,0,0);
  up  = vec3(0,1,0);

  mat4 mv = LookAt(eye,at,up);

  //mv = mv*Translate(0.3,0.0,0.0)*Scale(1.0,1.0,1.0);
  glUniformMatrix4fv(model_view,1,GL_TRUE,mv);  
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glUniform1i(glGetUniformLocation(program, "fog_flag"), fog_flag);
  Ofloor.drawObj(program);

  //mv = LookAt(eye, at, up) * Translate(0.0, 0.0, 0.0) * Scale (1.0, 1.0, 1.0);
  //glUniformMatrix4fv(model_view,1,GL_TRUE,mv);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  Oaxis.drawObj(program);


  update_sphere();
  mv = mv*Translate(pos) * Scale (1.0,1.0,1.0) * Rotate(angle*5, rot.x,rot.y,rot.z);
  glUniformMatrix4fv(model_view,1,GL_TRUE,mv);
  if(Osphere.m_solid_flag == 1){
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }else{
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  Osphere.drawObj(program);

  

  glutSwapBuffers();  
}




void idle (void)
{
    angle += 1.0;   
    glutPostRedisplay();
}

void keyboard(unsigned char key,int x,int y){

}


void quit(){
  exit(0);
}

void main_menu(int index){
  switch (index)
  {
    case (0) :
    {
      break;
    }
    case (9) :
    {
      exit(0);
      break;
    }
    case (2) :
    {     
      Osphere.m_solid_flag = 1;
      break;
    }
    case (3) :
    {
      Osphere.m_solid_flag = 0;
      break;
    }
    case (4) :
    {
      fog_flag = 0;
      cout <<  fog_flag << endl;
      break;
    }
    case (5) :
    {
      fog_flag = 1;
      cout <<  fog_flag << endl;
      break;
    }
    case (6) :
    {
      fog_flag = 2;
      cout <<  fog_flag << endl;
      break;
    }
    case (7) :
    {
      fog_flag = 3;
      cout <<  fog_flag << endl;
      break;
    }
  }
  display();
}


void addMenu() {

  int text_ground_tmp = glutCreateMenu(main_menu);
  glutAddMenuEntry("No", 0);
  glutAddMenuEntry("Yes", 1);

  int wire_frame_sphere = glutCreateMenu(main_menu);
  glutAddMenuEntry("No", 2);
  glutAddMenuEntry("Yes", 3);

  int fog_option = glutCreateMenu(main_menu);
  glutAddMenuEntry("No fog", 4);
  glutAddMenuEntry("Linear", 5);
  glutAddMenuEntry("Exponential", 6);
  glutAddMenuEntry("Exponential square", 7);

  glutCreateMenu(main_menu);
  glutAddMenuEntry("Default View Point", 8);
  glutAddMenuEntry("Quit", 9);

  glutAddSubMenu("Wire Frame Sphere", wire_frame_sphere);

  glutAddSubMenu("Fog option", fog_option);
  glutAddSubMenu("Texture Mapped Ground", text_ground_tmp);


  glutAttachMenu(GLUT_LEFT_BUTTON);
}



void reshape(int width, int height)
{
    glClearColor(0.529f,0.807f,0.92f,0.0f);
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
  addMenu();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
}
