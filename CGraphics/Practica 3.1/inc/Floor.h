#include "../lib/Angel-yjc.h"
#include <GL/glut.h>
#include <fstream>

#ifndef FLOOR_H
#define FLOOR_H


	typedef Angel::vec3  color3;
	typedef Angel::vec3  point3;
	typedef Angel::vec4  color4;
	typedef Angel::vec4  point4;

	GLuint vNormal;
	GLuint vTexCoord;
	

vec2 quad_texCoord[6] = {
  vec2(0.0, 0.0),  // for a
  vec2(0.0, 1.0),  // for b
  vec2(1.0, 1.0),  // for c

  vec2(1.0, 1.0),  // for c
  vec2(1.0, 0.0),  // for d
  vec2(0.0, 0.0),  // for a 
};


	class Floor{
		
		int 		m_number_vertices;
		GLuint 		m_floor_buffer;
		point3* 	m_floor_points;
		color3*   	m_floor_colors;
		std::string m_name;
		int 		m_text_flag;		

	public:
		int 		m_solid_flag;
		int 		m_text_ground_flag;
		
		Floor(){}

		void ReadFloor(const char* dir,std::string name){
			std::fstream file;
			file.open(dir);			
			float a,b,c;
			int m,n;					
			file >> m;			
			m_name = name;

			m_number_vertices = m*3;
			m_floor_points = new point3[m*3];
			m_floor_colors = new point3[m*3];
			
			for(int i=0;i<m_number_vertices;i++){
				m_floor_colors[i] = color3(0.0,1.0,0.0);
			}

			for(int i=0;i<m;i++){
				file >> n;												
				for(int j=0;j<n;j++){
					file >> a >> b >> c;
					m_floor_points[i*n+j] = point3(a,b,c);
				}
			}				
		}

		void InitFloor(){										
		  glGenBuffers(1, &m_floor_buffer);		 	
		  glBindBuffer(GL_ARRAY_BUFFER, m_floor_buffer);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices + sizeof(color3)*m_number_vertices + sizeof(vec2)*m_number_vertices, NULL, GL_STATIC_DRAW);
		  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*m_number_vertices, m_floor_points);
		  glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices, sizeof(color3)*m_number_vertices, m_floor_colors);	
		  glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices+sizeof(color3)*m_number_vertices, sizeof(vec2)*m_number_vertices, quad_texCoord);	

		}

		void DrawFloor(GLuint& program,GLuint& model_view,mat4& mv) {

		  mv = mv*Translate(0.3,0.0,0.0) * Scale(1.0,1.0,1.0);
  		  glUniformMatrix4fv(model_view,1,GL_TRUE,mv);  
  		  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		  glBindBuffer(GL_ARRAY_BUFFER, m_floor_buffer);

		  GLuint vPosition = glGetAttribLocation(program, "vPosition");
		  glEnableVertexAttribArray(vPosition);
		  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		  		  
		  GLuint vColor = glGetAttribLocation(program, "vColor");
		  glEnableVertexAttribArray(vColor);
		  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point3) * m_number_vertices) );

		  
		  //mv = mv * Rotate(-35, 0.0, 1.0, 0.0);
  		  //glUniformMatrix4fv(model_view,1,GL_TRUE,mv);
		  
		  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
		  glEnableVertexAttribArray( vTexCoord );
		  glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point3)*m_number_vertices ));

		  /*
		  if(m_text_ground_flag==1){

		  	std::cout<< "**************************" << std::endl;
		  	vNormal = glGetAttribLocation( program, "vNormal" );
			glEnableVertexAttribArray( vNormal );
			glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( m_number_vertices * sizeof(point4)) );

			vTexCoord = glGetAttribLocation( program, "vTexCoord" );
			glEnableVertexAttribArray( vTexCoord );
			glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_number_vertices * (sizeof(point4) + sizeof(point3)) ));
		  }	
		  */
		  

		  glDrawArrays(GL_TRIANGLES, 0, m_number_vertices); 	


		  glDisableVertexAttribArray(vTexCoord);
		  //glDisableVertexAttribArray(vNormal);	
		
		  glDisableVertexAttribArray(vPosition);
		  glDisableVertexAttribArray(vColor);

		}

	};




#endif