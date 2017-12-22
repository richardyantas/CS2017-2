#include "../lib/Angel-yjc.h"
#include <GL/glut.h>
#include <fstream>

#ifndef AXIS_H
#define AXIS_H


	typedef Angel::vec3  color3;
	typedef Angel::vec3  point3;

	class Axis{
		
		int 		m_number_vertices;
		GLuint 		m_axis_buffer;
		point3* 	m_axis_points;
		color3*   	m_axis_colors;
		std::string m_name;
		int 		m_text_flag;

	public:
		int 		m_solid_flag;
		
		Axis(){}

		void ReadAxis(const char* dir,std::string name){

			std::fstream file;
			file.open(dir);			
			float a,b,c;
			int m,n;					
			file >> m;			
			m_name = name;
			
			m_number_vertices = m*2;

			m_axis_points = new point3[m*2];
			m_axis_colors = new point3[m*2];				
			m_axis_colors[0] = color3(1.0, 0.0, 0.0);
			m_axis_colors[1] = color3(1.0, 0.0, 0.0);
			m_axis_colors[2] = color3(1.0, 0.0, 1.0);
			m_axis_colors[3] = color3(1.0, 0.0, 1.0);
			m_axis_colors[4] = color3(0.0, 0.0, 1.0);
			m_axis_colors[5] = color3(0.0, 0.0, 1.0);
			
			for(int i=0;i<m;i++){
				file >> n;												
				for(int j=0;j<n;j++){
					file >> a >> b >> c;
					m_axis_points[i*n+j] = point3(a,b,c);
				}
			}				
		}

		void InitAxis(){										
		  glGenBuffers(1, &m_axis_buffer);		 	
		  glBindBuffer(GL_ARRAY_BUFFER, m_axis_buffer);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices + sizeof(color3)*m_number_vertices, NULL, GL_STATIC_DRAW);
		  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*m_number_vertices, m_axis_points);
		  glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices, sizeof(color3)*m_number_vertices, m_axis_colors);		 
		}


		void DrawAxis(GLuint& program,GLuint& model_view,mat4& mv){
		  //mv = LookAt(eye, at, up) * Translate(0.0, 0.0, 0.0) * Scale (1.0, 1.0, 1.0);
  		  //glUniformMatrix4fv(model_view,1,GL_TRUE,mv);
    	  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		  glBindBuffer(GL_ARRAY_BUFFER, m_axis_buffer);
		  GLuint vPosition = glGetAttribLocation(program, "vPosition");
		  glEnableVertexAttribArray(vPosition);
		  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		  GLuint vColor = glGetAttribLocation(program, "vColor");
		  glEnableVertexAttribArray(vColor);
		  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point3) * m_number_vertices) );
		  glDrawArrays(GL_LINES, 0, m_number_vertices); 		 	  		 
		  glDisableVertexAttribArray(vPosition);
		  glDisableVertexAttribArray(vColor);
		}
	};




#endif