#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Ukuran jendela
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// variabel global
char projectionName[70] = "Normal"; // Default proyeksi
int projectionType = 0; // 0=Perspective, 1=Orthographic

// Fungsi untuk menampilkan teks
void renderText(float x, float y, void *font, const char *string) {
    glColor3f(1.0f, 1.0f, 1.0f); // Warna teks putih
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Variabel Kamera dan Proyeksi
float angle = 0.0f, deltaAngle = 0.0f, ratio;
float x = 0.0f, y = 2.0f, z = 15.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
int deltaMove = 0;
int projectionMode = 0; // 0: Perspective, 1: Orthographic
int orthoMode = 0;      // Sub-mode untuk orthographic

// Fungsi untuk menggambar silinder (roda 3D)
void drawCylinder(float radius, float height, int segments) {
    float x, y, z;
    float angle;

    // Lingkaran atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < segments; i++) {
        angle = 2.0f * 3.14159f * i / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height / 2);
    }
    glEnd();

    // Lingkaran bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    for (int i = segments - 1; i >= 0; i--) {
        angle = 2.0f * 3.14159f * i / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, -height / 2);
    }
    glEnd();

    // Sisi silinder
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        angle = 2.0f * 3.14159f * i / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);

        glNormal3f(cos(angle), sin(angle), 0.0f);
        glVertex3f(x, y, height / 2);
        glVertex3f(x, y, -height / 2);
    }
    glEnd();
}

// Fungsi untuk menggambar kotak 3D
void drawBox(float width, float height, float depth) {
    float w = width / 2;
    float h = height / 2;
    float d = depth / 2;

    glBegin(GL_QUADS);

    // Depan
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-w, -h, d);
    glVertex3f(w, -h, d);
    glVertex3f(w, h, d);
    glVertex3f(-w, h, d);

    // Belakang
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, h, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, -h, -d);

    // Atas
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-w, h, -d);
    glVertex3f(-w, h, d);
    glVertex3f(w, h, d);
    glVertex3f(w, h, -d);

    // Bawah
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-w, -h, -d);
    glVertex3f(w, -h, -d);
    glVertex3f(w, -h, d);
    glVertex3f(-w, -h, d);

    // Kanan
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(w, -h, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, h, d);
    glVertex3f(w, -h, d);

    // Kiri
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, -h, d);
    glVertex3f(-w, h, d);
    glVertex3f(-w, h, -d);

    glEnd();
}

// Fungsi untuk menggambar mobil 3D
void drawCustomTrapezoid(
    float x1, float y1, float z1, // Sudut bawah kiri depan
    float x2, float y2, float z2, // Sudut bawah kanan depan
    float x3, float y3, float z3, // Sudut atas kanan depan
    float x4, float y4, float z4, // Sudut atas kiri depan
    float x5, float y5, float z5, // Sudut bawah kiri belakang
    float x6, float y6, float z6, // Sudut bawah kanan belakang
    float x7, float y7, float z7, // Sudut atas kanan belakang
    float x8, float y8, float z8  // Sudut atas kiri belakang
) {
    glBegin(GL_QUADS);

    // Depan
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x1, y1, z1); // Bawah kiri depan
    glVertex3f(x2, y2, z2); // Bawah kanan depan
    glVertex3f(x3, y3, z3); // Atas kanan depan
    glVertex3f(x4, y4, z4); // Atas kiri depan

    // Belakang
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x5, y5, z5); // Bawah kiri belakang
    glVertex3f(x8, y8, z8); // Atas kiri belakang
    glVertex3f(x7, y7, z7); // Atas kanan belakang
    glVertex3f(x6, y6, z6); // Bawah kanan belakang

    // Kanan
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x2, y2, z2); // Bawah kanan depan
    glVertex3f(x6, y6, z6); // Bawah kanan belakang
    glVertex3f(x7, y7, z7); // Atas kanan belakang
    glVertex3f(x3, y3, z3); // Atas kanan depan

    // Kiri
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x1, y1, z1); // Bawah kiri depan
    glVertex3f(x4, y4, z4); // Atas kiri depan
    glVertex3f(x8, y8, z8); // Atas kiri belakang
    glVertex3f(x5, y5, z5); // Bawah kiri belakang

    // Atas
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x4, y4, z4); // Atas kiri depan
    glVertex3f(x3, y3, z3); // Atas kanan depan
    glVertex3f(x7, y7, z7); // Atas kanan belakang
    glVertex3f(x8, y8, z8); // Atas kiri belakang

    // Bawah
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x1, y1, z1); // Bawah kiri depan
    glVertex3f(x5, y5, z5); // Bawah kiri belakang
    glVertex3f(x6, y6, z6); // Bawah kanan belakang
    glVertex3f(x2, y2, z2); // Bawah kanan depan

    glEnd();
}

