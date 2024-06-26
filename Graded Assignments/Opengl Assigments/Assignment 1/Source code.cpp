#include <cstdlib>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define NUMBERPRIMITIVES 4
#define POLYGON 4

// Globals.
static int width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?

// Point class.

class Point
{
public:
	Point(float xVal, float yVal, float sizeVal)
	{
		x = xVal; y = yVal; size = sizeVal;
	}
	Point() {};
	void drawPoint(void); // Function to draw a point.
	float getX() const { return x; }
    float getY() const { return y; }
private:
	float x, y; // x and y co-ordinates of point.
	float size; // Size of point.
};

//float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0.0);
	glEnd();
}

// Vector of points.
std::vector<Point> points;

// Iterator to traverse a Point array.
std::vector<Point>::iterator pointsIterator;

// Function to draw all points in the points array.
void drawPoints(void)
{
	// Loop through the points array drawing each point.
	for (auto point : points) { point.drawPoint(); }
}

// Line class.
class Line
{
public:
	Line(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	}
	Line() {};
	void drawLine();
private:
    float x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};


// Function to draw a line.
void Line::drawLine()
{
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

// Vector of lines.
std::vector<Line> lines;

// Function to draw all lines in the lines array.
void drawLines(void)
{
	// Loop through the lines array drawing each line.
	for (auto line : lines) { line.drawLine(); }
}

// Rectangle class.
class Rect
{
public:
	Rect(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	}
	Rect() {};
	void drawRectangle();
    float getX1() const { return x1; } // Getter for x1.
    float getY1() const { return y1; } // Getter for y1.
    float getX2() const { return x2; } // Getter for x2.
    float getY2() const { return y2; } // Getter for y2.
private:
    float x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
};

// Function to draw a rectangle.
void Rect::drawRectangle()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
std::vector<Rect> rectangles;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
	// Loop through the rectangles array drawing each rectangle.
	for (auto rectangle : rectangles) { rectangle.drawRectangle(); }
}

// Vector of points.
std::vector<Point> vertices;

class Pg: private Point
{
public:
    Pg(std::vector<Point> vertice)
    {
      vertices =  vertice;
    };
    Pg() {};
    void drawPolygon();
    std::vector<Point> getvertices() { return vertices; }
    void clip(Rect clipper, std::vector<Point> polygon);
private:
    std::vector<Point> vertices;
};
std::vector<Pg> PGs;

void Pg::drawPolygon()
{
    glBegin(GL_POLYGON); // Draw A Poly
    glColor3f(0,0, 0);
    for (auto vertex : vertices)
    {
        glVertex3f(vertex.getX(), vertex.getY(), 0.0);
    }
    glEnd();
}



void drawPolygons(void)
{
    for (auto pg : PGs) { pg.drawPolygon(); }
}

float x_intersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    float num = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    return num / den;
}

float y_intersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    float num = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    return num / den;
}

