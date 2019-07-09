#pragma once
#include <stack>
#include <fstream>
#include <sstream>
#include <iostream>

#include <gl\gl.h>		// need this file to do graphics with opengl
#include <gl\glu.h>		// need this file to set up a perspective projection easily
#include "GL\SOIL.h"

class Renderer
{
public:

	Renderer()
	{
		LoadTexture("Barbarian Head Front1.png", 1);
		LoadTexture("Barbarian Head Front2.png", 2);
		LoadTexture("Barbarian Head Front3.png", 3);
		LoadTexture("Barbarian Head Front4.png", 4);
		LoadTexture("Barbarian Head SideL1.png", 5);
		LoadTexture("Barbarian Head SideL2.png", 6);
		LoadTexture("Barbarian Head SideL3.png", 7);
		LoadTexture("Barbarian Head SideL4.png", 8);
		LoadTexture("Barbarian Head SideR1.png", 9);
		LoadTexture("Barbarian Head SideR2.png", 10);
		LoadTexture("Barbarian Head SideR3.png", 11);
		LoadTexture("Barbarian Head SideR4.png", 12);
		LoadTexture("Barbarian Head Back.png", 13);
		LoadTexture("Barbarian Torso Front.png", 14);
		LoadTexture("Barbarian ArmL.png", 15, 16, 32);
		LoadTexture("Barbarian ArmR.png", 16, 16, 32);
		LoadTexture("Barbarian LegL.png", 18, 16, 32);
		LoadTexture("Barbarian LegR.png", 19, 16, 32);
		LoadTexture("CharacterFinder.png", 20);
		LoadTexture("Blood.png", 21);
	}

	~Renderer()
	{
		DeleteTextures(19);
	}

	void Queue(Entity* Object)
	{
		DrawStack.push(Object);
	}

	void LoadTexture(std::string Name, GLuint ID, int width, int height)
	{
		Name = "Sprites\\" + Name;
		const char* FileName = Name.c_str();
		image = SOIL_load_image(FileName, &width, &height, 0, SOIL_LOAD_RGBA);
		glBindTexture(GL_TEXTURE_2D, ID);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenTextures(1, &ID);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void LoadTexture(std::string Name, GLuint ID)
	{
		LoadTexture(Name, ID, width, height);
	}

	void DeleteTextures(int tCount)
	{
		for (GLuint i = 1; i <= tCount; i++)
		glDeleteTextures(1, &i);
	}

	void Draw()
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glMatrixMode(GL_MODELVIEW);
		while (!DrawStack.empty())
		{
			Object = DrawStack.top();
			ObjectPosition = Object->GetPosition();
			ObjectRotation = Object->GetRotation();

			glLoadIdentity();
			glPushMatrix();
			if (Object->Parent != NULL && Object->Name != "ARROW")
			{
				ParentPosition = Object->Parent->GetPosition();
				ParentRotation = Object->Parent->GetRotation();
				glTranslatef(ParentPosition[0] + ObjectPosition[0], ParentPosition[1] + ObjectPosition[1], ParentPosition[2] + ObjectPosition[2]);
				glRotatef(ParentRotation[0] + ObjectRotation[0], ParentRotation[1] + ObjectRotation[1], ParentRotation[2] + ObjectRotation[2], ParentRotation[3] + ObjectRotation[3]);
			}
			else
			{
				glTranslatef(ObjectPosition[0], ObjectPosition[1], ObjectPosition[2]);
				glRotatef(ObjectRotation[0], ObjectRotation[1], ObjectRotation[2], ObjectRotation[3]);
			}
			

			if (Object->Visuals.Texture != 0)
			{
				glBindTexture(GL_TEXTURE_2D, Object->Visuals.Texture);
				glBegin(GL_QUADS);

				glTexCoord2f(0, 0);
				glVertex2f(Object->Visuals.VertexData[0], Object->Visuals.VertexData[1]);
				glTexCoord2f(1.0f, 0);
				glVertex2f(Object->Visuals.VertexData[2], Object->Visuals.VertexData[3]);
				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(Object->Visuals.VertexData[4], Object->Visuals.VertexData[5]);
				glTexCoord2f(0, 1.0f);
				glVertex2f(Object->Visuals.VertexData[6], Object->Visuals.VertexData[7]);

				glEnd();
			}
			glPopMatrix();

			DrawStack.pop();

		}
		glDisableClientState(GL_VERTEX_ARRAY);
		
	}

private:
	std::stack<Entity*> DrawStack;
	Entity* Object;
	
	const float* ParentPosition;
	const float* ObjectPosition;

	const float* ParentRotation;
	const float* ObjectRotation;

	int width = 32, height = 32;
	unsigned char* image;


};

