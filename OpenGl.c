#include "local.h"

struct toOpenGl{
    int workerNum[6];
    int medPassed[6];
    int medFailed[6];
    int lineSpeed[6];
};

int inworkerNum[6];
int inmedPassed[6];
int inmedFailed[6];
int inlineSpeed[6];

int Number_Of_Production_Lines = 0;

void drawArea(float x, float y)
{
	glColor3f(0.0, 0.0, 0.0); // Set color to black
	glBegin(GL_QUADS);
	glVertex2f(x - 0.15, y - 0.2);
	glVertex2f(x + 0.15, y - 0.2);
	glVertex2f(x + 0.15, y + 0.2);
	glVertex2f(x - 0.15, y + 0.2);
	glEnd();
}

void drawField()
{
	glColor3f(0.4f, 0.3f, 0.3f); // Set color to gray
	glBegin(GL_QUADS);
	glVertex2f(-1, -1);
	glVertex2f(1, -1);
	glVertex2f(1, 1);
	glVertex2f(-1, 1);
	glEnd();
}

void drawText(float x, float y, char *text)
{
	// Set text color (white)
	glColor3f(1.0, 1.0, 1.0);

	// Set text position (behind the player)
	float text_x = x - 0.1; // Adjust this value based on your requirement
	float text_y = y - 0.1; // Adjust this value based on your requirement

	// Set text position
	glRasterPos2f(text_x, text_y);

	// Render text
	int len = strlen(text);
	for (int i = 0; i < len; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

void drawCounter(float x, float y, int count)
{
	// Convert integer count to string
	char text[20];
	sprintf(text, "%d", count);

	// Set text color (red)
	glColor3f(0.5, 0.1, 0.4);

	// Set text position
	float text_x = x - 0.1; // Adjust this value based on your requirement
	float text_y = y - 0.1; // Adjust this value based on your requirement
	glRasterPos2f(text_x, text_y);

	// Render text
	int len = strlen(text);
	for (int i = 0; i < len; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

void drawFloatCounter(float x, float y, float count){
	// Convert integer count to string
	char text[20];
    float newCount = 1.0 / (count/1000000);
	sprintf(text, "%.5f medicine/s", newCount);

	// Set text color (red)
	glColor3f(0.5, 0.1, 0.4);

	// Set text position
	float text_x = x - 0.1; // Adjust this value based on your requirement
	float text_y = y - 0.1; // Adjust this value based on your requirement
	glRasterPos2f(text_x, text_y);

	// Render text
	int len = strlen(text);
	for (int i = 0; i < len; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

void drawBlackText(float x, float y, char *text)
{
	// Set text color (white)
	glColor3f(0.0, 0.0, 0.0);

	// Set text position (behind the player)
	float text_x = x - 0.1; // Adjust this value based on your requirement
	float text_y = y - 0.1; // Adjust this value based on your requirement

	// Set text position
	glRasterPos2f(text_x, text_y);

	// Render text
	int len = strlen(text);
	for (int i = 0; i < len; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

void drawRedText(float x, float y, char *text)
{
	// Set text color (white)
	glColor3f(1.0, 0.0, 0.0);

	// Set text position (behind the player)
	float text_x = x - 0.1; // Adjust this value based on your requirement
	float text_y = y - 0.1; // Adjust this value based on your requirement

	// Set text position
	glRasterPos2f(text_x, text_y);

	// Render text
	int len = strlen(text);
	for (int i = 0; i < len; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

void drawFrame(float x, float y)
{
    // Set color to gray
    glColor3f(0.5f, 0.5f, 0.5f); 
    
    // Calculate half-width and half-height of the square
    float halfWidth = 0.3;
    float halfHeight = 0.35;
    
    // Draw filled square
    glBegin(GL_QUADS);
    glVertex2f(x - halfWidth, y - halfHeight);
    glVertex2f(x + halfWidth, y - halfHeight);
    glVertex2f(x + halfWidth, y + halfHeight);
    glVertex2f(x - halfWidth, y + halfHeight);
    glEnd();
    
    // Set color to black for the frame
    glColor3f(0.0f, 0.0f, 0.0f);
    
    // Draw the frame using lines
    glBegin(GL_LINE_LOOP);
    glVertex2f(x - halfWidth, y - halfHeight);
    glVertex2f(x + halfWidth, y - halfHeight);
    glVertex2f(x + halfWidth, y + halfHeight);
    glVertex2f(x - halfWidth, y + halfHeight);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawField();

    drawBlackText(-0.1, 0.98, "Gaza in ");
    drawRedText(0.0, 0.98, "HEART");
    drawBlackText(0.1, 0.98, ",Pray for Gaza");

    ///////////////////////////////////////////////line1//////////////////////////////////
    if(Number_Of_Production_Lines >= 1){
        drawFrame(-0.65, 0.4);
        drawBlackText(-0.65, 0.9, "Production Line 1");
        drawBlackText(-0.82, 0.75, "- Line Type:");
        drawBlackText(-0.82, 0.55, "# Workers:");
        drawBlackText(-0.82, 0.45, "# Medicines passed the inspection:");
        drawBlackText(-0.82, 0.35, "# Medicines fail the inspection:");
        drawBlackText(-0.82, 0.25, "- Line Speed:");

        drawRedText(-0.65, 0.75, "Liquid product line");
        drawCounter(-0.67, 0.55, inworkerNum[0]);
        drawCounter(-0.39, 0.45, inmedPassed[0]);
        drawCounter(-0.43, 0.35, inmedFailed[0]);
        drawFloatCounter(-0.64, 0.25, (inlineSpeed[0] * (1.0/inworkerNum[0])));
    }


    ///////////////////////////////////////////////line2//////////////////////////////////
    if(Number_Of_Production_Lines >= 3){
        drawFrame(0, 0.4);
        drawBlackText(0.0, 0.9, "Production Line 3");
        drawBlackText(-0.17, 0.75, "- Line Type:");
        drawBlackText(-0.17, 0.55, "# Workers:");
        drawBlackText(-0.17, 0.45, "# Medicines passed the inspection:");
        drawBlackText(-0.17, 0.35, "# Medicines fail the inspection:");
        drawBlackText(-0.17, 0.25, "- Line Speed:");

        drawRedText(0, 0.75, "Liquid product line");
        drawCounter(-0.02, 0.55, inworkerNum[1]);
        drawCounter(0.26, 0.45, inmedPassed[1]);
        drawCounter(0.22, 0.35, inmedFailed[1]);
        drawFloatCounter(0.01, 0.25, (inlineSpeed[1] * (1.0/inworkerNum[1])));
    }

    ///////////////////////////////////////////////line3//////////////////////////////////
    if(Number_Of_Production_Lines >= 5){
        drawFrame(0.65, 0.4);
        drawBlackText(0.65, 0.9, "Production Line 5");
        drawBlackText(0.48, 0.75, "- Line Type:");
        drawBlackText(0.48, 0.55, "# Workers:");
        drawBlackText(0.48, 0.45, "# Medicines passed the inspection:");
        drawBlackText(0.48, 0.35, "# Medicines fail the inspection:");
        drawBlackText(0.48, 0.25, "- Line Speed:");

        drawRedText(0.65, 0.75, "Liquid product line");
        drawCounter(0.63, 0.55, inworkerNum[2]);
        drawCounter(0.91, 0.45, inmedPassed[2]);
        drawCounter(0.87, 0.35, inmedFailed[2]);
        drawFloatCounter(0.66, 0.25, (inlineSpeed[2] * (1.0/inworkerNum[2])));
    }
    ///////////////////////////////////////////////line4//////////////////////////////////
    if(Number_Of_Production_Lines >= 2){
        drawFrame(-0.65, -0.5);
        drawBlackText(-0.65, 0.0, "Production Line 2");
        drawBlackText(-0.82, -0.15, "- Line Type:");
        drawBlackText(-0.82, -0.35, "# Workers:");
        drawBlackText(-0.82, -0.45, "# Medicines passed the inspection:");
        drawBlackText(-0.82, -0.55, "# Medicines fail the inspection:");
        drawBlackText(-0.82, -0.65, "- Line Speed:");

        drawRedText(-0.65, -0.15, "Pills product line");
        drawCounter(-0.67, -0.35, inworkerNum[3]);
        drawCounter(-0.39, -0.45, inmedPassed[3]);
        drawCounter(-0.43, -0.55, inmedFailed[3]);
        drawFloatCounter(-0.64, -0.65, (inlineSpeed[3] * (1.0/inworkerNum[3])));
    }

    ///////////////////////////////////////////////line5//////////////////////////////////
    if(Number_Of_Production_Lines >= 4){
        drawFrame(0, -0.5);
        drawBlackText(0.0, 0.0, "Production Line 4");
        drawBlackText(-0.17, -0.15, "- Line Type:");
        drawBlackText(-0.17, -0.35, "# Workers:");
        drawBlackText(-0.17, -0.45, "# Medicines passed the inspection:");
        drawBlackText(-0.17, -0.55, "# Medicines fail the inspection:");
        drawBlackText(-0.17, -0.65, "- Line Speed:");

        drawRedText(0, -0.15, "Pills product line");
        drawCounter(-0.02, -0.35, inworkerNum[4]);
        drawCounter(0.26, -0.45, inmedPassed[4]);
        drawCounter(0.22, -0.55, inmedFailed[4]);
        drawFloatCounter(0.01, -0.65, (inlineSpeed[4] * (1.0/inworkerNum[4])));
    }

    ///////////////////////////////////////////////line6//////////////////////////////////
    if(Number_Of_Production_Lines >= 6){
        drawFrame(0.65, -0.5);
        drawBlackText(0.65, 0.0, "Production Line 6");
        drawBlackText(0.48, -0.15, "- Line Type:");
        drawBlackText(0.48, -0.35, "# Workers:");
        drawBlackText(0.48, -0.45, "# Medicines passed the inspection:");
        drawBlackText(0.48, -0.55, "# Medicines fail the inspection:");
        drawBlackText(0.48, -0.65, "- Line Speed:");

        drawRedText(0.65, -0.15, "Pills product line");
        drawCounter(0.63, -0.35, inworkerNum[5]);
        drawCounter(0.91, -0.45, inmedPassed[5]);
        drawCounter(0.87, -0.55, inmedFailed[5]);
        drawFloatCounter(0.66, -0.65, (inlineSpeed[5] * (1.0/inworkerNum[5])));
    }
    glFlush();
}

int ShmidToOpenGl;

void updatePosition()
{
    
	struct toOpenGl *Info;
    if ((ShmidToOpenGl = shmget(12345, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
    {
        perror("shmget fail");
        exit(1);
    }
    
    Info = (struct toOpenGl *)shmat(ShmidToOpenGl ,0 ,0);

    if(Number_Of_Production_Lines == 1){
        inworkerNum[0] = Info[0].workerNum[0];
        inmedPassed[0] = Info[0].medPassed[0];
        inmedFailed[0] = Info[0].medFailed[0];
        inlineSpeed[0] = Info[0].lineSpeed[0];
    }
    else if(Number_Of_Production_Lines == 2){
        inworkerNum[0] = Info[0].workerNum[0];
        inmedPassed[0] = Info[0].medPassed[0];
        inmedFailed[0] = Info[0].medFailed[0];
        inlineSpeed[0] = Info[0].lineSpeed[0];

        inworkerNum[3] = Info[0].workerNum[1];
        inmedPassed[3] = Info[0].medPassed[1];
        inmedFailed[3] = Info[0].medFailed[1];
        inlineSpeed[3] = Info[0].lineSpeed[1];
    }
    else if(Number_Of_Production_Lines == 3){
        inworkerNum[0] = Info[0].workerNum[0];
        inmedPassed[0] = Info[0].medPassed[0];
        inmedFailed[0] = Info[0].medFailed[0];
        inlineSpeed[0] = Info[0].lineSpeed[0];

        inworkerNum[1] = Info[0].workerNum[1];
        inmedPassed[1] = Info[0].medPassed[1];
        inmedFailed[1] = Info[0].medFailed[1];
        inlineSpeed[1] = Info[0].lineSpeed[1];

        inworkerNum[3] = Info[0].workerNum[2];
        inmedPassed[3] = Info[0].medPassed[2];
        inmedFailed[3] = Info[0].medFailed[2];
        inlineSpeed[3] = Info[0].lineSpeed[2];
    }
    else if(Number_Of_Production_Lines == 4){
        inworkerNum[0] = Info[0].workerNum[0];
        inmedPassed[0] = Info[0].medPassed[0];
        inmedFailed[0] = Info[0].medFailed[0];
        inlineSpeed[0] = Info[0].lineSpeed[0];

        inworkerNum[1] = Info[0].workerNum[1];
        inmedPassed[1] = Info[0].medPassed[1];
        inmedFailed[1] = Info[0].medFailed[1];
        inlineSpeed[1] = Info[0].lineSpeed[1];

        inworkerNum[3] = Info[0].workerNum[2];
        inmedPassed[3] = Info[0].medPassed[2];
        inmedFailed[3] = Info[0].medFailed[2];
        inlineSpeed[3] = Info[0].lineSpeed[2];

        inworkerNum[4] = Info[0].workerNum[3];
        inmedPassed[4] = Info[0].medPassed[3];
        inmedFailed[4] = Info[0].medFailed[3];
        inlineSpeed[4] = Info[0].lineSpeed[3];
    }
    else if(Number_Of_Production_Lines == 5){
        inworkerNum[0] = Info[0].workerNum[0];
        inmedPassed[0] = Info[0].medPassed[0];
        inmedFailed[0] = Info[0].medFailed[0];
        inlineSpeed[0] = Info[0].lineSpeed[0];

        inworkerNum[1] = Info[0].workerNum[1];
        inmedPassed[1] = Info[0].medPassed[1];
        inmedFailed[1] = Info[0].medFailed[1];
        inlineSpeed[1] = Info[0].lineSpeed[1];

        inworkerNum[2] = Info[0].workerNum[2];
        inmedPassed[2] = Info[0].medPassed[2];
        inmedFailed[2] = Info[0].medFailed[2];
        inlineSpeed[2] = Info[0].lineSpeed[2];

        inworkerNum[3] = Info[0].workerNum[3];
        inmedPassed[3] = Info[0].medPassed[3];
        inmedFailed[3] = Info[0].medFailed[3];
        inlineSpeed[3] = Info[0].lineSpeed[3];

        inworkerNum[4] = Info[0].workerNum[4];
        inmedPassed[4] = Info[0].medPassed[4];
        inmedFailed[4] = Info[0].medFailed[4];
        inlineSpeed[4] = Info[0].lineSpeed[4];

    }
    else if(Number_Of_Production_Lines == 6){
        inworkerNum[0] = Info[0].workerNum[0];
        inmedPassed[0] = Info[0].medPassed[0];
        inmedFailed[0] = Info[0].medFailed[0];
        inlineSpeed[0] = Info[0].lineSpeed[0];

        inworkerNum[1] = Info[0].workerNum[1];
        inmedPassed[1] = Info[0].medPassed[1];
        inmedFailed[1] = Info[0].medFailed[1];
        inlineSpeed[1] = Info[0].lineSpeed[1];

        inworkerNum[2] = Info[0].workerNum[2];
        inmedPassed[2] = Info[0].medPassed[2];
        inmedFailed[2] = Info[0].medFailed[2];
        inlineSpeed[2] = Info[0].lineSpeed[2];

        inworkerNum[3] = Info[0].workerNum[3];
        inmedPassed[3] = Info[0].medPassed[3];
        inmedFailed[3] = Info[0].medFailed[3];
        inlineSpeed[3] = Info[0].lineSpeed[3];

        inworkerNum[4] = Info[0].workerNum[4];
        inmedPassed[4] = Info[0].medPassed[4];
        inmedFailed[4] = Info[0].medFailed[4];
        inlineSpeed[4] = Info[0].lineSpeed[4];

        inworkerNum[5] = Info[0].workerNum[5];
        inmedPassed[5] = Info[0].medPassed[5];
        inmedFailed[5] = Info[0].medFailed[5];
        inlineSpeed[5] = Info[0].lineSpeed[5];

    }
    
    
	
    sleep(0.5);
    glutPostRedisplay();
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	gluOrtho2D(-1, 1, -1, 1);
}
void RemoveShm(int n);
int main(int argc, char **argv)
{
    signal(SIGINT, RemoveShm);
    FILE *file = fopen("settings.txt", "r");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1; // Exit with error
    }
    
    fscanf(file, "Production Lines and number of different medicines: %d\n", &Number_Of_Production_Lines);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1700, 900);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pharmaceutical factory");

	init();
	glutDisplayFunc(display);
	glutIdleFunc(updatePosition);
	glutMainLoop();

	return 0;
}

void RemoveShm(int n){
	int d1 = shmctl(ShmidToOpenGl, IPC_RMID, NULL);
    if (d1 == -1)
    {
        perror("shmctl");
        exit(1);
    }
	exit(0);
}