void drawMobil3D() {
    glPushMatrix();
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 1.5f, 0.0f); // Posisikan mobil di atas lantai
    glScalef(0.02f, 0.02f, 0.02f); // Perkecil ukuran mobil

    // Body utama mobil
    glColor3f(0.0f, 0.0f, 0.97f);
    drawBox(290.0f, 50.0f, 120.0f); // Lebar mobil diperbesar menjadi 120.0f

    // Atap mobil berbentuk trapesium
    glPushMatrix();
    glTranslatef(-35.0f, 25.0f, 0.0f); 
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCustomTrapezoid(
        -110.0f, 0.0f, 50.0f,  // Sudut bawah kiri depan
         130.0f, 0.0f, 50.0f,  // Sudut bawah kanan depan
          90.0f, 80.0f, 50.0f, // Sudut atas kanan depan
         -80.0f, 80.0f, 50.0f, // Sudut atas kiri depan
        -110.0f, 0.0f, -50.0f, // Sudut bawah kiri belakang
         130.0f, 0.0f, -50.0f, // Sudut bawah kanan belakang
         90.0f, 80.0f, -50.0f, // Sudut atas kanan belakang
        -80.0f, 80.0f, -50.0f  // Sudut atas kiri belakang
    );
    glPopMatrix();
    
    // Jendela depan
    glPushMatrix();
    glTranslatef(-35.0f, 25.0f, 0.0f); 
    glColor3f(0.6f, 0.8f, 1.0f); // Warna biru muda
    glBegin(GL_QUADS);
    glVertex3f(131.0f, 0.0f, 50.0f); // Bawah kiri
    glVertex3f(91.0f, 80.0f, 50.0f); // Atas kiri
    glVertex3f(91.0f, 80.0f, -50.0f); // Atas kanan
    glVertex3f(131.0f, 0.0f, -50.0f); // Bawah kanan
    glEnd();
    glPopMatrix();
    
    // Jendela samping 
    glPushMatrix();
    glTranslatef(0.0f, 25.0f, -9.0f); 
    glColor3f(0.6f, 0.8f, 1.0f); // Warna biru muda
    glBegin(GL_QUADS);
    glVertex3f(-130.0f, 10.0f, 60.0f); // Bawah kiri
    glVertex3f(80.0f, 10.0f, 60.0f); // Bawah kanan
    glVertex3f(50.0f, 70.0f, 60.0f); // Atas kanan
    glVertex3f(-110.0f, 70.0f, 60.0f); // Atas kiri
    glEnd();
    glPopMatrix();
    
    // Jendela samping 
    glPushMatrix();
    glTranslatef(0.0f, 25.0f, 9.0f); 
    glColor3f(0.6f, 0.8f, 1.0f); // Warna biru muda
    glBegin(GL_QUADS);
    glVertex3f(-130.0f, 10.0f, -60.0f); // Bawah kiri
    glVertex3f(80.0f, 10.0f, -60.0f); // Bawah kanan
    glVertex3f(50.0f, 70.0f, -60.0f); // Atas kanan
    glVertex3f(-110.0f, 70.0f, -60.0f); // Atas kiri
    glEnd();
    glPopMatrix();
    
    // Jendela belakang
	glPushMatrix();
	glTranslatef(-145.0f, 25.0f, 0.0f); 
	glColor3f(0.6f, 0.8f, 1.0f); // Warna biru muda
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 50.0f); // Bawah kiri
	glVertex3f(0.0f, 0.0f, -50.0f); // Bawah kanan
	glVertex3f(29.0f, 80.0f, -50.0f); // Atas kanan
	glVertex3f(29.0f, 80.0f, 50.0f); // Atas kiri
	glEnd();
	glPopMatrix();
	
	// Lampu depan kiri
    glPushMatrix();
    glTranslatef(146.0f, -10.0f, 40.0f); // Posisikan lampu depan kiri
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 0.0f); // Warna kuning untuk lampu
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float angle = i * 3.14159f / 180.0f;
        glVertex3f(cos(angle) * 10.0f, sin(angle) * 10.0f, 0.0f);
    }
    glEnd();
    glPopMatrix();

    // Lampu depan kanan
    glPushMatrix();
    glTranslatef(146.0f, -10.0f, -40.0f); // Posisikan lampu depan kanan
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 0.0f); // Warna kuning untuk lampu
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float angle = i * 3.14159f / 180.0f;
        glVertex3f(cos(angle) * 10.0f, sin(angle) * 10.0f, 0.0f);
    }
    glEnd();
    glPopMatrix();
    
    // Lampu belakang kiri
	glPushMatrix();
	glTranslatef(-145.0f, 0.0f, 40.0f);
	glColor3f(1.0f, 0.0f, 0.0f); // Warna merah
	drawBox(2.0f, 20.0f, 20.0f);
	glPopMatrix();
	
	// Lampu belakang kanan
	glPushMatrix();
	glTranslatef(-145.0f, 0.0f, -40.0f);
	glColor3f(1.0f, 0.0f, 0.0f); // Warna merah
	drawBox(2.0f, 20.0f, 20.0f);
	glPopMatrix();
	
    // Gagang pintu kiri
	glPushMatrix();
	glTranslatef(0.0f, 10.0f, 60.5f); // Sedikit di luar body mobil
	glColor3f(0.7f, 0.7f, 0.7f); // Warna abu-abu terang
	drawBox(20.0f, 5.0f, 2.0f);
	glPopMatrix();
	
	// Garis pemisah jendela samping kiri
	glPushMatrix();
	glTranslatef(-25.0f, 60.0f, 50.9f); // Sedikit di luar body mobil
	glColor3f(1.0f, 1.0f, 1.0f); // Warna putih
	drawBox(20.0f, 70.0f, 0.5f); // Garis tipis vertikal
	glPopMatrix();
	
	// Garis pemisah jendela samping kanan
	glPushMatrix();
	glTranslatef(-25.0f, 60.0f, -50.9f); // Sedikit di luar body mobil
	glColor3f(1.0f, 1.0f, 1.0f); // Warna putih
	drawBox(20.0f, 70.0f, 0.5f); // Garis tipis vertikal
	glPopMatrix();
	
	// Gagang pintu kanan
	glPushMatrix();
	glTranslatef(0.0f, 10.0f, -60.5f); // Sedikit di luar body mobil
	glColor3f(0.7f, 0.7f, 0.7f); // Warna abu-abu terang
	drawBox(20.0f, 5.0f, 2.0f);
	glPopMatrix();
	
	// Lampu polisi di atas mobil (gabungan 2 trapesium)
	glPushMatrix();
	glTranslatef(0.0f, 110.0f, 0.0f); // Posisi di atas atap mobil
	// Trapesium biru (sisi kiri)
	glColor3f(0.0f, 0.0f, 1.0f); // Warna biru
	glBegin(GL_QUADS);
	// Depan
	glVertex3f(-25.0f, 0.0f, 15.0f);  // Bawah kiri
	glVertex3f(0.0f, 0.0f, 15.0f);    // Bawah kanan
	glVertex3f(0.0f, 15.0f, 10.0f);   // Atas kanan
	glVertex3f(-20.0f, 15.0f, 10.0f); // Atas kiri
	// Belakang
	glVertex3f(-25.0f, 0.0f, -15.0f);  // Bawah kiri
	glVertex3f(0.0f, 0.0f, -15.0f);    // Bawah kanan
	glVertex3f(0.0f, 15.0f, -10.0f);   // Atas kanan
	glVertex3f(-20.0f, 15.0f, -10.0f); // Atas kiri
	// Atas
	glVertex3f(-20.0f, 15.0f, 10.0f);  // Depan kiri
	glVertex3f(0.0f, 15.0f, 10.0f);    // Depan kanan
	glVertex3f(0.0f, 15.0f, -10.0f);   // Belakang kanan
	glVertex3f(-20.0f, 15.0f, -10.0f); // Belakang kiri
	// Sisi kiri
	glVertex3f(-25.0f, 0.0f, 15.0f);   // Bawah depan
	glVertex3f(-25.0f, 0.0f, -15.0f);  // Bawah belakang
	glVertex3f(-20.0f, 15.0f, -10.0f); // Atas belakang
	glVertex3f(-20.0f, 15.0f, 10.0f);  // Atas depan
	// Sisi kanan
	glVertex3f(0.0f, 0.0f, 15.0f);    // Bawah depan
	glVertex3f(0.0f, 0.0f, -15.0f);   // Bawah belakang
	glVertex3f(0.0f, 15.0f, -10.0f);  // Atas belakang
	glVertex3f(0.0f, 15.0f, 10.0f);   // Atas depan
	glEnd();
	
	// Trapesium merah (sisi kanan)
	glColor3f(1.0f, 0.0f, 0.0f); // Warna merah
	glBegin(GL_QUADS);
	// Depan
	glVertex3f(0.0f, 0.0f, 15.0f);    // Bawah kiri
	glVertex3f(25.0f, 0.0f, 15.0f);   // Bawah kanan
	glVertex3f(20.0f, 15.0f, 10.0f);  // Atas kanan
	glVertex3f(0.0f, 15.0f, 10.0f);   // Atas kiri
	// Belakang
	glVertex3f(0.0f, 0.0f, -15.0f);    // Bawah kiri
	glVertex3f(25.0f, 0.0f, -15.0f);   // Bawah kanan
	glVertex3f(20.0f, 15.0f, -10.0f);  // Atas kanan
	glVertex3f(0.0f, 15.0f, -10.0f);   // Atas kiri
	// Atas
	glVertex3f(0.0f, 15.0f, 10.0f);    // Depan kiri
	glVertex3f(20.0f, 15.0f, 10.0f);   // Depan kanan
	glVertex3f(20.0f, 15.0f, -10.0f);  // Belakang kanan
	glVertex3f(0.0f, 15.0f, -10.0f);   // Belakang kiri
	// Sisi kiri
	glVertex3f(0.0f, 0.0f, 15.0f);     // Bawah depan
	glVertex3f(0.0f, 0.0f, -15.0f);    // Bawah belakang
	glVertex3f(0.0f, 15.0f, -10.0f);   // Atas belakang
	glVertex3f(0.0f, 15.0f, 10.0f);    // Atas depan
	// Sisi kanan
	glVertex3f(25.0f, 0.0f, 15.0f);    // Bawah depan
	glVertex3f(25.0f, 0.0f, -15.0f);   // Bawah belakang
	glVertex3f(20.0f, 15.0f, -10.0f);  // Atas belakang
	glVertex3f(20.0f, 15.0f, 10.0f);   // Atas depan
	glEnd();
	
	glPopMatrix();
    // Bumper
	glPushMatrix();
	glTranslatef(0.1f, -20.0f, 0.0f);
	glColor3f(0.4f, 0.4f, 0.4f); // Warna abu-abu
	drawBox(289.0f, 10.0f, 120.5f);
	glPopMatrix();
    
    // Roda depan kanan
    glPushMatrix();
    glTranslatef(100.0f, -25.0f, 60.0f); // Sesuaikan posisi roda depan kanan
    glColor3f(0.0f, 0.0f, 0.0f); // Warna hitam untuk roda luar
    drawCylinder(30.0f, 15.0f, 30); // Perbesar ukuran roda
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk lingkaran dalam
    drawCylinder(15.0f, 16.0f, 30); // Tambahkan lingkaran putih di dalam roda
    glPopMatrix();

    // Roda depan kiri
    glPushMatrix();
    glTranslatef(100.0f, -25.0f, -60.0f); // Sesuaikan posisi roda depan kiri
    glColor3f(0.0f, 0.0f, 0.0f); // Warna hitam untuk roda luar
    drawCylinder(30.0f, 15.0f, 30); // Perbesar ukuran roda
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk lingkaran dalam
    drawCylinder(15.0f, 16.0f, 30); // Tambahkan lingkaran putih di dalam roda
    glPopMatrix();

    // Roda belakang kanan
    glPushMatrix();
    glTranslatef(-100.0f, -25.0f, 60.0f); // Sesuaikan posisi roda belakang kanan
    glColor3f(0.0f, 0.0f, 0.0f); // Warna hitam untuk roda luar
    drawCylinder(30.0f, 15.0f, 30); // Perbesar ukuran roda
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk lingkaran dalam
    drawCylinder(15.0f, 16.0f, 30); // Tambahkan lingkaran putih di dalam roda
    glPopMatrix();

    // Roda belakang kiri
    glPushMatrix();
    glTranslatef(-100.0f, -25.0f, -60.0f); // Sesuaikan posisi roda belakang kiri
    glColor3f(0.0f, 0.0f, 0.0f); // Warna hitam untuk roda luar
    drawCylinder(30.0f, 15.0f, 30); // Perbesar ukuran roda
    glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk lingkaran dalam
    drawCylinder(15.0f, 16.0f, 30); // Tambahkan lingkaran putih di dalam roda
    glPopMatrix();
    
    

    glPopMatrix();
}


