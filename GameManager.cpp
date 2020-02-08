#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define PI 3.14159265
const int NumVertices = 342;

GLfloat radius = 0.25;
GLfloat rotatingDegree = 0.0;

point4 PlanetSystem[NumVertices];
color4 colors[NumVertices];

GLuint ww = 500;
GLuint wh = 500;

GLuint vao;

point4 centerPoint = point4(0.0, 0.0, 0.0, 1.0);

// RGBA colors
color4 vertex_colors[6] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(135.0/255, 206.0/255, 250.0/255, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
};

GLuint  thetaIndex;  // The location of the "theta" shader uniform variable
GLuint translateRotatingValue;

GLfloat  ThetaValue[3] = { 0.0, 0.0, 0.0 };
GLfloat  TranslateRotatingValue[3] = { 0.0, 0.0, 0.0 };

int Index = 0;
//----------------------------------------------------------------------------
void drawNeptune()
{
	for (float phi = -80.0; phi <= 80.0; phi += 20.0)
	{
		float phir = phi * DegreesToRadians;
		float phir20 = (phi + 20.0)*DegreesToRadians;
		for (float theta = -180.0; theta <= 180.0; theta += 20.0)
		{
			float thetar = theta * DegreesToRadians;
			PlanetSystem[Index] = point4(sin(thetar)*cos(phir)*radius,
				cos(thetar)*cos(phir)*radius, sin(phir)*radius, 1.0f);
			colors[Index] = vertex_colors[4];
			Index++;
			PlanetSystem[Index] = point4(sin(thetar)*cos(phir20)*radius,
				cos(thetar)*cos(phir20)*radius, sin(phir20)*radius, 1.0);
			colors[Index] = vertex_colors[4];
			Index++;
		}
	}
}


void fillPointsandColors()
{
	drawNeptune();


}

//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
	fillPointsandColors();

	//printf("Pressing left, respectively.\n");;
	// Create a vertex array object
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PlanetSystem) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(PlanetSystem), PlanetSystem);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(PlanetSystem), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(PlanetSystem)));

	thetaIndex = glGetUniformLocation(program, "theta");
	translateRotatingValue = glGetUniformLocation(program, "translateRotatingValue");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glUniform3fv(thetaIndex, 1, ThetaValue);
	glUniform3fv(translateRotatingValue, 1, TranslateRotatingValue);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 342);
//	glDrawArrays(GL_TRIANGLE_FAN, 342, 40);

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:     
			
			if (rotatingDegree > 360) rotatingDegree = 0;

			//Rotate
			ThetaValue[0] = rotatingDegree;
			ThetaValue[1] = 0;
			ThetaValue[2] = rotatingDegree;
			rotatingDegree += 5;

			TranslateRotatingValue[0] = sin(rotatingDegree * DegreesToRadians)/1.25;
			TranslateRotatingValue[1] = cos(rotatingDegree * DegreesToRadians)/1.25;
			TranslateRotatingValue[2] = 0;


			glutPostRedisplay();
			break;
		case GLUT_MIDDLE_BUTTON:   break;
		case GLUT_RIGHT_BUTTON:    break;
		}
	}
}

//----------------------------------------------------------------------------

void reshapeFunc(GLsizei w, GLsizei h)
{
	/* adjust clipping box */


	/* adjust viewport and clear */

	glViewport(0, 0, ww*h / wh, h);

	/* set global size for use by drawing routine */

	//ww = w;
	//wh = h;
	//resetGame();
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(ww, wh);

	// set OpenGL context to 3.1 or 3.2 and 
	// set profile to core
	// deprecated functions will not be available in core profile
	// the other option is GLUT_COMPATIBILITY PROFILE which let's us use deprecated functionality if
	// vendor implementation provide it
	//glutInitContextVersion(3, 1);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	//create graphics window
	glutCreateWindow("Planet System");

	//include the following statement due to an error in GLEW library
	glewExperimental = GL_TRUE;

	glewInit();

	init();
	glutReshapeFunc(reshapeFunc);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();
	return 0;
}