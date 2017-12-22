#include "../lib/Angel-yjc.h"
#include <GL/glut.h>
#include <fstream>
#include "Utils.h"

#ifndef SPHERE_H
#define SPHERE_H

	typedef Angel::vec3  color3;
	typedef Angel::vec3  point3;

	point3 A( 3.0,1.0, 5.0);  
	point3 B(-1.0,1.0,-4.0);
	point3 C( 3.5,1.0,-2.5);
	point3 pos = A;
	point3 obj = B;
	point3 rot;
	point3 uBA = (B-A)/utils::norm(B,A);
	point3 uCB = (C-B)/utils::norm(C,B);
	point3 uAC = (A-C)/utils::norm(A,C);
	point3 u = uBA;

	int flag1 = 1;
	int flag2 = 0;
	int flag3 = 0;

	float dangle = 1.0;
	float radio  = 1.0;
	float dl = dangle*radio;
	int c = 0;
	GLfloat angle = 0.0; 

	point3 p[3] = {B,C,A};
	point3 uu[3] = {uBA,uCB,uAC};

	class Sphere{
		
		int 		m_number_vertices;
		GLuint 		m_sphere_buffer;
		point3* 	m_sphere_points;
		color3*   	m_sphere_colors;
		std::string m_name;
		int 		m_text_flag;

	public:
		int 		m_solid_flag;
		
		Sphere(){}

		void ReadSphere(const char* dir,std::string name){
			std::fstream file;
			file.open(dir);			
			float a,b,c;
			int m,n;					
			file >> m;			
			m_name = name;
			m_number_vertices = m*3;
			m_sphere_points = new point3[m*3];
			m_sphere_colors = new point3[m*3];				
			for(int i=0;i<m_number_vertices;i++){
				m_sphere_colors[i] = color3(1.0,0.84,0.0);
			}

			for(int i=0;i<m;i++){
				file >> n;												
				for(int j=0;j<n;j++){
					file >> a >> b >> c;
					m_sphere_points[i*n+j] = point3(a,b,c);
				}
			}	
			

		}

		void UpdateSphere(){   
		  angle += 1.0;   
		  if(utils::norm(pos,obj) < 0.1){
		    c++;
		    pos = obj;
		    obj = p[c%3]; 
		    u = uu[c%3];
		    std::cout << "pos: " << pos << " ----  obj: "<< obj << std::endl;      
		  }
		  rot = point3( u.x,u.y,-u.z );  
		  pos = pos + u*dl/20;    
		}


		void InitSphere(){										
		  glGenBuffers(1, &m_sphere_buffer);		 	
		  glBindBuffer(GL_ARRAY_BUFFER, m_sphere_buffer);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices + sizeof(color3)*m_number_vertices, NULL, GL_STATIC_DRAW);
		  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*m_number_vertices, m_sphere_points);
		  glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices, sizeof(color3)*m_number_vertices, m_sphere_colors);		 
		}

		void DrawSphere(GLuint& program,GLuint& model_view,mat4& mv){
		  UpdateSphere();
		  mv = mv*Translate(pos) * Scale (1.0,1.0,1.0) * Rotate(angle*5, rot.x,rot.y,rot.z);
		  glUniformMatrix4fv(model_view,1,GL_TRUE,mv);

		  glPolygonMode(GL_FRONT_AND_BACK, m_solid_flag?GL_FILL:GL_LINE);

		  glBindBuffer(GL_ARRAY_BUFFER, m_sphere_buffer);
		  GLuint vPosition = glGetAttribLocation(program, "vPosition");
		  glEnableVertexAttribArray(vPosition);
		  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		  GLuint vColor = glGetAttribLocation(program, "vColor");
		  glEnableVertexAttribArray(vColor);
		  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point3) * m_number_vertices) );
		  glDrawArrays(GL_TRIANGLES, 0, m_number_vertices); 		  	  		 
		  glDisableVertexAttribArray(vPosition);
		  glDisableVertexAttribArray(vColor);
		}
	};




#endif