/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

// #version 150  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version
#version 130

in  vec4 color;
out vec4 fColor;

void main() 
{ 
    fColor = color;
} 

