/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

#version 130  

in  vec4 color;
out vec4 fColor;

void main() 
{ 
    fColor = color;
} 

