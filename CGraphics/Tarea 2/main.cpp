
#include "Angel-yjc.h"
#include <fstream>
using namespace std;

typedef Angel::vec3  color3;
typedef Angel::vec3  point3;

GLuint Angel::InitShader(const char* vShaderFile, const char* fShaderFile);

GLuint program;       /* shader program object id */
GLuint cube_buffer;   /* vertex buffer object id for cube */
GLuint floor_buffer;  /* vertex buffer object id for floor */

// Projection transformation parameters
GLfloat  fovy = 45.0; // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;      // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 3.0;

GLfloat angle = 0.0; // rotation angle in degrees
vec4 init_eye(6.0, 3.0, -1.0, 3.0); // initial viewer position
vec4 eye = init_eye;               // current viewer position

int animationFlag = 1; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'

int cubeFlag = 1;   // 1: solid cube; 0: wireframe cube. Toggled by key 'c' or 'C'
int floorFlag = 1;  // 1: solid floor; 0: wireframe floor. Toggled by key 'f' or 'F'

const int cube_NumVertices = 36; //(6 faces)*(2 triangles/face)*(3 vertices/triangle)
#if 0
#endif
#if 1
point3 cube_points[100]; 
color3 cube_colors[100];
#endif


// Vertices of a unit cube centered at origin, sides aligned with axes
point3 vertices[8] = {
    point3( -0.5, -0.5,  0.5),
    point3( -0.5,  0.5,  0.5),
    point3(  0.5,  0.5,  0.5),
    point3(  0.5, -0.5,  0.5),
    point3( -0.5, -0.5, -0.5),
    point3( -0.5,  0.5, -0.5),
    point3(  0.5,  0.5, -0.5),
    point3(  0.5, -0.5, -0.5)
};
// RGBA colors
color3 vertex_colors[8] = {
    color3( 0.0, 0.0, 0.0),  // black
    color3( 1.0, 0.0, 0.0),  // red
    color3( 1.0, 1.0, 0.0),  // yellow
    color3( 0.0, 1.0, 0.0),  // green
    color3( 0.0, 0.0, 1.0),  // blue
    color3( 1.0, 0.0, 1.0),  // magenta
    color3( 1.0, 1.0, 1.0),  // white
    color3( 0.0, 1.0, 1.0)   // cyan
};
//----------------------------------------------------------------------------
int Index = 0; // YJC: This must be a global variable since quad() is called
               //      multiple times and Index should then go up to 36 for
               //      the 36 vertices and colors

// quad(): generate two triangles for each face and assign colors to the vertices


// ****************************************************************************************************************

point3 *sphere_vertices;
color3 *sphere_color;
GLuint sphere_buffer;
int sphere_NumVertices;

void read_sphere()
{
  int Points;
  GLfloat temp[3];
  int count = 0,col,points;
  cin >> col;
  sphere_vertices = new point3[col*3];
  sphere_color = new point3[col*3];

  for(int i=0;i<col;i++)
  {
    cin >> points;
    for(int j=0;j<points;j++){
      for(int k=0;k<3;k++){
        cin >> temp[k];
      }
      sphere_vertices[count++]=point3(temp[0],temp[1],temp[2]);
    }
  }
  sphere_NumVertices = col*3;
}



// **************************************************************************************

//----------------------------------------------------------------------------
// generate 12 triangles: 36 vertices and 36 colors
//----------------------------------------------------------------------------
// generate 12 triangles: 36 vertices and 36 colors
void colorSphere()
{
    for(int i=0; i<sphere_NumVertices/6; i++){
        sphere_color[6*i] = color3( 1.0, 0.0, 0.0);
        sphere_color[6*i+1] = color3( 0.0, 0.0, 0.0);
        sphere_color[6*i+2] = color3( 0.0, 1.0, 0.0);
        sphere_color[6*i+3] = color3( 0.0, 1.0, 0.0);
        sphere_color[6*i+4] = color3( 1.0, 1.0, 0.0);
        sphere_color[6*i+5] = color3( 1.0, 0.0, 0.0);      
    }     
}
//-----------------------------------------------------------------------------

const int floor_NumVertices = 6; //(1 face)*(2 triangles/face)*(3 vertices/triangle)
point3 floor_points[floor_NumVertices]; // positions for all vertices
color3 floor_colors[floor_NumVertices]; // colors for all vertices

// generate 2 triangles: 6 vertices and 6 colors
void floor()
{
    floor_colors[0] = point3(0.0,1.0,0.0); floor_points[0] = point3(5.0,0.0,8.0);
    floor_colors[1] = point3(0.0,1.0,0.0); floor_points[1] = point3(-5.0,0.0,-4.0);
    floor_colors[2] = point3(0.0,1.0,0.0); floor_points[2] = point3(-5.0,0.0,8.0);

    floor_colors[3] = point3(0.0,1.0,0.0); floor_points[3] = point3(-5.0,0.0,-4.0);
    floor_colors[4] = point3(0.0,1.0,0.0); floor_points[4] = point3(5.0,0.0,8.0);
    floor_colors[5] = point3(0.0,1.0,0.0); floor_points[5] = point3(5.0,0.0,-4.0);
}


