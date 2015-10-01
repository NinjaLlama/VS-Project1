/*

manyModelsStatic.cpp

465 utility include files:  shader465.hpp, triModel465.hpp  

Shaders:  simpleVertex.glsl and simpleFragment.glsl
  provide flat shading with a fixed light position

C OpenGL Core 3.3 example that loads "nModels" *.tri model files 
and displays them in at static locations with a static view.
Demonstrates use of triModel465.hpp functions for loading *.tri models.

display() updated to comment out unneeded statements like:
glEnableVertexAttribArray( vPosition[m] );

These lines are not needed even when the models are moving.

Mike Barnes
10/23/2014
*/

# define __Windows__
# include "../includes465/include465.hpp"
# define __INCLUDES465__ 

const int X = 0, Y = 1, Z = 2 ,W=3, START = 0, STOP = 1;
// constants for models:  file names, vertex count, model display size
const int nModels = 8;  // number of models in this scene
//I added the ship I made and replace xyz coordinate plane with a model of the sun with the x coordinate in white, the y coordinate in green and z in blue
char * modelFile[nModels] = {"Ruber.tri", "Unum.tri ","Duo.tri", "Primus.tri",
"Segundus.tri", "BattleCruiser.tri", "sphere-r50.tri", "axes-r100.tri" };
float modelBR[nModels];       // model's bounding radius
float scaleValue[nModels];    // model's scaling "size" value
const int nVertices[nModels] = {264 *3, 264 * 3, 278 * 3, 264 * 3, 264 * 3, 2772 * 3, 264 * 3, 120 * 3};
char * vertexShaderFile   = "simpleVertex.glsl";     
char * fragmentShaderFile = "simpleFragment.glsl";    
GLuint shaderProgram; 
GLuint VAO[nModels];      // Vertex Array Objects
GLuint buffer[nModels];   // Vertex Buffer Objects

// vectors and values for lookAt
glm::vec3 eye, at, up;

//camera view booleans
bool cameraFront = false; 
bool cameraTop = true; //starts with front camera, switch to top when 'v' is pressed
bool cameraShip = false;
bool cameraUnum = false;
bool cameraDuo = false;

// window title strings
char baseStr[50] = "465 Project 1 : ";
char viewStr[15] = "  View Front";
char warbirdStr[15] = "  Warbird ??";
char unumStr[15] = "  Unum ?";
char secundusStr[15] = "  Secundus ?";
char fpsStr[15] = "  F/S ??";
char updateStr[20] = "  U/S ??";
char titleStr[500];

// rotation variables  -- the modelMatrix
GLfloat rotateRadianOne = 0.0f;
GLfloat rotateRadianTwo = 0.0f;
glm::mat4 identity(1.0f);
glm::mat4 rotationOne;  // the modelMatrix
glm::mat4 rotationTwo;
int timerDelay = 40, frameCount = 0;
double currentTime, lastTime, timeInterval;
bool idleTimerFlag = false;  // interval or idle timer ?
bool wireFrame = false;     // initially show surfaces

// Shader handles, matrices, etc
GLuint MVP ;  // Model View Projection matrix's handle
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models
// model, view, projection matrices and values to create modelMatrix.
//loaded in order of Ruber, Umun, Duo, Primus, Secundus, Warbird, missiles
float modelSize[nModels] = { 2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 500.0f, 25.0f, 1000.0f};   // size of model
glm::vec3 scale[nModels];       // set in init()
glm::vec3 translate[nModels] = {glm::vec3(0,0,0), glm::vec3(4000, 0, 0), glm::vec3(9000, 0, 0),
glm::vec3(900, 0, 0), glm::vec3(1750, 0, 0),
glm::vec3(5000, 1000, 5000), glm::vec3(4900, 1000, 4850), glm::vec3(9000, 0, 0)


};
glm::mat4 modelMatrix;          // set in display()
glm::mat4 DuoMatrix;
glm::vec3 DuoTranslate;
glm::mat4 axesMatrix;
glm::vec3 axesTranslate;
glm::mat4 DuoRotation = glm::rotate(identity, PI, glm::vec3(0, 1, 0));
glm::mat4 axesRotation = glm::rotate(identity, PI, glm::vec3(0, 1, 0));
glm::mat4 UnumMatrix;
glm::vec3 UnumTranslate;
glm::mat4 viewMatrix;           // set in init()
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 ModelViewProjectionMatrix; // set in display();


