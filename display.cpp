float lightX = -2.;
float lightY = 7.;
float lightZ = -1.;
unsigned int depthMapFBO;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMap;

void
InitGraphics( )
{`
. . .
	//Create a framebuffer object
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);

	//Create a texture that will be the framebuffer's depth buffer
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//Attach texture to framebuffer as depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

	//Force OpenGL to accept the framebuffer that doesn't have a color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
. . .
}

void
Display( )
{
	glutSetWindow( MainWindow );
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );

	glEnable( GL_NORMALIZE );
	glm::vec3 lightPos(lightX, lightY, lightZ); //Some arbitrary thing for now

	//First pass, render from light's perspective, store depth of scene in texture
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	//Might have to do perspective instead of ortho??

	//lightPos.x = 3 * sin(Time) * lightX;
	//lightPos.y = 2 * cos(Time) * lightY;
	//lightPos.z = 5.0 + cos(Time);

	//These matrices are the equivalent of projection and view matrices
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));

	//This matrix is the transformation matrix that the vertex shader will use
	//instead of glModelViewProjectionMatrix
	lightSpaceMatrix = lightProjection * lightView;

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	Pattern->Use();
	Pattern->SetUniformVariable("lightSpaceMatrix", lightSpaceMatrix);
	//This matrix is going to be the model coordinate matrix for all the objects
	glm::mat4 mod = glm::mat4(1.);
	Pattern->SetUniformVariable("model", mod);
	
	//Render Sphere
	MjbSphere(5, 30, 30);

	//Render cubes:
	mod = glm::mat4(1.);
	mod = glm::translate(mod, glm::vec3(0., 6.5 + 2 * sin(3.14 * Time), 0.));
	mod = glm::scale(mod, glm::vec3(0.5));
	mod = glm::transpose(mod);
	Pattern->SetUniformVariable("model", mod);
	glutSolidCube(1.);

	mod = glm::mat4(1.f);
	float angle = 45. * 2 * sin(3.14 * Time);
	mod = glm::translate(mod, glm::vec3(2.0f, 6.0f, -1.0));
	mod = glm::rotate(mod, glm::radians(angle), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	mod = glm::scale(mod, glm::vec3(0.5f));
	mod = glm::transpose(mod);
	Pattern->SetUniformVariable("model", mod);
	glutSolidCube(2.);

	Pattern->Use(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//reset viewport
	GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = (vx - v) / 2;
	GLint yb = (vy - v) / 2;
	glViewport(xl, yb, v, v);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render scene normally with depth map to render shadows
	ShadowMapping->Use();
	ShadowMapping->SetUniformVariable("lightX", lightX);
	ShadowMapping->SetUniformVariable("lightY", lightY);
	ShadowMapping->SetUniformVariable("lightZ", lightZ);
	ShadowMapping->SetUniformVariable("lightSpaceMatrix", lightSpaceMatrix);

	mod = glm::mat4(1.);
	ShadowMapping->SetUniformVariable("model", mod);

	//Render Sphere
	MjbSphere(5, 30, 30);
	
	//Render cubes:
	mod = glm::mat4(1.);
	
	mod = glm::translate(mod, glm::vec3(0., 6.5 + 2*sin(3.14*Time), 0.));
	mod = glm::scale(mod, glm::vec3(0.5));
	mod = glm::transpose(mod);
	ShadowMapping->SetUniformVariable("model", mod);
	glutSolidCube(1.);

	mod = glm::mat4(1.);
	
	mod = glm::translate(mod, glm::vec3(2.0f, 6.0f, -1.0));
	mod = glm::rotate(mod, glm::radians(angle), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	mod = glm::scale(mod, glm::vec3(0.5f));
	mod = glm::transpose(mod);
	ShadowMapping->SetUniformVariable("model", mod);
	glutSolidCube(2.);

	ShadowMapping->Use(0);
	glutSwapBuffers( );
	glFlush( );
}
