#version 330 core									//initilizing the openGL version
layout (location = 0) in vec3 aPos;					//we say that hey there is a layout for aPos a 3 dimensional vector;
	
//layout (location = 1) in vec3 aColor;				//now we say to expect a second set of data that will once again be 3 dimensonal, put we need to pass this into frag as it is the color part

//layout (location = 2) in vec2 aTex;					//now we need to pass the texture to the frag so it can colorise and render it

//out vec3 color;										//saying that we will output a 3dimensonal vector named color;

//out vec2 texCoord;						

//uniform float scale;								//A variable that can be acessed ANYWHERE
													//never declare unless u will use, as openGL will auto delete if u dont	
void main()
{
  // gl_Position = vec4(aPos.x * scale, aPos.y * scale, aPos.z * scale, 1.0);	 //OpenGL knows it needs to read gl_Position to take vector info.
  // color = aColor;															//we will pass color to the frag
  //  texCoord = aTex;

	gl_Position = vec4(aPos, 1.0);

}