const int axis_NumVertices=6;
point3 axis_points[axis_NumVertices];
color3 axis_colors[axis_NumVertices];
GLuint axis_buffer;


void axis(){

    axis_colors[0] = color3( 1.0, 0.0, 0.0);axis_points[0] =point3(0.0,0.0,0.0);
    axis_colors[1] = color3( 1.0, 0.0, 0.0);axis_points[1] =point3(3.0,0.0,0.0);

    axis_colors[2] = color3( 1.0, 0.0, 1.0);axis_points[2] =point3(0.0,0.0,0.0);
    axis_colors[3] = color3( 1.0, 0.0, 1.0);axis_points[3] =point3(0.0,3.0,0.0);

    axis_colors[4] = color3( 0.0, 0.0, 1.0);axis_points[4] =point3(0.0,0.0,0.0);
    axis_colors[5] = color3( 0.0, 0.0, 1.0);axis_points[5] =point3(0.0,0.0,3.0);
}

//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{

    freopen("sphere.1024","r",stdin);
    
    read_sphere();
    colorSphere();

 // Create and initialize a vertex buffer object for cube, to be used in display()

    glGenBuffers(1, &sphere_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);

    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(point3)*sphere_NumVertices + sizeof(color3)*sphere_NumVertices,
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 
                    sizeof(point3) * sphere_NumVertices, sphere_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 
                    sizeof(point3) * sphere_NumVertices, 
                    sizeof(color3) * sphere_NumVertices,
                    sphere_color);
    
    floor();     

 // Create and initialize a vertex buffer object for floor, to be used in display()
    glGenBuffers(1, &floor_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors),
		 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_colors),
                    floor_colors);

    axis();

    glGenBuffers(1, &axis_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, axis_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis_points) + sizeof(axis_colors),
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(axis_points), axis_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(axis_points), sizeof(axis_colors),
                    axis_colors);


 // Load shaders and create a shader program (to be used in display())
    program = InitShader("vshader42.glsl", "fshader42.glsl");
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); 
    glLineWidth(2.0);
}
//----------------------------------------------------------------------------
// drawObj(buffer, num_vertices):
//   draw the object that is associated with the vertex buffer object "buffer"
//   and has "num_vertices" vertices.
//
void drawObj(GLuint buffer, int num_vertices)
{
    //--- Activate the vertex buffer object to be drawn ---//
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*----- Set up vertex attribute arrays for each vertex attribute -----*/
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			  BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation(program, "vColor"); 
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
			  BUFFER_OFFSET(sizeof(point3) * num_vertices) ); 
      // the offset is the (total) size of the previous vertex attribute array(s)

    /* Draw a sequence of geometric objs (triangles) from the vertex buffer
       (using the attributes specified in each enabled vertex attribute array) */
    if( buffer == axis_buffer ){
      glDrawArrays(GL_LINES, 0, num_vertices);
    }
    else{
      glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    }
    /*--- Disable each vertex attribute array being enabled ---*/
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);
}


/*
  set_view_reference_point(x,y,z);
  set_view_plane_normal(nx,ny,nz);
  set_view_up(vup_x,vup_y,vup_z);
*/


//----------------------------------------------------------------------------

point3 calculateDirection(point3 from, point3 to){
  point3 v;
  v.x = to.x - from.x;
  v.y = to.y - from.y;
  v.z = to.z - from.z;
  return normalize(v);
}

float distanceFromTO(point3 p1,point3 p2){
  float dx = p1.x - p2.x;
  float dy = p1.y - p2.y;
  float dz = p1.z - p2.z;
  //return calculateLength(dx,dy,dz);
  float d = sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));
}

vec3 normalize(const vec3& v){
  return v/length(v);
}

point3 center(3,1,5);
point3 A(3,1,5);
point3 B(-1,1,-4);
point3 C;
point3 u;

point3 updateCenter(point3 center,point3 u,float d){
    float step = 0.01;
    if(length(center-B)>0.5){center += u*step;}
    return center;
}


