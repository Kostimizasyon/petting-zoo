#include "Texture.h"

Texture::Texture(int widthImg,int heightImg,unsigned char* bytes) {

	glGenTextures(1, &ID); //how many textures we want to create, and a reference to the texture
	glActiveTexture(GL_TEXTURE0); //Slide our texture into the 0th index in our texture unit
	glBindTexture(GL_TEXTURE_2D, ID); //We bind out texture, giving its type and reference value

	//Our texture type, the setting we want to modify, and the value we want to give the setting

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //We say that we want to keep pixels as is for magnifiying and minifiying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //We say that both on the s and t axis (x,y) we want the iamge to repeat to wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//Now we generate the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	//type of texture, 0, type of color channels we want the texture to have, width, height, 0, type of color channels of our image, data type of pixels, image data itself 

	//GL_RGB => JPEG ;; GL_RGBA => PNG

	glGenerateMipmap(GL_TEXTURE_2D); //generint the mipmaps of the texture => smaller resolutions of the texture


}

void Texture::Bind() {

	glBindTexture(GL_TEXTURE_2D, ID);

}

void Texture::Unbind() {

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::Delete() {

	glDeleteTextures(1, &ID);

}