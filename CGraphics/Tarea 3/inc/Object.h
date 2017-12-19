#include "../lib/Angel-yjc.h"
#include <GL/glut.h>
#include <fstream>

#ifndef OBJECT_H
#define OBJECT_H


	typedef Angel::vec3  color3;
	typedef Angel::vec3  point3;

	class Object{
		
		int 			  m_number_vertices;
		GLuint 		  m_object_buffer;
		point3* 	  m_object_points;
		color3*   	m_object_colors;
		std::string m_name;
		int 				m_text_flag;


	public:
		int 				m_solid_flag;
		
		Object(){}

		void ReadObject(const char* dir,std::string name){

			std::fstream file;
			file.open(dir);			
			float a,b,c;
			int m,n;					
			file >> m;			
			m_name = name;
			if( name == "axis"){				
				m_number_vertices = m*2;
				//std::cout << "**********************" << std::endl ;
				m_object_points = new point3[m*2];
				m_object_colors = new point3[m*2];				
				m_object_colors[0] = color3(1.0, 0.0, 0.0);
				m_object_colors[1] = color3(1.0, 0.0, 0.0);
				m_object_colors[2] = color3(1.0, 0.0, 1.0);
				m_object_colors[3] = color3(1.0, 0.0, 1.0);
				m_object_colors[4] = color3(0.0, 0.0, 1.0);
				m_object_colors[5] = color3(0.0, 0.0, 1.0);
			}else{								
				
				m_number_vertices = m*3;
				m_object_points = new point3[m*3];
				m_object_colors = new point3[m*3];

				if( name == "floor"){					
					for(int i=0;i<m_number_vertices;i++){
						m_object_colors[i] = color3(0.0,1.0,0.0);
					}
				}
				if(name == "sphere"){					
					for(int i=0;i<m_number_vertices;i++){
						m_object_colors[i] = color3(1.0,0.84,0.0);
					}
				}				
			}

			for(int i=0;i<m;i++){
				file >> n;												
				for(int j=0;j<n;j++){
					file >> a >> b >> c;
					m_object_points[i*n+j] = point3(a,b,c);
				}
			}	
			

		}

		void InitObject(){										
			glGenBuffers(1, &m_object_buffer);		 	
		  glBindBuffer(GL_ARRAY_BUFFER, m_object_buffer);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices + sizeof(color3)*m_number_vertices, NULL, GL_STATIC_DRAW);
		  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*m_number_vertices, m_object_points);
		  glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices, sizeof(color3)*m_number_vertices, m_object_colors);		 
		}


		void drawObj(GLuint& program){

			/*
			if(m_text_flag == 1){
		  	glUniform1i( glGetUniformLocation(program, "texture_1D"), 1 );
				glUniform1i( glGetUniformLocation(program, "texture_2D"), 0 );
				glUniform1i(glGetUniformLocation(program, "text_ground_flag"), 1);
		  }else{
		  	GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
				glEnableVertexAttribArray( vTexCoord );
				glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(num_vertices * (sizeof(point4) + sizeof(point3)) ));

		  }*/

			glBindBuffer(GL_ARRAY_BUFFER, m_object_buffer);
		  GLuint vPosition = glGetAttribLocation(program, "vPosition");
		  glEnableVertexAttribArray(vPosition);
		  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		  GLuint vColor = glGetAttribLocation(program, "vColor");
		  glEnableVertexAttribArray(vColor);
		  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point3) * m_number_vertices) );

		  if(m_name == "axis"){
		  	glDrawArrays(GL_LINES, 0, m_number_vertices); 
		  }else{
		  	glDrawArrays(GL_TRIANGLES, 0, m_number_vertices); 
			}		  		 
		  glDisableVertexAttribArray(vPosition);
		  glDisableVertexAttribArray(vColor);
		}
	};




#endif