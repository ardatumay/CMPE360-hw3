//-----------------------------------------------------
// Title: OPENGL
// Author: ARDA TÜMAY
// ID: 13180171934
// Section: 1
// Assignment: 3
// Description: This class is a main class for creating teapot and room around it.
//-----------------------------------------------------

#include "GL/freeglut.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "vecmath.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Globals
GLfloat Lt0pos[] = { 1.0f, 1.0f, 5.0f, 1.0f };
GLfloat vertex1[] = { -2.3, -1.0, -1.1 }; //quad back, quad left
GLfloat vertex2[] = { -2.3, 1.6, -1.1 }; //quad back, quad left
GLfloat vertex3[] = { 2.42, 1.62, -1.14 }; //quad back, quad right
GLfloat vertex4[] = { 2.4, -1.0, -1.1 }; //quad back, quad right
GLfloat vertex5[] = { -1.9, 1.4, 3.4 }; //quad left
GLfloat vertex6[] = { -1.9, -1.0, 3.4 }; //quad left
GLfloat vertex7[] = { 2.82, -1.0, 3.4 }; //quad right
GLfloat vertex8[] = { 2.82, 1.4, 3.4 }; //quad right
GLdouble cameraCenter[] = { 0.0, 0.0, 5.0 };
int iCount = 0, kCount = 0, jCount = 0, lCount = 0, uCount = 0, oCount = 0;

GLuint texture[3];
struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};

typedef struct Image Image;
#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;

// You will need more global variables to implement color and position changes

// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
void makeCheckImage(void) {
	int i, j, c;
	for (i = 0; i < checkImageWidth; i++) {
		for (j = 0; j < checkImageHeight; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte) c;
			checkImage[i][j][1] = (GLubyte) c;
			checkImage[i][j][2] = (GLubyte) c;
		}
	}
}
int ImageLoad1(char *filename, Image *image) {
	FILE *file;
	unsigned long size; // size of the image in bytes.
	unsigned long i; // standard counter.
	unsigned short int planes; // number of planes in image (must be 1)
	unsigned short int bpp; // number of bits per pixel (must be 24)
	char temp; // temporary color storage for bgr-rgb conversion.
	// make sure the file is there.
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}
	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);
	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
	//printf("Width of %s: %lu\n", filename, image->sizeX);
	// read the height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	//printf("Height of %s: %lu\n", filename, image->sizeY);
	// calculate the size (assuming 24 bits or 3 bytes per pixel)
	size = image->sizeX * image->sizeY * 3;
	// read the planes
	if ((fread(&planes, 2, 1, file)) != 1) {

		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
	}
	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);
		return 0;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}
	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);
	// read the data.
	image->data = (char *) malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	// we're done.
	return 1;

}
int ImageLoad2(char *filename, Image *image) {
	FILE *file;
	unsigned long size; // size of the image in bytes.
	unsigned long i; // standard counter.
	unsigned short int planes; // number of planes in image (must be 1)
	unsigned short int bpp; // number of bits per pixel (must be 24)
	char temp; // temporary color storage for bgr-rgb conversion.
	// make sure the file is there.
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}
	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);
	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
	//printf("Width of %s: %lu\n", filename, image->sizeX);
	// read the height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	//printf("Height of %s: %lu\n", filename, image->sizeY);
	// calculate the size (assuming 24 bits or 3 bytes per pixel)
	size = image->sizeX * image->sizeY * 3;
	// read the planes
	if ((fread(&planes, 2, 1, file)) != 1) {

		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
	}
	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);
		return 0;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}
	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);
	// read the data.
	image->data = (char *) malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 1];
		image->data[i + 1] = temp;
	}
	// we're done.
	return 1;

}

