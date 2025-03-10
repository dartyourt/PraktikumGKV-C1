#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <cmath>

// Ukuran jendela
const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;

// Fungsi untuk menggambar lingkaran padat dengan pusat di (0,0)
void drawCircle(float radius, int segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// Fungsi untuk menggambar lingkaran kawah oval di bulan
void drawcircle(float radius, int segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = radius * cos(angle)*0.9;
        float y = radius * sin(angle)*0.78;
        glVertex2f(x, y);
    }
    glEnd();
}

void init2D() {
    // Warna latar belakang (langit biru)
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
    // Proyeksi orthographic
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    // -------------------------
    // Gambar bumi(biru)
    // -------------------------
    glPushMatrix();
        glTranslatef(700.0f, 500.0f, 0.0f);
        glColor3f(0.0f, 0.4f, 1.0f); // biru
        drawCircle(40.0f, 36);      // Lingkaran bumi
    glPopMatrix();

    // -------------------------
    // 1. Gambar rumput (hijau)
    // -------------------------
    glPushMatrix();
    	glRotatef(15.0f,0.0f,0.0f,1.0f);
    	glColor3f(0.5f, 0.5f, 0.5f);
    	glBegin(GL_POLYGON);
        	glVertex2f(0.0f,    -600.0f);
        	glVertex2f(900.0f,  -600.0f);
        	glVertex2f(900.0f, 120.0f);
        	glVertex2f(0.0f,   120.0f);
    	glEnd();
    glPopMatrix();
    
    // -------------------------
    // Gambar kawah bulan 1
    // -------------------------
    glPushMatrix();
        glTranslatef(750.0f, 080.0f, 0.0f);
    	glRotatef(18.0f,0.0f,0.0f,1.0f);
        glColor3f(0.3f, 0.3f, 0.3f); // abu gelap
        drawcircle(40.0f, 36);      // Lingkaran kawah
    glPopMatrix();
    
    // -------------------------
    // Gambar kawah bulan 2
    // -------------------------
    glPushMatrix();
        glTranslatef(178.0f, 080.0f, 0.0f);
    	glRotatef(10.0f,0.0f,0.0f,1.0f);
        glColor3f(0.3f, 0.3f, 0.3f); // abu gelap
        drawcircle(40.0f, 36);      // Lingkaran kawah
    glPopMatrix();
    // -------------------------
    // Gambar kawah bulan 3
    // -------------------------
    glPushMatrix();
        glTranslatef(420.0f, 070.0f, 0.0f);
    	glRotatef(12.0f,0.0f,0.0f,1.0f);
        glColor3f(0.3f, 0.3f, 0.3f); // abu gelap
        drawcircle(40.0f, 36);      // Lingkaran kawah
    glPopMatrix();
    // -------------------------
    // Gambar kawah bulan 4
    // -------------------------
    glPushMatrix();
        glTranslatef(721.0f, 240.0f, 0.0f);
    	glRotatef(12.4f,0.0f,0.0f,1.0f);
        glColor3f(0.3f, 0.3f, 0.3f); // abu gelap
        drawcircle(40.0f, 36);      // Lingkaran kawah
    glPopMatrix();
    // -------------------------
    // 3. Gambar mobil
    // -------------------------
    // Posisi dasar mobil di (225,80)
    glPushMatrix();
        glTranslatef(225.0f, 080.0f, 0.0f);
        glRotatef(15.0f,0.0f,0.0f,1.0f);
        glRotatef(0.0f, 0.0f, 0.0f, 0.0f);

        // Body utama mobil (merah)
        glColor3f(0.0f, 0.0f, 0.97f);  // Merah
        glBegin(GL_POLYGON);	
            glVertex2f(50.0f,  20.0f);
            glVertex2f(340.0f, 20.0f);
            glVertex2f(340.0f, 70.0f);
            glVertex2f(50.0f,  70.0f);
        glEnd();
        
        // lampu biru (atap)
        glColor3f(0.2f, 0.2f, 1.0f);
        glBegin(GL_POLYGON);
            glVertex2f(180.0f, 135.0f);
            glVertex2f(210.0f, 135.0f);
            glVertex2f(190.0f, 145.0f);
            glVertex2f(180.0f, 145.0f);
        glEnd();
        
        // lampu merah (atap)
        glColor3f(1.0f, 0.2f, 0.2f);
        glBegin(GL_POLYGON);
            glVertex2f(150.0f, 135.0f);
            glVertex2f(180.0f, 135.0f);
            glVertex2f(180.0f, 145.0f);
            glVertex2f(160.0f, 145.0f);
        glEnd();
        

        // Bagian atas body (atap)
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
            glVertex2f(50.0f,  70.0f);
            glVertex2f(300.0f, 70.0f);
            glVertex2f(270.0f, 135.0f);
            glVertex2f(80.0f, 135.0f);
        glEnd();

        // Lampu depan (kuning)
        glColor3f(1.0f, 0.8f, 0.2f);
        glBegin(GL_POLYGON);
            glVertex2f(330.0f, 40.0f);
            glVertex2f(340.5f, 40.0f);
            glVertex2f(340.0f, 60.0f);
            glVertex2f(330.0f, 60.0f);
        glEnd();
        
        // bumper depan (abu)
        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_POLYGON);
            glVertex2f(300.0f, 20.0f);
            glVertex2f(340.5f, 20.0f);
            glVertex2f(340.0f, 40.0f);
            glVertex2f(300.0f, 40.0f);
        glEnd();
        
        // lampu belakang (abu-abu)
        glColor3f(1.0f, 0.2f, 0.2f);
        glBegin(GL_POLYGON);
            glVertex2f(50.0f, 40.0f);
            glVertex2f(60.0f, 40.0f);
            glVertex2f(60.0f, 60.0f);
            glVertex2f(50.0f, 60.0f);
        glEnd();
        
        // Bumper belakang (abu-abu)
        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_POLYGON);
            glVertex2f(50.0f, 20.0f);
            glVertex2f(90.0f, 20.0f);
            glVertex2f(90.0f, 40.0f);
            glVertex2f(50.0f, 40.0f);
        glEnd();
        
        // Roda depan (lingkaran hitam)
        glPushMatrix();
            glTranslatef(290.0f, 20.0f, 0.0f); 
            glColor3f(0.0f, 0.0f, 0.0f);      // Hitam
            drawCircle(28.0f, 30);           // radius 25
            // Bagian putih di dalam roda
            glColor3f(1.0f, 1.0f, 1.0f);
            drawCircle(18.0f, 30);
        glPopMatrix();

        //  Roda belakang
        glPushMatrix();
            glTranslatef(95.0f, 20.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            drawCircle(28.0f, 30);
            // Bagian putih di dalam roda
            glColor3f(1.0f, 1.0f, 1.0f);
            drawCircle(18.0f, 30);
        glPopMatrix();

        // Jendela (putih keabu-abuan)
        // Jendela depan
        glColor3f(0.6f, 0.8f, 1.0f);
        glBegin(GL_POLYGON);
            glVertex2f(70.0f, 80.0f);
            glVertex2f(180.0f, 80.0f);
            glVertex2f(180.0f, 125.0f);
            glVertex2f(91.0f, 125.0f);
        glEnd();

        // Jendela belakang
        glBegin(GL_POLYGON);
            glVertex2f(190.0f, 80.0f);
            glVertex2f(280.0f, 80.0f);
            glVertex2f(260.0f, 125.0f);
            glVertex2f(190.0f, 125.0f);
        glEnd();


        // Pegangan pintu (hitam)
        glColor3f(0.0f, 0.0f, 0.0f);
        // Pegangan pintu depan
        glBegin(GL_POLYGON);
            glVertex2f(140.0f, 50.0f);
            glVertex2f(155.0f, 50.0f);
            glVertex2f(155.0f, 53.0f);
            glVertex2f(140.0f, 53.0f);
        glEnd();
        // Pegangan pintu belakang
        glBegin(GL_POLYGON);
            glVertex2f(210.0f, 50.0f);
            glVertex2f(225.0f, 50.0f);
            glVertex2f(225.0f, 53.0f);
            glVertex2f(210.0f, 53.0f);
        glEnd();

    glPopMatrix(); // Selesai menggambar mobil

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("gambarmobil");
    init2D();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

