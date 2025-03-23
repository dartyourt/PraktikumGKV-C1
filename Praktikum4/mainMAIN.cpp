#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <cmath>

// Ukuran jendela
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Variabel Kamera
float angle = 0.0f, deltaAngle = 0.0f, ratio;
float x = 0.0f, y = 2.0f, z = 15.0f; // Posisi awal kamera
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
int deltaMove = 0;

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

// Fungsi untuk menggambar kerucut (cone)
void drawCone(float radius, float height, int segments) {
    float x, y, z;
    float angle;

    // Lingkaran alas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    for (int i = segments - 1; i >= 0; i--) {
        angle = 2.0f * 3.14159f * i / segments;
        x = radius * cos(angle);
        z = radius * sin(angle);
        glVertex3f(x, 0, z);
    }
    glEnd();

    // Sisi kerucut
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; i++) {
        angle = 2.0f * 3.14159f * i / segments;
        x = radius * cos(angle);
        z = radius * sin(angle);
        
        float nextAngle = 2.0f * 3.14159f * ((i+1) % segments) / segments;
        float nextX = radius * cos(nextAngle);
        float nextZ = radius * sin(nextAngle);
        
        // Calculate normal for this face
        float nx = x + nextX;
        float nz = z + nextZ;
        float len = sqrt(nx*nx + height*height + nz*nz);
        nx /= len;
        float ny = height/len;
        nz /= len;
        
        glNormal3f(nx, ny, nz);
        glVertex3f(0, height, 0);      // Puncak kerucut
        glVertex3f(x, 0, z);           // Titik alas saat ini
        glVertex3f(nextX, 0, nextZ);   // Titik alas berikutnya
    }
    glEnd();
}

// Fungsi untuk menggambar pohon
void drawTree(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(scale, scale, scale);
    
    // Batang pohon (silinder)
    glColor3f(0.55f, 0.27f, 0.07f); // Warna coklat
    glPushMatrix();
    glTranslatef(0, 1.0f, 0);
    glRotatef(-90, 1, 0, 0); // Putar silinder agar vertikal
    drawCylinder(0.5f, 4.0f, 20);
    glPopMatrix();
    
    // Daun pohon (kerucut)
    glColor3f(0.0f, 0.5f, 0.0f); // Warna hijau tua
    glPushMatrix();
    glTranslatef(0, 3.0f, 0); // Posisikan di atas batang pohon
    drawCone(2.0f, 4.0f, 20);
    glPopMatrix();
    
    // Daun pohon tingkat kedua (kerucut)
    glColor3f(0.0f, 0.6f, 0.0f); // Warna hijau sedikit lebih terang
    glPushMatrix();
    glTranslatef(0, 5.0f, 0); // Posisikan di atas daun pertama
    drawCone(1.5f, 3.0f, 20);
    glPopMatrix();
    
    // Daun pohon tingkat ketiga (kerucut paling atas)
    glColor3f(0.0f, 0.7f, 0.0f); // Warna hijau lebih terang
    glPushMatrix();
    glTranslatef(0, 6.5f, 0); // Posisikan di atas daun kedua
    drawCone(1.0f, 2.0f, 20);
    glPopMatrix();
    
    glPopMatrix();
}
// Fungsi untuk menggambar grid lantai
void drawGrid() {
    const float Z_MIN = -50, Z_MAX = 50;
    const float X_MIN = -50, X_MAX = 50;
    
    // Padang rumput hijau
    glColor3f(0.0, 0.6, 0.1); // Warna hijau rumput
    glBegin(GL_QUADS);
    glVertex3f(X_MIN, 0, Z_MIN);
    glVertex3f(X_MAX, 0, Z_MIN);
    glVertex3f(X_MAX, 0, Z_MAX);
    glVertex3f(X_MIN, 0, Z_MAX);
    glEnd();
    
    // Variasi warna rumput untuk efek yang lebih alami
    glColor3f(0.0, 0.55, 0.05); // Hijau sedikit berbeda
    glBegin(GL_QUADS);
    for (float x = X_MIN; x < X_MAX; x += 5.0) {
        for (float z = Z_MIN; z < Z_MAX; z += 5.0) {
            if ((int)(x + z) % 2 == 0) { // Pola papan catur untuk variasi
                glVertex3f(x, 0.01, z);
                glVertex3f(x + 5.0, 0.01, z);
                glVertex3f(x + 5.0, 0.01, z + 5.0);
                glVertex3f(x, 0.01, z + 5.0);
            }
        }
    }
    glEnd();
}

// Fungsi untuk memutar kamera
void orientMe(float ang) {
    lx = sin(ang);
    lz = -cos(ang);
}

// Fungsi untuk maju/mundur kamera
void moveMeFlat(int i) {
    x += i * lx * 0.02f; // Pergerakan lebih lambat
    z += i * lz * 0.02f;
}

// Fungsi Reshape
void reshape(int w, int h) {
    if (h == 0) h = 1;
    ratio = 1.0f * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

    drawGrid();
    drawMobil3D();
    
    drawTree(-20, 20, 1.0);  // Pohon kiri depan
    drawTree(-30, -25, 1.2); // Pohon kiri belakang
    drawTree(15, 30, 0.8);   // Pohon kanan depan
    drawTree(25, -20, 1.1);  // Pohon kanan belakang
    drawTree(40, 10, 1.3);   // Pohon kanan jauh
    drawTree(-40, 5, 1.0);   // Pohon kiri jauh

    glutSwapBuffers();
}

// Fungsi untuk menangani input keyboard
void pressKey(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_UP: deltaMove = 1; break;
        case GLUT_KEY_DOWN: deltaMove = -1; break;
        case GLUT_KEY_LEFT: deltaAngle = -0.002f; break; // Rotasi lebih lambat
        case GLUT_KEY_RIGHT: deltaAngle = 0.002f; break;
    }
}

void releaseKey(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_UP: case GLUT_KEY_DOWN: deltaMove = 0; break;
        case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
    }
}

void update() {
    if (deltaMove) moveMeFlat(deltaMove);
    if (deltaAngle) {
        angle += deltaAngle;
        orientMe(angle);
    }
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Mobilang Suka Tapi Malu");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    init();
    glutMainLoop();
    return 0;
}