Image * loadTexture1() {
	Image *image1;
	// allocate space for texture
	image1 = (Image *) malloc(sizeof(Image));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	if (!ImageLoad1("C:/Users/User/workspaceC++/cmpe360_hw3/wall.bmp", image1)) {
		exit(1);
	}
	return image1;
}
Image * loadTexture2() {
	Image *image2;
	// allocate space for texture
	image2 = (Image *) malloc(sizeof(Image));
	if (image2 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	if (!ImageLoad2("C:/Users/User/workspaceC++/cmpe360_hw3/TEDU.bmp", image2)) {
		exit(1);
	}
	return image2;
}

inline void glVertex(const Vector3f &a) {
	glVertex3fv(a);
}

inline void glNormal(const Vector3f &a) {
	glNormal3fv(a);
}

// This function is called whenever a "Normal" key press is received.
void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // Escape key
		exit(0);
		break;
	case 'c':
		// add code to change color here
		cout << "Unhandled key press " << key << "." << endl;
		break;
	case 'w':
		if (cameraCenter[1] >= 4.5) {
			break;
		} else {
			cameraCenter[1] += 0.3;
			break;
		}
	case 'a':
		if (cameraCenter[0] <= -4.5) {
			break;
		} else {
			cameraCenter[0] -= 0.3;
			break;
		}
	case 's':
		if (cameraCenter[1] <= -4.5) {
			break;
		} else {
			cameraCenter[1] -= 0.3;
			break;
		}
	case 'q':
		//if (cameraCenter[2] >= 10) {
		//	break;
		//} else {
			cameraCenter[2] += 0.3;
			break;
		//}
	case 'e':
		//if (cameraCenter[2] <= -4.5) {
		//	break;
		//} else {
			cameraCenter[2] -= 0.3;
			break;
		//}
	case 'd':
		if (cameraCenter[0] >= 4.5) {
			break;
		} else {
			cameraCenter[0] += 0.3;
			break;
		}
	case 'i':
		vertex6[1] += 0.1f;
		iCount++;
		break;
	case 'k':
		vertex6[1] -= 0.1f;
		kCount++;
		break;
	case 'j':
		vertex6[0] -= 0.1f;
		jCount++;
		break;
	case 'l':
		vertex6[0] += 0.1f;
		lCount++;
		break;
	case 'u':
		vertex6[2] -= 0.1f;
		uCount++;
		break;
	case 'o':
		vertex6[2] += 0.1f;
		oCount++;
		break;
	case 'p':
		cout << "i count: " << iCount << endl;
		cout << "k count: " << kCount << endl;
		cout << "j count: " << jCount << endl;
		cout << "l count: " << lCount << endl;
		cout << "u count: " << uCount << endl;
		cout << "o count: " << oCount << endl;
		GLfloat x, y, z;
		x = -jCount * (0.1f) + lCount * (0.1f);
		y = iCount * (0.1f) - kCount * (0.1f);
		z = oCount * (0.1f) - uCount * (0.1f);
		cout << "x: " << x << endl;
		cout << "y: " << y << endl;
		cout << "z: " << z << endl;
		break;
	default:
		cout << "Unhandled key press " << key << "." << endl;
	}

	// this will refresh the screen so that the user sees the color change
	glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		// add code to change light position
		//cout << "Unhandled key press: up arrow." << endl;
		Lt0pos[1] += 0.3f;
		break;
	case GLUT_KEY_DOWN:
		// add code to change light position
		//cout << "Unhandled key press: down arrow." << endl;
		Lt0pos[1] -= 0.3f;
		break;
	case GLUT_KEY_LEFT:
		// add code to change light position
		//cout << "Unhandled key press: left arrow." << endl;
		Lt0pos[0] -= 0.3f;
		break;
	case GLUT_KEY_RIGHT:
		// add code to change light position
		//cout << "Unhandled key press: right arrow." << endl;
		Lt0pos[0] += 0.3f;
		break;
	}

	// this will refresh the screen so that the user sees the light position
	glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void) {
	int i;

	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rotate the image
	glMatrixMode( GL_MODELVIEW);  // Current matrix affects objects positions
	glLoadIdentity();              // Initialize to the identity

	// Position the camera at [0,0,5], looking at [0,0,0],
	// with [0,1,0] as the up direction.

	gluLookAt(cameraCenter[0], cameraCenter[1], cameraCenter[2], 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);

	// Set material properties of object

	// Here are some colors you might use - feel free to add more
	GLfloat diffColors[4][4] = { { 0.5, 0.5, 0.9, 1.0 }, { 0.9, 0.5, 0.5, 1.0 },
			{ 0.5, 0.9, 0.3, 1.0 }, { 0.3, 0.8, 0.9, 1.0 } };

	// Here we use the first color entry as the diffuse color
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[0]);

	// Define specular color and shininess
	GLfloat specColor[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess[] = { 100.0 };

	// Note that the specular color and shininess can stay constant
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	// Set light properties

	// Light color (RGBA)
	GLfloat Lt0diff[] = { 1.0, 1.0, 1.0, 1.0 };
	// Light position
	//GLfloat Lt0pos[] = {1.0f, 1.0f, 5.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// This GLUT method draws a teapot.
	//glDisable(GL_TEXTURE_2D);


	//glBindTexture(GL_TEXTURE_2D,0);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.5,0.5,0.0);
	glRotatef(270,0.0,0.0,1.0);
	glTranslatef(-0.5,-0.5,0.0);
	glMatrixMode(GL_MODELVIEW);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glutSolidTeapot(1.0);
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glLoadIdentity();
	glPushMatrix();
	glutSolidSphere(1,20,20);
	glTranslatef(10,3.8,9);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(vertex1[0], vertex1[1], vertex1[2]);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(vertex2[0], vertex2[1], vertex2[2]);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(vertex3[0], vertex3[1], vertex3[2]);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(vertex4[0], vertex4[1], vertex4[2]);
	glEnd();


	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(vertex1[0], vertex1[1], vertex1[2]);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(vertex2[0], vertex2[1], vertex2[2]);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(vertex5[0], vertex5[1], vertex5[2]);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(vertex6[0], vertex6[1], vertex6[2]);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(vertex3[0], vertex3[1], vertex3[2]);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(vertex4[0], vertex4[1], vertex4[2]);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(vertex7[0], vertex7[1], vertex7[2]);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(vertex8[0], vertex8[1], vertex8[2]);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(vertex2[0], vertex2[1], vertex2[2]);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(vertex3[0], vertex3[1], vertex3[2]);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(vertex8[0], vertex8[1], vertex8[2]);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(vertex5[0], vertex5[1], vertex5[2]);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(vertex1[0], vertex1[1], vertex1[2]);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(vertex4[0], vertex4[1], vertex4[2]);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(vertex7[0], vertex7[1], vertex7[2]);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(vertex6[0], vertex6[1], vertex6[2]);
	glEnd();

	// Dump the image to the screen.
	glutSwapBuffers();

}