// Fungsi untuk menggambar grid lantai
void drawGrid() {
    const float Z_MIN = -50, Z_MAX = 50;
    const float X_MIN = -50, X_MAX = 50;
    const float gap = 1.5;
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    for (double i = Z_MIN; i < Z_MAX; i += gap) {
        glVertex3f(i, 0, Z_MIN);
        glVertex3f(i, 0, Z_MAX);
    }
    for (double i = X_MIN; i < X_MAX; i += gap) {
        glVertex3f(X_MIN, 0, i);
        glVertex3f(X_MAX, 0, i);
    }
    glEnd();
}

void setProjection(int projNum) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    switch (projNum) {
        // Proyeksi Perspektif (1-3)
        case 1: // 1-point perspective
            strcpy(projectionName, "Perspective: 1-Point");
            projectionType = 0;
            glMatrixMode(GL_PROJECTION);
            gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.0f, 5.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            break;
            
        case 2: // 2-point perspective
            strcpy(projectionName, "Perspective: 2-Point");
            projectionType = 0;
            glMatrixMode(GL_PROJECTION);
            gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(15.0f, 5.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            break;
            
        case 3: // 3-point perspective
            strcpy(projectionName, "Perspective: 3-Point");
            projectionType = 0;
            glMatrixMode(GL_PROJECTION);
            gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(15.0f, 10.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.577f, 0.577f, 0.577f);
            break;
            
        // Proyeksi Orthographic (4-9)
        case 4: // Plan view (top)
            strcpy(projectionName, "Orthographic: Plan View (Top)");
            projectionType = 1;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-20, 20, -15, 15, 0.1, 1000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0, 20, 0, 0, 0, 0, 0, 0, -1);
            break;
            
        case 5: // Front view
            strcpy(projectionName, "Orthographic: Front View");
            projectionType = 1;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-20, 20, -15, 15, 0.1, 1000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
            break;
            
        case 6: // Side view
            strcpy(projectionName, "Orthographic: Side View");
            projectionType = 1;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-20, 20, -15, 15, 0.1, 1000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(20, 0, 0, 0, 0, 0, 0, 1, 0);
            break;
            
        case 7: // Isometric
            strcpy(projectionName, "Orthographic: Isometric");
            projectionType = 1;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-20, 20, -15, 15, 0.1, 1000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);
            break;
            
        case 8: // Dimetric
            strcpy(projectionName, "Orthographic: Dimetric");
            projectionType = 1;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-20, 20, -15, 15, 0.1, 1000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(15, 10, 15, 0, 0, 0, 0, 1, 0);
            break;
            
        case 9: // Trimetric
            strcpy(projectionName, "Orthographic: Trimetric");
            projectionType = 1;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-20, 20, -15, 15, 0.1, 1000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(10, 15, 20, 0, 0, 0, 0, 1, 0);
            break;
            
        default: // Default camera (first person view)
            strcpy(projectionName, "Projection Trial");
            projectionType = 0;
            glMatrixMode(GL_PROJECTION);
            gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
            break;
    }
}