void Pg::clip(Rect clipper, std::vector<Point> polygon)
{
        std::vector<Point> tmp;
        for (int i = 0; i < vertices.size(); i++) {
            int j = i + 1;
            if (j == vertices.size()) {
                j = 0;
            }

            int x = std::min(clipper.getX1(), clipper.getX2());
            if (vertices[i].getX() >= x && vertices[j].getX() >= x) {
                tmp.push_back(vertices[j]);
            } else if (vertices[i].getX() < x && vertices[j].getX() < x) {
                continue;
            } else if (vertices[i].getX() >= x && vertices[j].getX() < x) {
                float y = (float(x - vertices[i].getX()) / float(vertices[j].getX() - vertices[i].getX())) *
                              (vertices[j].getY() - vertices[i].getY()) +
                          vertices[i].getY();
                tmp.push_back(Point(x, y,pointSize));
            } else if (vertices[i].getX() < x && vertices[j].getX() >= x) {
                float y = (float(x - vertices[i].getX()) / float(vertices[j].getX() - vertices[i].getX())) *
                              (vertices[j].getY() - vertices[i].getY()) +
                          vertices[i].getY();
                tmp.push_back(Point(x, y,pointSize));
                tmp.push_back(vertices[j]);
            }
        }
        vertices = tmp;
        tmp = {};

        int y = std::max(clipper.getY1(), clipper.getY2());
        for (int i = 0; i < vertices.size(); i++) {
            int j = i + 1;
            if (j == vertices.size()) {
                j = 0;
            }

            if (vertices[i].getY() <= y && vertices[j].getY() <= y) {
                tmp.push_back(vertices[j]);
            } else if (vertices[i].getY() > y && vertices[j].getY() > y) {
                continue;
            } else if (vertices[i].getY() <= y && vertices[j].getY() > y) {
                float x = (float(y - vertices[i].getY()) / float(vertices[j].getY() - vertices[i].getY())) *
                              (vertices[j].getX() - vertices[i].getX()) +
                          vertices[i].getX();
                tmp.push_back(Point(x, y,pointSize));
            } else if (vertices[i].getY() > y && vertices[j].getY() <= y) {
                float x = (float(y - vertices[i].getY()) / float(vertices[j].getY() - vertices[i].getY())) *
                              (vertices[j].getX() - vertices[i].getX()) +
                          vertices[i].getX();

                tmp.push_back(Point(x, y,pointSize));
                tmp.push_back(vertices[j]);
            }
        }
        vertices = tmp;
        tmp = {};

        // Repeat similar logic for the remaining two edges (x2 and y1)

        int x_max = std::max(clipper.getX1(), clipper.getX2());
        for (int i = 0; i < vertices.size(); i++) {
            int j = i + 1;
            if (j == vertices.size()) {
                j = 0;
            }

            if (vertices[i].getX() <= x_max && vertices[j].getX() <= x_max) {
                tmp.push_back(vertices[j]);
            } else if (vertices[i].getX() > x_max && vertices[j].getX() > x_max) {
                continue;
            } else if (vertices[i].getX() <= x_max && vertices[j].getX() > x_max) {
                float y = (float(x_max - vertices[i].getX()) / float(vertices[j].getX() - vertices[i].getX())) *
                              (vertices[j].getY() - vertices[i].getY()) +
                          vertices[i].getY();
                ;
                tmp.push_back(Point(x_max, y,pointSize));
            } else if (vertices[i].getX() > x_max && vertices[j].getX() <= x_max) {
                float y = (float(x_max - vertices[i].getX()) / float(vertices[j].getX() - vertices[i].getX())) *
                              (vertices[j].getY() - vertices[i].getY()) +
                          vertices[i].getY();
                tmp.push_back(Point(x_max, y,pointSize));
                tmp.push_back(vertices[j]);
            }
        }
        vertices = tmp;
        tmp = {};

        int y_min = std::min(clipper.getY1(), clipper.getY2());
        for (int i = 0; i < vertices.size(); i++) {
            int j = i + 1;
            if (j == vertices.size()) {
                j = 0;
            }

            if (vertices[i].getY() >= y_min && vertices[j].getY() >= y_min) {
                tmp.push_back(vertices[j]);
            } else if (vertices[i].getY() < y_min && vertices[j].getY() < y_min) {
                continue;
            } else if (vertices[i].getY() >= y_min && vertices[j].getY() < y_min) {
                float x = (float(y_min - vertices[i].getY()) / float(vertices[j].getY() - vertices[i].getY())) *
                              (vertices[j].getX() - vertices[i].getX()) +
                          vertices[i].getX();
                tmp.push_back(Point(x, y_min,pointSize));
            } else if (vertices[i].getY() < y_min && vertices[j].getY() >= y_min) {
                float x = (float(y_min - vertices[i].getY()) / float(vertices[j].getY() - vertices[i].getY())) *
                              (vertices[j].getX() - vertices[i].getX()) +
                          vertices[i].getX();
                tmp.push_back(Point(x, y_min,pointSize));
                tmp.push_back(vertices[j]);
            }
        }
        vertices = tmp;
        PGs.push_back(vertices);
        vertices.clear();
        PGs.erase(PGs.begin());
}

void clips(void) {
    for (auto rect : rectangles) {
         for(int i = 0; i < PGs.size(); i++ ){
            PGs[i].clip(rect,PGs[i].getvertices());
        }
    }
}