int i = 0;
void display( void )
{
  GLuint  model_view;  // model-view matrix uniform shader variable location
  GLuint  projection;  // projection matrix uniform shader variable location

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glClearColor(0.529f, 0.807f, 0.92f, 0.0f);
    
    glUseProgram(program); // Use the shader program

    model_view = glGetUniformLocation(program, "model_view" );
    projection = glGetUniformLocation(program, "projection" );

/*---  Set up and pass on Projection matrix to the shader ---*/

    //GLfloat  fovy = 45.0; // Field-of-view in Y direction angle (in degrees)
    //GLfloat  aspect;      // Viewport aspect ratio
    //GLfloat  zNear = 0.5, zFar = 3.0;

    //mat4  p = Perspective(0, 1, 0, 0);
    mat4  p = Perspective(fovy+30, aspect+0.5, zNear+1.5, zFar+10.0);

    glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

    vec4 VRP(7,3,-10,1);
    vec4 VPN(-7,-3,10,0);
    vec4 VUP(0,1,0,0);

/*---  Set up and pass on Model-View matrix to the shader ---*/
    // eye is a global variable of vec4 set to init_eye and updated by keyboard()

    vec4 at(2.0, 0.0, 0.0, 0.0);
    vec4 up(0.0, 2.0, 0.0, 0.0);

    //eye = VRP;
    //at = VRP+VPN;
    //up = VUP+VRP;

    mat4  mv = LookAt(eye, at, up);

    //mv *= Rotate(speed,axis[curTrack].x,axis[curTrack].y,axis[curTrack].z);
    //mv *= Translate(center.x,center.y,center.z);

    u = calculateDirection(A,B);
    float d = distanceFromTO(A,B);

    
    center = updateCenter(center,u,d);

    mv = mv * Translate(center) * Scale (1.0, 1.0, 1.0) * Rotate( angle,-center.x, 1 ,center.z );

    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    if (cubeFlag == 1) // Filled cube
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else              // Wireframe cube
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //drawObj(cube_buffer, cube_NumVertices);  // draw the cube
    drawObj(sphere_buffer, sphere_NumVertices); 


/*----- Set up the Mode-View matrix for the floor -----*/
 // The set-up below gives a new scene (scene 2), using Correct LookAt() function
    mv = LookAt(eye, at, up) * Translate(0.3, 0.0, 0.0) * Scale (1.0, 1.0, 1.0);
 //
 // The set-up below gives the original scene (scene 1), using Correct LookAt()
 //    mv = Translate(0.0, 0.0, 0.3) * LookAt(eye, at, up) * Scale (1.6, 1.5, 3.3);
 //
 // The set-up below gives the original scene (scene 1), when using previously 
 //       Incorrect LookAt() (= Translate(1.0, 1.0, 0.0) * correct LookAt() ) 
 //    mv = Translate(-1.0, -1.0, 0.3) * LookAt(eye, at, up) * Scale (1.6, 1.5, 3.3);
 //
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    if (floorFlag == 1) // Filled floor
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else              // Wireframe floor
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(floor_buffer, floor_NumVertices);  // draw the floor


    mv = LookAt(eye, at, up) * Translate(0.0, 0.0, 0.0) * Scale (1.0, 1.0, 1.0);

    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(axis_buffer, axis_NumVertices);  // draw the floor



    glutSwapBuffers();
}
//---------------------------------------------------------------------------
void idle (void)
{
    //angle += 0.02;
    angle += 1.0;    //YJC: change this value to adjust the cube rotation speed.

    glutPostRedisplay();
}
//----------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
    	case 033: // Escape Key
    	case 'q': case 'Q':
    	    exit( EXIT_SUCCESS );
    	    break;

            case 'X': eye[0] += 1.0; break;
    	case 'x': eye[0] -= 1.0; break;
            case 'Y': eye[1] += 1.0; break;
    	case 'y': eye[1] -= 1.0; break;
            case 'Z': eye[2] += 1.0; break;
    	case 'z': eye[2] -= 1.0; break;

            case 'a': case 'A': // Toggle between animation and non-animation
    	    animationFlag = 1 -  animationFlag;
                if (animationFlag == 1) glutIdleFunc(idle);
                else                    glutIdleFunc(NULL);
                break;
    	   
            case 'c': case 'C': // Toggle between filled and wireframe cube
    	    cubeFlag = 1 -  cubeFlag;   
                break;

            case 'f': case 'F': // Toggle between filled and wireframe floor
    	    floorFlag = 1 -  floorFlag; 
                break;

    	case ' ':  // reset to initial viewer/eye position
    	    eye = init_eye;
    	    break;
    }
    glutPostRedisplay();
}
//----------------------------------------------------------------------------
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    aspect = (GLfloat) width  / (GLfloat) height;
    glutPostRedisplay();
}
//----------------------------------------------------------------------------
int main(int argc, char **argv)
{ int err;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(450,150); 
    glutInitWindowSize(600, 600);
    // glutInitContextVersion(3, 2);
    // glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("Color Cube");

  /* Call glewInit() and error checking */
  err = glewInit();
  if (GLEW_OK != err)
  { printf("Error: glewInit failed: %s\n", (char*) glewGetErrorString(err)); 
    exit(1);
  }
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    init();
    glutMainLoop();
    return 0;
}
