/***************************
 * File: vshader42.glsl:
 *   A simple vertex shader.
 *
 * - Vertex attributes (positions & colors) for all vertices are sent
 *   to the GPU via a vertex buffer object created in the OpenGL program.
 *
 * - This vertex shader uses the Model-View and Projection matrices passed
 *   on from the OpenGL program as uniform variables of type mat4.
 ***************************/

#version 130
 
in  vec3 vPosition;
in  vec3 vColor;
out vec4 color;

out int fog_f;


uniform mat4 model_view;
uniform mat4 projection;

out vec4 Eye;				// To Fog

void main() 
{
	//fog_f=fog_flag;
	Eye =  model_view * vec4(vPosition, 1.0);
	vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	vec4 vColor4 = vec4(vColor.r, vColor.g, vColor.b, 1.0); 
  gl_Position = projection * model_view * vPosition4;

  color = vColor4;
} 
