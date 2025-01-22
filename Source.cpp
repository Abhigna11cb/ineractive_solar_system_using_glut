#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265358979323846
float cameraX = 0.0f, cameraY = 10.0f, cameraZ = 20.0f;
float lookAtX = 0.0f, lookAtY = 0.0f, lookAtZ = 0.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;
// Array to hold current angles for each planet
float planetAngles[8] = { 0.0f };  // Increased size to 8 planets

// Orbital speeds for each planet
float orbitalSpeeds[8] = { 0.01f, 0.0075f, 0.005f, 0.004f, 0.0025f, 0.002f, 0.0015f, 0.001f };  // Added speed for Neptune

// Flag to control revolution (start/stop)
int isRevolutionActive = 1;  // Initially, planets are revolving

// Flag to switch between views (solar system and black screen)
int isBlackScreen = 0;  // 0 for solar system view, 1 for black screen

// Function to set up lighting
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPosition[] = { 0.0, 0.0, 5.0, 1.0 };
    GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glShadeModel(GL_SMOOTH);
}

// Function to draw the gradient background
void drawGradientBackground() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST); // Disable depth testing to ensure the gradient is behind everything
    glDisable(GL_LIGHTING);  // Disable lighting for the background

    glBegin(GL_QUADS);
    // Top gradient color (dark blue)
    glColor3f(0.0f, 0.0f, 0.2f);
    glVertex2f(0.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);

    // Bottom gradient color (black)
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glEnd();

    glEnable(GL_DEPTH_TEST); // Re-enable depth testing
    glEnable(GL_LIGHTING);   // Re-enable lighting

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// Function to draw stars covering the entire screen
void drawGalaxyStars() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST); // Disable depth testing for the background
    glDisable(GL_LIGHTING);  // Disable lighting for the stars

    glColor3f(1.0f, 1.0f, 1.0f); // Star color: white
    glPointSize(1.2f);           // Adjust point size for smaller stars

    glBegin(GL_POINTS);
    srand(0); // Fixed seed for consistent star pattern
    for (int i = 0; i < 1000; i++) { // Number of stars
        float x = (float)rand() / RAND_MAX; // Random x between 0.0 and 1.0
        float y = (float)rand() / RAND_MAX; // Random y between 0.0 and 1.0
        glVertex2f(x, y);
    }
    glEnd();

    glEnable(GL_DEPTH_TEST); // Re-enable depth testing
    glEnable(GL_LIGHTING);   // Re-enable lighting

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// Function to draw the Sun
void drawSun() {
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(1.0f, 50, 50);
}

// Function to draw orbit paths
void drawOrbit(float semiMajorAxis, float semiMinorAxis, float tiltAngle) {
    glPushMatrix();
    glRotatef(tiltAngle, 1.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float angle = i * PI / 180.0f;
        float x = semiMajorAxis * cos(angle);
        float z = semiMinorAxis * sin(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
    glPopMatrix();
}
// Function to display text in 3D space
void drawText3D(float x, float y, float z, const char* text) {
    glRasterPos3f(x, y, z);  // Set the position for the text

    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);  // Render each character
        text++;
    }
}

// Function to draw a planet
void drawPlanet(float semiMajorAxis, float semiMinorAxis, float size, int planetIndex, float red, float green, float blue, float tiltAngle)
{
    float angle = planetAngles[planetIndex];

    float x = semiMajorAxis * cos(angle);
    float z = semiMinorAxis * sin(angle);

    glPushMatrix();
    glRotatef(tiltAngle, 1.0f, 0.0f, 0.0f);
    glColor3f(red, green, blue);
    glTranslatef(x, 0.0f, z);
    glutSolidSphere(size, 20, 20);
    glPopMatrix();
}
// Function to draw a planet with customizable size, position, and color
void drawPlanetWithParams(float size, float xPosition, float yPosition, float zPosition, float red, float green, float blue) {
    // Set the color of the planet
    glColor3f(red, green, blue);

    // Move to the specified position
    glPushMatrix();
    glTranslatef(xPosition, yPosition, zPosition);

    // Draw the planet as a sphere
    glutSolidSphere(size, 20, 20);  // Drawing the planet as a solid sphere with the given size

    glPopMatrix();  // Restore the previous transformation matrix
}