void reshape(int width, int height) {
  float aspectRatio = (float) width / (float) height;
  float FOVY = glm::radians(60.0f);
  glViewport(0, 0, width, height);
  printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n", 
    FOVY, width, height, aspectRatio);
  projectionMatrix = glm::perspective(FOVY, aspectRatio, 1.0f, 100000.0f); 
  }

// update and display animation state in window title
void updateTitle() {
	strcpy(titleStr, baseStr);
	strcat(titleStr, warbirdStr);
	strcat(titleStr, unumStr);
	strcat(titleStr, secundusStr);
	strcat(titleStr, updateStr);
	strcat(titleStr, fpsStr);
	strcat(titleStr, viewStr);
	 //printf("title string = %s \n", titleStr);
	glutSetWindowTitle(titleStr);
}


void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // update model matrix
  for(int m = 0; m < nModels; m++) {
	  if (m == 1) //Unum orbits Ruber
	  {
		  modelMatrix = rotationOne * glm::translate(glm::mat4(), translate[m]) *
			  glm::scale(glm::mat4(), glm::vec3(scale[m]));
		  UnumMatrix = modelMatrix;
		  UnumTranslate = glm::vec3(UnumMatrix[3]);
	  }
	  else if (m == 2) //Duo orbits Ruber
	  {
		  modelMatrix = rotationTwo * DuoRotation*glm::translate(glm::mat4(), translate[m]) *
			  glm::scale(glm::mat4(), glm::vec3(scale[m]));
		  DuoMatrix = modelMatrix;
		  DuoTranslate = glm::vec3(DuoMatrix[3]);
		  //showVec3("Duo", DuoTranslate); //for debugging purposes
	  }
	  else if (m == 3) //Primus orbits Duo
	  {

		  modelMatrix = glm::translate(glm::mat4(), DuoTranslate) * rotationOne * glm::translate(glm::mat4(), translate[m]) *
			  glm::scale(glm::mat4(), glm::vec3(scale[m]));
			  //* glm::translate(glm::mat4(), DuoTranslate);
	  }
	  else if (m == 4) //Secundus orbits Duo
	  {
		  modelMatrix = glm::translate(glm::mat4(), DuoTranslate) * rotationTwo * glm::translate(glm::mat4(), translate[m]) *
			  glm::scale(glm::mat4(), glm::vec3(scale[m]));
	  }
	  else if (m == 7) //Duo orbits Ruber
	  {
		  modelMatrix = rotationTwo * axesRotation*glm::translate(glm::mat4(), translate[m]) *
			  glm::scale(glm::mat4(), glm::vec3(scale[m]));
		  axesMatrix = modelMatrix;
		  axesTranslate = glm::vec3(DuoMatrix[3]);
	  }
	  else
	  {
		  modelMatrix = glm::translate(glm::mat4(), translate[m]) *
			  glm::scale(glm::mat4(), glm::vec3(scale[m]));
	  }
	//dynamic cameras
	  //~~~ my camera logic is messed up, how to fix???? ~~~
	  //Unum camera
	  if (cameraDuo) //if cameraDuo is true, it means Unum is the current camera view and the Duo camera view is the next to be toggled
	  {
		  glm::vec3 zUnum = glm::vec3(UnumMatrix[0][2], UnumMatrix[0][1] * -1, UnumMatrix[0][0] * -1);
		  zUnum = glm::normalize(zUnum);     // camera is on XZ plane
		  eye = UnumTranslate + zUnum * 4000.0f;                   // camera is looking at Unum
		  at = UnumTranslate;
		  up = glm::vec3(0.0f, 1.0f, 0.0f);                    // camera's up is Y
		  viewMatrix = glm::lookAt(eye, at, up);
	  }
	  //Duo camera
	  if (cameraFront) //if cameraFront is true, it means Duo is the current camera view and the front camera view is the next to be toggled
	  {
		  //glm::vec3 zDuo = glm::vec3(DuoMatrix[0][2] *-1, DuoMatrix[1][2] *-1, DuoMatrix[2][2] *-1);
		  //90 degrees CW about y-axis: (x, y, z) -> (-z, y, x) -- this is how to get z-axis from x-axis --
		  //glm::vec3(DuoMatrix[0][0] * -1, DuoMatrix[1][0] * -1, DuoMatrix[2][0] * -1)
		  //glm::vec3 zDuo = glm::vec3(DuoTranslate - glm::vec3(DuoMatrix[0]*-1.0f));
		  //glm::vec3 zDuo = glm::vec3(DuoMatrix[0] * -1.0f);
		  glm::vec3 zDuo = glm::vec3(DuoMatrix[0][2], DuoMatrix[0][1]*-1, DuoMatrix[0][0]*-1);
		  //showVec3("Duo", zDuo);
		  zDuo = glm::normalize(zDuo);
		  //DuoMatrix[2], DuoMatrix[6], DuoMatrix[10]
		  //eye = glm::vec3(xCoordinateDuo, 0.0f, zCoordinateDuo + 4000.0f);     // camera is on XZ plane
		  eye = DuoTranslate + zDuo * 4000.0f;
		  //at = glm::vec3(xCoordinateDuo, 0.0f, 0.0f);                    // camera is looking at Duo
		  at = DuoTranslate;
		  up = glm::vec3(0.0f, 1.0f, 0.0f);                    // camera's up is Y
		  viewMatrix = glm::lookAt(eye, at, up);
	  }

    // glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr( modelMatrix)); 
    ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix; 
    glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
	glBindVertexArray(VAO[m]);
	/*  The following 3 lines are not needed !
    glEnableVertexAttribArray( vPosition[m] );
    glEnableVertexAttribArray( vColor[m] );
    glEnableVertexAttribArray( vNormal[m] );
	*/
    glDrawArrays(GL_TRIANGLES, 0, nVertices[m] ); 
    }
  glutSwapBuffers();
  frameCount++;
  // see if a second has passed to set estimated fps information
  currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
  timeInterval = currentTime - lastTime;
  if (timeInterval >= 1000) {
	  sprintf(fpsStr, "  F/S %4d", (int)(frameCount / (timeInterval / 1000.0f)));
	  sprintf(updateStr, "  U/S %4d", (int)(frameCount / (timeInterval / 1000.0f)));
	  lastTime = currentTime;
	  frameCount = 0;
	  updateTitle();
  }
  }

