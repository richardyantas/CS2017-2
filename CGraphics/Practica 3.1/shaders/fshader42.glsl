/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

#version 130  

in  vec4 color;
out vec4 fColor;


/* Fog */
in vec4 Eye;
in vec2 texCoord;

uniform int fog_flag;

float density = 0.09;
float fogStart = 0.0;
float fogEnd = 18.0;

uniform sampler2D texture_2D;
//uniform sampler1D texture_1D;

uniform int text_ground_flag;

uniform vec4 fogColor = vec4(0.7, 0.7, 0.7, 0.5);


float getFogFactor_linear(float fogCoord) {
	float result;
	if (fogStart <= fogCoord && fogCoord <= fogEnd) {
		result = 1.0 - (fogEnd - fogCoord) / (fogEnd - fogStart);
	}
	else if (fogCoord < fogStart) result = 0.0;
	else if (fogCoord > fogEnd) result = 1.0;
	return result;
}

float getFogFactor_exp(float fogCoord) {
	float result = exp(-density * fogCoord);
	result = clamp(1.0 - result, 0.0, 1.0);
	return result;
}

float getFogFactor_exp2(float fogCoord) {
	float exp_aux = (density * fogCoord) * (density * fogCoord);
	float result = exp(-exp_aux);
	result = clamp(1.0 - result, 0.0, 1.0);
	return result;
}



void main() 
{ 
	float fogCoord = abs(Eye.z/Eye.w);
    vec4 aux_color = color;
    

	if (text_ground_flag == 1){		
		aux_color = fColor*texture(texture_2D,texCoord);	
	}
	
    if (fog_flag == 0)
    	fColor = aux_color;
  	else if (fog_flag == 1)
  		fColor = mix(aux_color, fogColor, getFogFactor_linear(fogCoord));  	
  	else if (fog_flag == 2)
    	fColor = mix(aux_color, fogColor, getFogFactor_exp(fogCoord));
    else
    	fColor = mix(aux_color, fogColor, getFogFactor_exp2(fogCoord));
} 
