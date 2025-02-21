#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constants
#define NUM_STARS 200
#define NUM_COMETS 10

// Planet information structure
typedef struct {
    const char *name;
    const char *info;
} PlanetInfo;

// Planet data
PlanetInfo planetInfos[] = {
    {"Mercury", "Mercury is the closest planet to the Sun."},
    {"Venus", "Venus is the hottest planet in our solar system."},
    {"Earth", "Earth is the only planet known to support life."},
    {"Mars", "Mars is known as the Red Planet."},
    {"Jupiter", "Jupiter is the largest planet in our solar system."},
    {"Saturn", "Saturn is known for its spectacular ring system."},
    {"Uranus", "Uranus has a unique tilt that makes it roll on its side."},
    {"Neptune", "Neptune is known for its strong winds and storms."}
};

int selectedPlanet = -1; // Index of the clicked planet

// Revolution speeds for planets (minimized)
static float mercuryRevolutionSpeed = 0.001f;
static float venusRevolutionSpeed = 0.0008f;
static float earthRevolutionSpeed = 0.0006f;
static float marsRevolutionSpeed = 0.0005f;
static float jupiterRevolutionSpeed = 0.0003f;
static float saturnRevolutionSpeed = 0.0002f;
static float uranusRevolutionSpeed = 0.0001f;
static float neptuneRevolutionSpeed = 0.00005f;

// Rotation speeds for planets
static float mercuryRotationSpeed = 0.1f;
static float venusRotationSpeed = 0.08f;
static float earthRotationSpeed = 0.06f;
static float marsRotationSpeed = 0.05f;
static float jupiterRotationSpeed = 0.03f;
static float saturnRotationSpeed = 0.02f;
static float uranusRotationSpeed = 0.01f;
static float neptuneRotationSpeed = 0.005f;

// Rotation angles for planets
static float mercuryRotationAngle = 0.0f;
static float venusRotationAngle = 0.0f;
static float earthRotationAngle = 0.0f;
static float marsRotationAngle = 0.0f;
static float jupiterRotationAngle = 0.0f;
static float saturnRotationAngle = 0.0f;
static float uranusRotationAngle = 0.0f;
static float neptuneRotationAngle = 0.0f;

// Revolution angles for planets
static float mercuryAngle = 0.0f;
static float venusAngle = 0.0f;
static float earthAngle = 0.0f;
static float marsAngle = 0.0f;
static float jupiterAngle = 0.0f;
static float saturnAngle = 0.0f;
static float uranusAngle = 0.0f;
static float neptuneAngle = 0.0f;

// Arrays for star and comet positions
float starPositions[NUM_STARS][3];
float cometPositions[NUM_COMETS][6]; // [x, y, z, length, angle, speed]

// Planet orbits radii and angles
float planetOrbitRadii[] = {1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5};
float planetAngles[] = {mercuryAngle, venusAngle, earthAngle, marsAngle, jupiterAngle, saturnAngle, uranusAngle, neptuneAngle};

// Colors
GLfloat diffuseMaterial[4] = {0.5, 0.5, 0.5, 1.0};

// Global variable to control the simulation state
bool isSimulationRunning = true;

// Function to initialize star and comet positions
void initBackground() {
    // Generate random positions for stars
    for (int i = 0; i < NUM_STARS; i++) {
        starPositions[i][0] = ((float)rand() / RAND_MAX) * 20.0 - 10.0;
        starPositions[i][1] = ((float)rand() / RAND_MAX) * 20.0 - 10.0;
        starPositions[i][2] = ((float)rand() / RAND_MAX) * -20.0; // Z is negative to place stars in the background
    }

    // Generate random positions and attributes for comets
    for (int i = 0; i < NUM_COMETS; i++) {
        cometPositions[i][0] = ((float)rand() / RAND_MAX) * 20.0 - 10.0;
        cometPositions[i][1] = ((float)rand() / RAND_MAX) * 20.0 - 10.0;
        cometPositions[i][2] = ((float)rand() / RAND_MAX) * -20.0;
        cometPositions[i][3] = ((float)rand() / RAND_MAX) * 0.5 + 0.1; // Length
        cometPositions[i][4] = (float)rand() / RAND_MAX * 2 * M_PI; // Angle
        cometPositions[i][5] = ((float)rand() / RAND_MAX) * 0.01; // Speed
    }
}

// Function to draw stars in the background
void drawStars() {
    glColor3f(1.0, 1.0, 1.0); // White color for stars
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_STARS; i++) {
        glVertex3f(starPositions[i][0], starPositions[i][1], starPositions[i][2]);
    }
    glEnd();
}

// Function to draw comets
void drawComets() {
    glColor3f(1.0, 1.0, 1.0); // White color for comets
    for (int i = 0; i < NUM_COMETS; i++) {
        float x = cometPositions[i][0];
        float y = cometPositions[i][1];
        float z = cometPositions[i][2];
        float length = cometPositions[i][3];
        float angle = cometPositions[i][4];
        float speed = cometPositions[i][5];
        
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x + length * cos(angle), y + length * sin(angle), z);
        glEnd();
        
        // Optionally, you can animate comets by updating their positions over time.
        cometPositions[i][4] += speed; // Update the angle for the next frame
    }
}