// for use with Idle and intervalTimer functions 
// to set rotation
void update(void){
	rotateRadianOne += 0.004f;
	if (rotateRadianOne >  2 * PI) rotateRadianOne = 0.0f;
	rotationOne = glm::rotate(identity, rotateRadianOne, glm::vec3(0, 1, 0));
	rotateRadianTwo += 0.002f;
	if (rotateRadianTwo >  2 * PI) rotateRadianTwo = 0.0f;
	rotationTwo = glm::rotate(identity, rotateRadianTwo, glm::vec3(0, 1, 0));
	glutPostRedisplay();
}

// Estimate FPS, use for fixed interval timer driven animation
void intervalTimer(int i) {
	glutTimerFunc(timerDelay, intervalTimer, 1);
	if (!idleTimerFlag) update();  // fixed interval timer
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;
	/*case 'a': case 'A':  // change animation timer
		// printf("%s   %s\n", updateStr, fpsStr);
		if (idleTimerFlag) { // switch to interval timer  
			glutIdleFunc(NULL);
			strcpy(updateStr, " interval timer");
			idleTimerFlag = false;
		}
		else   {         // switch to idle timer
			glutIdleFunc(update);
			strcpy(updateStr, " idle timer");
			idleTimerFlag = true;
		}
		break;*/

	case 'v': case 'V' : //toggle camera
		if (cameraFront)
		{
			eye = glm::vec3(0.0f, 10000.0f, 20000.0f);   // camera's position
			at = glm::vec3(0);            // position camera is looking at
			up = glm::vec3(0.0f, 1.0f, 0.0f);            // camera'a up vector
			viewMatrix = glm::lookAt(eye, at, up);
			strcpy(viewStr, " View Front");
			cameraFront = false;
			cameraTop = true;
			break;
		}
		else if (cameraTop)
		{
			eye = glm::vec3(0.0f, 20000.0f, 0.0f);     // camera is on Z and above origin
			at = glm::vec3(0);                    // camera is looking at origin
			up = glm::vec3(0.0f, 0.0f, -1.0f);                    // camera's up is -Z
			viewMatrix = glm::lookAt(eye, at, up);
			strcpy(viewStr, " View Top");
			cameraTop = false;
			cameraShip = true;
			break;
		}
		else if (cameraShip)
		{
			eye = glm::vec3(5000.0f, 300.0f, 1000.0f);     // camera is up and behind the ship
			at = glm::vec3(5000.0f, 1000.0f, 5000.0f);                    // camera is looking at warbird
			up = glm::vec3(0.0f, 300.0f, 0.0f);                    // camera looking over the ship
			viewMatrix = glm::lookAt(eye, at, up);
			strcpy(viewStr, " View Warbird");
			cameraShip = false;
			cameraUnum = true;
			break;
		}
		else if (cameraUnum)
		{
			eye = glm::vec3(4000.0f, 0.0f, 4000.0f);     // camera is on XZ plane
			at = glm::vec3(4000.0f, 0.0f, 0.0f);                    // camera is looking at Unum
			up = glm::vec3(0.0f, 1.0f, 0.0f);                    // camera's up is Y
			viewMatrix = glm::lookAt(eye, at, up);
			strcpy(viewStr, " View Unum");
			cameraUnum = false;
			cameraDuo = true;
			break;
		}
		else
		{
			eye = glm::vec3(9000.0f, 0.0f, 4000.0f);     // camera is on XZ plane
			at = glm::vec3(9000.0f, 0.0f, 0.0f);                    // camera is looking at Duo
			up = glm::vec3(0.0f, 1.0f, 0.0f);                    // camera's up is Y
			viewMatrix = glm::lookAt(eye, at, up);
			strcpy(viewStr, " View Duo");
			cameraDuo = false;
			cameraFront = true;
			break;
		}

		break;
	
	/*case 'f': case 'F':  // front view
		eye = glm::vec3(0.0f, 10000.0f, 20000.0f);   // camera's position
		at = glm::vec3(0);            // position camera is looking at
		up = glm::vec3(0.0f, 1.0f, 0.0f);            // camera'a up vector
		viewMatrix = glm::lookAt(eye, at, up);
		strcpy(viewStr, " front view"); break;
	case 't': case 'T':  // top view
		eye = glm::vec3(0.0f, 20000.0f, 0.0f);     // camera is on Z and above origin
		at = glm::vec3(0);                    // camera is looking at origin
		up = glm::vec3(0.0f, 0.0f, -1.0f);                    // camera's up is -Z
		viewMatrix = glm::lookAt(eye, at, up);
		strcpy(viewStr, " top view"); break;
	case 'l': case 'L':  // front and right view
		eye = glm::vec3(5000.0f, 5000.0f, 20000.0f);     // camera is on Z and above origin
		at = glm::vec3(0);                    // camera is looking at origin
		up = glm::vec3(0.0f, 1.0f, 0.0f);                    // camera's up is Y
		viewMatrix = glm::lookAt(eye, at, up);
		strcpy(viewStr, " top view"); break;*/
	}
	updateTitle();
}