// Function to draw the Sun and planets in a static vertical order
// Example usage of drawPlanetWithParams
// Function to draw the Sun and planets in a static vertical order on the left side of the screen
void drawSolarSystemVertically() {
    // Draw Sun on the left side
    drawPlanetWithParams(1.0f, -12.0f, 5.0f, 0.0f, 1.0f, 1.0f, 0.0f); // Sun in yellow at position (-5, 0, 0)
    drawText3D(-10.0f, 5.0f, 0.0f, "Sun: The Sun is a star at the center of the solar system. It is about 109 times the size of Earth.");

    // Draw planets below the Sun with different colors and positions
    drawPlanetWithParams(0.1f, -12.5f, 3.0f, 0.0f, 0.5f, 0.5f, 0.5f); // Mercury in gray
    drawText3D(-10.6f, 3.0f, 0.0f, "Mercury: The smallest planet. Mercury is the closest planet to the Sun. It has no atmosphere.");

    drawPlanetWithParams(0.2f, -13.0f, 1.5f, 0.0f, 0.8f, 0.6f, 0.2f); // Venus in yellow-orange
    drawText3D(-11.0f, 1.5f, 0.0f, "Venus: Known as Earth's twin. Venus has a dense atmosphere mainly composed of carbon dioxide.");
    drawPlanetWithParams(0.3f, -13.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); // Earth in blue
    drawText3D(-11.2f, 0.0f, 0.0f, "Earth: Our home planet. Earth has one moon, which helps stabilize its axial tilt, leading to consistent seasons and climates.");

    drawPlanetWithParams(0.25f, -14.0f, -1.50f, 0.0f, 1.0f, 0.0f, 0.0f); // Mars in red
    drawText3D(-11.4f, -1.5f, 0.0f, "Mars: Known as the Red Planet. Mars is home to the largest volcano in the solar system, and the deepest canyon, Valles Marineris.");

    drawPlanetWithParams(0.6f, -14.6f, -3.0f, 0.0f, 0.6f, 0.3f, 0.0f); // Jupiter in brown
    drawText3D(-11.7f, -3.0f, 0.0f, "Jupiter: The largest planet. Jupiter has a Great Red Spot, a massive storm that has been raging for at least 400 years.");

    drawPlanetWithParams(0.5f, -15.0f, -5.0f, 0.0f, 0.9f, 0.8f, 0.3f); // Saturn in yellow-brown
    drawText3D(-12.2f, -5.0f, 0.0f, "Saturn: Famous for its rings. Saturn has 83 known moons, with Titan being the largest and even larger than Mercury.");

    drawPlanetWithParams(0.4f, -15.50f, -6.7f, 0.0f, 0.4f, 0.8f, 1.0f); // Uranus in cyan
    drawText3D(-12.3f, -6.7f, 0.0f, "Uranus: Rotates on its side. Uranus has a faint ring system and is the only planet that rotates clockwise on its axis.");

    drawPlanetWithParams(0.35f, -16.0f, -8.4f, 0.0f, 0.0f, 0.0f, 1.0f); // Neptune in dark blue
    drawText3D(-12.6f, -8.4f, 0.0f, "Neptune: The farthest planet from the Sun. Neptune has the strongest winds in the solar system, reaching speeds of over 2,000 km/h.");
}
float planetColors[8][3] = {
    {0.5f, 0.5f, 0.5f}, // Mercury
    {0.8f, 0.6f, 0.2f}, // Venus
    {0.0f, 0.0f, 1.0f}, // Earth
    {1.0f, 0.0f, 0.0f}, // Mars
    {0.6f, 0.3f, 0.0f}, // Jupiter
    {0.9f, 0.8f, 0.3f}, // Saturn
    {0.4f, 0.8f, 1.0f}, // Uranus
    {0.0f, 0.0f, 1.0f}  // Neptune
};

