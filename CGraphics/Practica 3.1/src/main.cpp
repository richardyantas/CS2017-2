#include <iostream>

#include "../lib/Angel-yjc.h"
#include "../inc/Axis.h"
#include "../inc/Floor.h"
#include "../inc/Sphere.h"
#include "../inc/InitShader.h"
#include "../inc/Menu.h"

using namespace std;

typedef Angel::vec3 color3;
typedef Angel::vec3 point3;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

GLuint program;       

GLfloat  fovy = 45.0;
GLfloat  aspect;      
GLfloat  zNear = 0.5, zFar = 100.0;

vec4 init_eye(6.0, 3.0, -1.0, 3.0);
vec4 eye = init_eye;              
vec4 at;
vec4 up;

int animationFlag = 1;
int index = 0;

Floor Ofloor;  // Avoid Conflicts with other names 
Sphere Osphere;
Axis Oaxis;

const char* dir_floor  = "objects/floor.2";
const char* dir_sphere = "objects/sphere.1024";
const char* dir_axis   = "objects/axis.6";

GLuint  f_tex_buffer;

static GLuint texName;

#define ImageWidth  64
#define ImageHeight 64
GLubyte Image[ImageHeight][ImageWidth][4];
#define stripeImageWidth 32
GLubyte stripeImage[4*stripeImageWidth];


/*   FLAGS   */
int fog_flag = 0;
int text_ground_flag = 0;


point4 f_tex_vertices[6] = {
  point4(  -5,  0,  -4, 1),
  point4(  -5,  0,  8, 1),
  point4( 5, 0,  8, 1),

  point4( 5, 0,  8, 1),
  point4( 5, 0,  -4, 1),
  point4(  -5,  0,  -4, 1),
};

point3 f_tex_normals[6] = {
  point3(  0,  1,  0),
  point3(  0,  1,  0),
  point3(  0,  1,  0),
  point3(  0,  1,  0),
  point3(  0,  1,  0),
  point3(  0,  1,  0),
};



void image_set_up(void)
{
  int i, j, c;
  /* --- Generate checkerboard image to the image array ---*/
  for (i = 0; i < ImageHeight; i++)
    for (j = 0; j < ImageWidth; j++)
    {
      c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0));

      if (c == 1) /* white */
      {
        c = 255;
        Image[i][j][0] = (GLubyte) c;
        Image[i][j][1] = (GLubyte) c;
        Image[i][j][2] = (GLubyte) c;
      }
      else  /* green */
      {
        Image[i][j][0] = (GLubyte) 0;
        Image[i][j][1] = (GLubyte) 150;
        Image[i][j][2] = (GLubyte) 0;
      }

      Image[i][j][3] = (GLubyte) 255;
    }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  /*--- Generate 1D stripe image to array stripeImage[] ---*/
  for (j = 0; j < stripeImageWidth; j++) {
    
    /* When j <= 4, the color is (255, 0, 0),   i.e., red stripe/line.
       When j > 4,  the color is (255, 255, 0), i.e., yellow remaining texture
     */

    stripeImage[4 * j] = (GLubyte)    255;
    stripeImage[4 * j + 1] = (GLubyte) ((j > 4) ? 255 : 0);
    stripeImage[4 * j + 2] = (GLubyte) 0;
    stripeImage[4 * j + 3] = (GLubyte) 255;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

}


void init_tex_ground()
{
  image_set_up();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  /*--- Create and Initialize a texture object ---*/
  glGenTextures(1, &texName);      // Generate texture obj name(s)

  glActiveTexture( GL_TEXTURE0 );  // Set the active texture unit to be 0
  glBindTexture(GL_TEXTURE_2D, texName); // Bind the texture to this texture unit

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight,0, GL_RGBA, GL_UNSIGNED_BYTE, Image);

  /*
  glGenBuffers(1, &f_tex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, f_tex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f_tex_vertices) + sizeof(f_tex_normals) + sizeof(f_tex_coord),NULL, GL_STATIC_DRAW);

  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(f_tex_vertices), f_tex_vertices);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(f_tex_vertices) , sizeof(f_tex_normals) ,f_tex_normals);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(f_tex_vertices) + sizeof(f_tex_normals) , sizeof(f_tex_normals), f_tex_normals);  
  */
}


void init(){

  init_tex_ground();
  //floor_light();

  Ofloor.ReadFloor(dir_floor,"floor");
  Osphere.ReadSphere(dir_sphere,"sphere");
  Oaxis.ReadAxis(dir_axis,"axis");

  Ofloor.InitFloor(); // The object is build after to execute main but is declarated
  Osphere.InitSphere();
  Oaxis.InitAxis(); // If not is called is give segmentation fault

  program = InitShader("shaders/vshader42.glsl","shaders/fshader42.glsl");
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0,0.0,0.0,1.0);
  glLineWidth(2.0);  
}

void display(void){
  GLuint model_view;
  GLuint projection;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
  glUseProgram(program);

  /* Added two parts */
  glUniform1i(glGetUniformLocation(program, "text_ground_flag"), text_ground_flag);
  glUniform1i(glGetUniformLocation(program, "fog_flag"), fog_flag);
  glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
  /* ----------------*/

  model_view = glGetUniformLocation(program,"model_view");
  projection = glGetUniformLocation(program,"projection");
  
  mat4 p = Perspective(fovy,1,zNear,zFar);
  glUniformMatrix4fv(projection,1,GL_TRUE,p);

  eye = vec3(7,3,-10);
  at  = vec3(0,0,0);
  up  = vec3(0,1,0);

  mat4 mv = LookAt(eye,at,up);
    
  Ofloor.DrawFloor(program,model_view,mv);
  Oaxis.DrawAxis(program,model_view,mv);
  Osphere.DrawSphere(program,model_view,mv);

  glutSwapBuffers();  
}

void idle (void)
{
    //angle += 1.0;   
    glutPostRedisplay();
}

void keyboard(unsigned char key,int x,int y){

}

/*
void tex_ground_menu(int index)
{
  //Ofloor.m_text_flag = (index == 2) ? 1 : 0;
  //display();
}
*/

void quit(){
  exit(0);
}

void main_menu(int index){
  switch (index)
  {
    case (0) :
    {
      text_ground_flag = 0;
      Ofloor.m_text_ground_flag = 0;
      break;
    }
    case (1) :
    {
      text_ground_flag = 1;
      Ofloor.m_text_ground_flag = 1;
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