// load the shader programs, vertex data from model files, create the solids, set initial view
void init() {
  // load the shader programs
  shaderProgram = loadShaders(vertexShaderFile,fragmentShaderFile);
  glUseProgram(shaderProgram);
  
  // generate VAOs and VBOs
  glGenVertexArrays( nModels, VAO );
  glGenBuffers( nModels, buffer );
  // load the buffers from the model files
  for (int i = 0; i < nModels; i++) {
    modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
      vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal"); 
    // set scale for models given bounding radius  
    scale[i] = glm::vec3( modelSize[i] * 1.0f/modelBR[i]);
    }
    
  MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");

  // initially use a front view
  eye = glm::vec3(0.0f, 10000.0f, 20000.0f);   // camera's position
  at = glm::vec3(0);            // position camera is looking at
  up = glm::vec3(0.0f, 1.0f, 0.0f);            // camera'a up vector

  viewMatrix = glm::lookAt(eye, at, up);
	

  // set render state values
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
  }

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutCreateWindow("465 Project 1");
  // initialize and verify glew
  glewExperimental = GL_TRUE;  // needed my home system 
  GLenum err = glewInit();  
  if (GLEW_OK != err) 
      printf("GLEW Error: %s \n", glewGetErrorString(err));      
    else {
      printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
      printf("OpenGL %s, GLSL %s\n", 
        glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION));
      }
  // initialize scene
  init();
  // set glut callback functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(NULL);  // start with intervalTimer
  glutTimerFunc(timerDelay, intervalTimer, 1);
  glutMainLoop();
  printf("done\n");
  return 0;
  }
  

