#version 330 core				//version once again
out vec4 FragColor;				//we will output the color of the vertex

//in vec3 color;					//MAKE SURE THEY ARE THE SAME NAME, we input a vec3 named color that we passed from our vert shader

//in vec2 texCoord;

//uniform sampler2D tex0;

void main()				
{
 //FragColor = texture(tex0, texCoord);
 FragColor = vec4(1.0,1.0,1.0,1.0);	
}