// Function to draw orbit axes
void drawOrbit(float radius) {
    glColor3f(0.0, 0.0, 1.0); // White color for orbit axes
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0;
        glVertex3f(cos(angle) * radius, sin(angle) * radius, 0);
    }
    glEnd();
}

// Function to draw a torus (rings)
void drawRing(float innerRadius, float outerRadius, float r, float g, float b) {
    glColor3f(r, g, b);
    glutWireTorus(innerRadius, outerRadius, 20, 20); // Wireframe torus
}

void myinit(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {0.0, 0.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    // Initialize background elements
    initBackground();
}

void renderBitmapString(float x, float y, float z, void *font, const char *string) {
    const char *c;
    glColor3f(1.0, 1.0, 1.0); // White color for text
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawPlanet(float angle, float orbitRadius, float planetSize, float r, float g, float b, const char *name, bool hasRings) {
    float x = orbitRadius * cos(angle);
    float y = orbitRadius * sin(angle);
    
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    
    // Rotate the planet on its axis
    if (planetSize > 0.0f) { // Apply rotation only if the planet has a size
        glRotatef(mercuryRotationAngle, 0.0, 1.0, 0.0);
    }
    
    // Draw the planet
    glColor3f(r, g, b);
    glutSolidSphere(planetSize, 20, 8);
    renderBitmapString(-0.1, 0.3, 0.0, GLUT_BITMAP_HELVETICA_12, name);
    
    // Draw rings if applicable
    if (hasRings) {
        glPushMatrix();
        glRotatef(90.0f, 1.0, 0.0, 0.0); // Rotate rings to be horizontal
        drawRing(0.05f, planetSize + 0.1f, 0.8f, 0.8f, 0.8f); // Gray color for rings
        glPopMatrix();
    }
    
    glPopMatrix();
}
void displayText()
{
	glColor3f(1.0, 1.0, 1.0);
	printf("SIMULATION OF SOLAR SYSTEM");
	glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw stars in the background
    drawStars();

    // Draw comets in the background
    drawComets();

	displayText();

    // Draw the Sun in the center
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0); // Yellow color for the Sun
    glutSolidSphere(0.5, 40, 16);
    glPopMatrix();

    // Draw orbits
    drawOrbit(1.0); // Orbit for Mercury
    drawOrbit(1.5); // Orbit for Venus
    drawOrbit(2.0); // Orbit for Earth
    drawOrbit(2.5); // Orbit for Mars
    drawOrbit(3.0); // Orbit for Jupiter
    drawOrbit(3.5); // Orbit for Saturn
    drawOrbit(4.0); // Orbit for Uranus
    drawOrbit(4.5); // Orbit for Neptune

    // Draw the planets
    drawPlanet(mercuryAngle, 1.0, 0.1, 1.0, 0.0, 0.0, "Mercury", false);
    drawPlanet(venusAngle, 1.5, 0.15, 0.7, 0.5, 1.0, "Venus", false);
    drawPlanet(earthAngle, 2.0, 0.2, 0.1, 0.6, 0.2, "Earth", false);
    drawPlanet(marsAngle, 2.5, 0.2, 1.0, 0.2, 0.0, "Mars", false);
    drawPlanet(jupiterAngle, 3.0, 0.3, 0.9, 0.7, 0.3, "Jupiter", true);
    drawPlanet(saturnAngle, 3.5, 0.3, 0.5, 0.5, 0.5, "Saturn", true);
    drawPlanet(uranusAngle, 4.0, 0.3, 0.4, 0.8, 0.6, "Uranus", false);
    drawPlanet(neptuneAngle, 4.5, 0.4, 0.0, 0.0, 1.0, "Neptune", false);

    

    glutSwapBuffers();

}


void idle(void) {
    if (isSimulationRunning) {
        // Update rotation and revolution angles for planets
        mercuryAngle += mercuryRevolutionSpeed;
        venusAngle += venusRevolutionSpeed;
        earthAngle += earthRevolutionSpeed;
        marsAngle += marsRevolutionSpeed;
        jupiterAngle += jupiterRevolutionSpeed;
        saturnAngle += saturnRevolutionSpeed;
        uranusAngle += uranusRevolutionSpeed;
        neptuneAngle += neptuneRevolutionSpeed;

        mercuryRotationAngle += mercuryRotationSpeed;
        venusRotationAngle += venusRotationSpeed;
        earthRotationAngle += earthRotationSpeed;
        marsRotationAngle += marsRotationSpeed;
        jupiterRotationAngle += jupiterRotationSpeed;
        saturnRotationAngle += saturnRotationSpeed;
        uranusRotationAngle += uranusRotationSpeed;
        neptuneRotationAngle += neptuneRotationSpeed;

        planetAngles[0] = mercuryAngle;
        planetAngles[1] = venusAngle;
        planetAngles[2] = earthAngle;
        planetAngles[3] = marsAngle;
        planetAngles[4] = jupiterAngle;
        planetAngles[5] = saturnAngle;
        planetAngles[6] = uranusAngle;
        planetAngles[7] = neptuneAngle;
    }

    glutPostRedisplay();
	
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keys(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // Escape key
            exit(0);
            break;
        case 's': // Stop simulation
            isSimulationRunning = false;
            break;
        case 'c': // Continue simulation
            isSimulationRunning = true;
            break;
        default:
            break;
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    myinit();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keys);
    glutIdleFunc(idle); // Set the idle function to ensure continuous animation
    glutMainLoop();
    return 0;
}