// Display function
void display() {
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, upX, upY, upZ);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (isBlackScreen) {
        // Draw a black screen if in black screen mode
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        drawGradientBackground(); // Draw the gradient background
        drawGalaxyStars();
        drawSolarSystemVertically();
    }
    else {
        // Solar system display
        drawGradientBackground(); // Draw the gradient background
        drawGalaxyStars(); // Draw stars over the gradient
        drawSun();

        drawOrbit(2.5f, 1.5f, 10.0f);
        drawOrbit(4.0f, 2.5f, 7.0f);
        drawOrbit(5.5f, 3.5f, 5.0f);
        drawOrbit(7.0f, 4.0f, 3.0f);
        drawOrbit(9.0f, 5.5f, 2.0f);
        drawOrbit(11.0f, 6.5f, 1.0f);
        drawOrbit(13.0f, 7.5f, 0.0f);
        drawOrbit(14.0f, 8.0f, -1.0f);  // Orbit for Neptune

        drawPlanet(2.5f, 1.5f, 0.1f, 0, planetColors[0][0], planetColors[0][1], planetColors[0][2], 10.0f);
        drawPlanet(4.0f, 2.5f, 0.2f, 1, planetColors[1][0], planetColors[1][1], planetColors[1][2], 7.0f);
        drawPlanet(5.5f, 3.5f, 0.3f, 2, planetColors[2][0], planetColors[2][1], planetColors[2][2], 5.0f);
        drawPlanet(7.0f, 4.0f, 0.25f, 3, planetColors[3][0], planetColors[3][1], planetColors[3][2], 3.0f);
        drawPlanet(9.0f, 5.5f, 0.6f, 4, planetColors[4][0], planetColors[4][1], planetColors[4][2], 2.0f);
        drawPlanet(11.0f, 6.5f, 0.5f, 5, planetColors[5][0], planetColors[5][1], planetColors[5][2], 1.0f);
        drawPlanet(13.0f, 7.5f, 0.4f, 6, planetColors[6][0], planetColors[6][1], planetColors[6][2], 0.0f);
        drawPlanet(14.0f, 8.0f, 0.35f, 7, planetColors[7][0], planetColors[7][1], planetColors[7][2], -1.0f);
    }

    glutSwapBuffers();
}

// Idle function to update planet angles if revolution is active
void updateAngles() {
    if (isRevolutionActive && !isBlackScreen) { // Update only if not in black screen mode
        for (int i = 0; i < 8; i++) {  // Update loop for 8 planets
            planetAngles[i] += orbitalSpeeds[i];
            if (planetAngles[i] > 2 * PI) {
                planetAngles[i] -= 2 * PI;
            }
        }
    }
    glutPostRedisplay();
}

// Mouse handler to start/stop revolution and switch to black screen
void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        isBlackScreen = !isBlackScreen;  // Toggle between solar system and black screen
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isRevolutionActive = !isRevolutionActive;  // Toggle revolution state
    }
}

// Keyboard handler
void handleKeyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }

    switch (key) {
    case 'f':  // Move camera forward
        cameraZ -= 1.0f;
        break;
    case 'b':  // Move camera backward
        cameraZ += 1.0f;
        break;
    case 'l':  // Move camera left
        cameraX -= 1.0f;
        break;
    case 'r':  // Move camera right
        cameraX += 1.0f;
        break;
    case 'u':  // Move camera up
        cameraY += 1.0f;
        break;
    case 'd':  // Move camera down
        cameraY -= 1.0f;
        break;
    case 's':  // Reset camera to default
        cameraX = 0.0f; cameraY = 10.0f; cameraZ = 20.0f;
        break;
    case 27:   // ESC to exit
        exit(0);
    }
    glutPostRedisplay();  // Update display after changes
}


void setPlanetColor(int planetIndex) {
    // Change the color of the selected planet
    srand(time(NULL)); // Use a random color
    planetColors[planetIndex][0] = (float)rand() / RAND_MAX;
    planetColors[planetIndex][1] = (float)rand() / RAND_MAX;
    planetColors[planetIndex][2] = (float)rand() / RAND_MAX;

    glutPostRedisplay(); // Update display after color change
}
void menuHandler(int option) {
    if (option >= 0 && option < 8) {
        setPlanetColor(option);
    }
}
void createMenu() {
    int planetMenu = glutCreateMenu(menuHandler);
    glutAddMenuEntry("Mercury", 0);
    glutAddMenuEntry("Venus", 1);
    glutAddMenuEntry("Earth", 2);
    glutAddMenuEntry("Mars", 3);
    glutAddMenuEntry("Jupiter", 4);
    glutAddMenuEntry("Saturn", 5);
    glutAddMenuEntry("Uranus", 6);
    glutAddMenuEntry("Neptune", 7);

    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}



// Initialize OpenGL settings
void init() {
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1920.0 / 1080.0, 1.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, upX, upY, upZ);
    setupLighting();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("3D Solar System");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(updateAngles);
    glutKeyboardFunc(handleKeyboard);
    glutMouseFunc(handleMouse); // Register mouse function for click events
    createMenu();
    glutMainLoop();
    return 0;
}