// Fungsi Reshape
void reshape(int w, int h) {
    if (h == 0) h = 1;
    ratio = 1.0f * w / h;
    glViewport(0, 0, w, h);
    
    // Tetap gunakan proyeksi yang dipilih sebelumnya saat ukuran window berubah
    setProjection(orthoMode);
}

// Fungsi Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Mode proyeksi sesuai dengan yang dipilih
    setProjection(orthoMode);
    
    // Gambar objek 3D
    drawGrid();
    drawMobil3D();
    
    // Setup untuk menampilkan teks
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Render teks info proyeksi di pojok kiri atas
    renderText(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, GLUT_BITMAP_HELVETICA_12, projectionName);
    renderText(10, glutGet(GLUT_WINDOW_HEIGHT) - 40, GLUT_BITMAP_HELVETICA_12, "Press 1-9 to change projection");
    
    // Kembalikan ke state sebelumnya
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glutSwapBuffers();
}

// Fungsi untuk menangani input keyboard (modifikasi utama di sini)
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            // Langsung gunakan nomor tombol untuk beralih proyeksi
            orthoMode = key - '0'; // Konversi dari ASCII ke integer (1-9)
            glutPostRedisplay();
            break;
            
        case '0': // Reset ke mode default
            orthoMode = 0;
            glutPostRedisplay();
            break;
            

    }
}
void releaseKey(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_UP: case GLUT_KEY_DOWN: deltaMove = 0; break;
        case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
    }
}

void update() {
    if (deltaMove && projectionMode == 0 && orthoMode == 0) { // Hanya bergerak di perspective mode standar
        moveMeFlat(deltaMove);
    }
    glutPostRedisplay();
}




void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

int main(int argc, char** argv) {
	strcpy(projectionName, "yayaya");
    orthoMode = 0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Mobilang Suka Tapi Malu");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutKeyboardFunc(keyboard); 
    glutSpecialUpFunc(releaseKey);
    init();
    glutMainLoop();
    return 0;
}