void drawPointSelectionBox(void)
{
	if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.9*height, 0.1*width, height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.9*height, 0.1*width, height);

	// Draw point icon.
	glPointSize(pointSize);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(0.05*width, 0.95*height, 0.0);
	glEnd();
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{
	if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

	// Draw line icon.
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.025*width, 0.825*height, 0.0);
	glVertex3f(0.075*width, 0.875*height, 0.0);
	glEnd();
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{
	if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

	// Draw rectangle icon.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
	glEnd();
}
void drawPolgonSelectionBox(void)
{
    // Highlight if polygon drawing is selected, dim otherwise
  if (primitive == POLYGON) glColor3f(1.0, 1.0, 1.0);
  else glColor3f(0.8, 0.8, 0.8);
  // Fill the selection box area
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

  // Draw black boundary line
  glColor3f(0.0, 0.0, 0.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

  // Draw polygon icon (triangle)
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_POLYGON);
  glVertex3f(0.04*width, 0.62*height, 0.0);
  glVertex3f(0.08*width, 0.68*height, 0.0);
  glVertex3f(0.06*width, 0.63*height, 0.0);
  glVertex3f(0.04*width, 0.67*height, 0.0);
  glEnd();
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
	glColor3f(0.6, 0.6, 0.6);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glVertex3f(tempX, tempY, 0.0);
	glEnd();
}
void drawTempPoints(void)
{
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(pointSize);
    glBegin(GL_POINTS);

    for (auto vertex : vertices)
    {
        glVertex3f(vertex.getX(), vertex.getY(), 0.0);
    }

    glEnd();
}
// Function to draw a grid.
void drawGrid(void)
{
	int i;

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x5555);
	glColor3f(0.75, 0.75, 0.75);

	glBegin(GL_LINES);
	for (i = 2; i <= 9; i++)
	{
		glVertex3f(i*0.1*width, 0.0, 0.0);
		glVertex3f(i*0.1*width, height, 0.0);
	}
	for (i = 1; i <= 9; i++)
	{
		glVertex3f(0.1*width, i*0.1*height, 0.0);
		glVertex3f(width, i*0.1*height, 0.0);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	drawPointSelectionBox();
	drawLineSelectionBox();
	drawRectangleSelectionBox();
	drawPolgonSelectionBox();
	drawInactiveArea();

	drawPoints();
	drawLines();
	drawRectangles();
	drawPolygons();
	if (((primitive == LINE) || (primitive == RECTANGLE) ) &&
		(pointCount == 1)) drawTempPoint();
    if ((primitive == POLYGON) && (pointCount >= 1)) drawTempPoints();

	if (isGrid) drawGrid();

	glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
	if (y < (1 - NUMBERPRIMITIVES*0.1)*height) primitive = INACTIVE;
	else if (y < (1 - 3 * 0.1)*height) primitive = POLYGON;
	else if (y < (1 - 2 * 0.1)*height) primitive = RECTANGLE;
	else if (y < (1 - 1 * 0.1)*height) primitive = LINE;

	else primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		y = height - y; // Correct from mouse to OpenGL co-ordinates.

		if (x < 0 || x > width || y < 0 || y > height); // Click outside canvas - do nothing.

		// Click in left selection area.
		else if (x < 0.1 * width) {
            if (primitive == POLYGON) {
                Pg pg(vertices);
                PGs.push_back(pg);
                vertices.clear(); // Clear vertices after creating polygon
            }
            pickPrimitive(y);
            pointCount = 0;
        }

		// Click in canvas.
		else
		{
			if (primitive == POINT) points.push_back(Point(x, y, pointSize));
			else if (primitive == LINE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					lines.push_back(Line(tempX, tempY, x, y));
					pointCount = 0;
				}
			}
			else if (primitive == RECTANGLE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					rectangles.push_back(Rect(tempX, tempY, x, y));
					pointCount = 0;
				}
			}
			else if (primitive == POLYGON)
			{
			    if (pointCount == 0)
                {
                    tempX = x; tempY = y;
                    vertices.push_back(Point(x, y, pointSize));
					pointCount++;
                }
                else
                {
			    vertices.push_back(Point(x, y, pointSize));

				pointCount++;
                }
			}
		}
	}
	glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set viewing box dimensions equal to window dimensions.
	glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

	// Pass the size of the OpenGL window to globals.
	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
    case 13:
        if (primitive == POLYGON) {
                Pg pg(vertices);
                PGs.push_back(pg);
                vertices.clear(); // Clear vertices after creating polygon
            }
        primitive= INACTIVE;
        pointCount = 0;
        glutPostRedisplay();
        break;
	default:
		break;
	}
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
	points.clear();
	lines.clear();
	rectangles.clear();
	vertices.clear();
	PGs.clear();
	primitive = INACTIVE;
	pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
	if (id == 1)
	{
		clearAll();
		glutPostRedisplay();
	}
	if (id == 2) exit(0);
	if (id == 5)
    {
        clips();
        glutPostRedisplay();
        }
}

// The sub-menu callback function.
void grid_menu(int id)
{
	if (id == 3) isGrid = 1;
	if (id == 4) isGrid = 0;
	glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
	int sub_menu;
	sub_menu = glutCreateMenu(grid_menu);
	glutAddMenuEntry("On", 3);
	glutAddMenuEntry("Off", 4);

	glutCreateMenu(rightMenu);
	glutAddSubMenu("Grid", sub_menu);
	glutAddMenuEntry("Clip", 5);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Quit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	makeMenu(); // Create menu.
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Left click on a box on the left to select a primitive." << std::endl
		<< "Then left click on the drawing area: once for point, twice for line or rectangle." << std::endl
		<< "Right click for menu options." << std::endl<<"Press enter after selecting corners for polygon to create polygon or click on INACTIVE AREA for the polygon to create"<<std::endl;
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("canvas.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseControl);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