// Initialize OpenGL's rendering modes
void initRendering() {
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
	glEnable(GL_LIGHTING);     // Enable lighting calculations
	glEnable(GL_LIGHT0);       // Turn on light #0.
	glDepthFunc(GL_LESS);
	Image *image1 = loadTexture1();
	if (image1 == NULL) {
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}
	Image *image2 = loadTexture2();
	if (image2 == NULL) {
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(3, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image1->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image2->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight, 0,
			GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);
	glEnable(GL_TEXTURE_2D);
	//glShadeModel(GL_FLAT);
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h) {
	// Always use the largest square viewport possible
	if (w > h) {
		glViewport((w - h) / 2, 0, h, h);
	} else {
		glViewport(0, (h - w) / 2, w, w);
	}

	// Set up a perspective view, with square aspect ratio
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 50 degree fov, uniform aspect ratio, near = 1, far = 100
	gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput() {
	// load the OBJ file here
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main(int argc, char** argv) {
	loadInput();

	glutInit(&argc, argv);

	// We're going to animate it, so double buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Initial parameters for window position and size
	glutInitWindowPosition(60, 60);
	glutInitWindowSize(360, 360);
	glutCreateWindow("CMPE 360 Homework 03");

	// Initialize OpenGL parameters.
	initRendering();

	// Set up callback functions for key presses
	glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
	glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

	// Set up the callback function for resizing windows
	glutReshapeFunc(reshapeFunc);

	// Call this whenever window needs redrawing
	glutDisplayFunc(drawScene);

	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop();

	return 0;	// This line is never reached.
}
