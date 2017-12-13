#include "../lib/Angel-yjc.h"

#ifndef OBJECT_H
#define OBJECT_H


	typedef Angel::vec3  color3;
	typedef Angel::vec3  point3;

	class Object{
		//point3 vertex_position[];
		//color3 vertex_color[];
		//GLuint buffer[];	
		int flag;

	public:
		 
		Object(GLuint &floor_buffer,const point3 floor_points[6],const color3 floor_colors[6]){
			flag = 1;
			glGenBuffers(1, &floor_buffer);
		  glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors), NULL, GL_STATIC_DRAW);
		  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
		  glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_colors), floor_colors);
		}

		//void createVertexBuffer();

		void drawObj(GLuint buffer,int num_vertices,GLuint program){
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
		  GLuint vPosition = glGetAttribLocation(program, "vPosition");
		  glEnableVertexAttribArray(vPosition);
		  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		  GLuint vColor = glGetAttribLocation(program, "vColor");
		  glEnableVertexAttribArray(vColor);
		  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point3) * num_vertices) );

		  glDrawArrays(GL_TRIANGLES, 0, num_vertices);
		  
		  glDisableVertexAttribArray(vPosition);
		  glDisableVertexAttribArray(vColor);
		}
	};




#endif