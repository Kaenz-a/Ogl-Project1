#include <stdio.h>
#include <string.h>

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogldev_math_3d.h"
#include "camera.h"
#include "world_transform.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <cmath>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

GLuint VBO;
GLuint IBO;
GLuint VBO2;
GLuint IBO2;
GLuint gWVPLocation;
GLuint gTranslationLocation;

static double duration=0;
static std::clock_t start;


WorldTrans CubeWorldTransform;
Vector3f CameraPos(0.0f, 0.0f, -3.0f);
Vector3f CameraTarget(0.0f, 0.0f, 1.0f);
Vector3f CameraUp(0.0f, 1.0f, 0.0f);
Camera GameCamera(WINDOW_WIDTH, WINDOW_HEIGHT, CameraPos, CameraTarget, CameraUp);

float FOV = 90.0f;
float zNear = 0.5f;
float zFar = 100.0f;
PersProjInfo persProjInfo = { FOV, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, zNear, zFar };


static void RenderSceneCB()
{
    
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    static float Scale_x = 0.0f;
    static float Scale_y = 0.0f;
    static float Scale_z = 0.0f;
    static float Delta_x;
    static float Delta_y;
    static float Delta_z;

    
    Matrix4f Translation(1.0f, 0.0f, 0.0f, Scale_x,
                         0.0f, 1.0f, 0.0f, Scale_y,
                         0.0f, 0.0f, 1.0f, Scale_z,
                         0.0f, 0.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(gTranslationLocation, 1, GL_TRUE, &Translation.m[0][0]);
    
    std::cout << "Operation took " << duration << "seconds" << std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    GameCamera.OnRender();

#ifdef _WIN64
    float YRotationAngle = 0.1f;
#else
    float YRotationAngle = 1.0f;
#endif

    CubeWorldTransform.SetPosition(0.0f, 0.0f, 2.0f);
    CubeWorldTransform.Rotate(0.0f, 0.0f, 0.0f);
    Matrix4f World = CubeWorldTransform.GetMatrix();

    Matrix4f View = GameCamera.GetMatrix();

    Matrix4f Projection;
    Projection.InitPersProjTransform(persProjInfo);

    Matrix4f WVP = Projection * View * World;

    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &WVP.m[0][0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    
    glDrawElements(GL_TRIANGLES, 500, GL_UNSIGNED_INT, 0);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    Scale_x = 0.0f;
    Scale_y = 0.0f;
    Scale_z = 0.0f;
    if (duration <= 0) {
        Delta_x = 0.0f;
        Delta_y = 0.0f;
        Delta_z = 0.0f;
        std::cout << "if 1 ";
    }
    if (4 > duration&& duration > 0) {
        Delta_x = -0.007f+ Delta_x;
        std::cout << "if 2 ";
    }
    if (duration == 3 ) {
        Delta_x = -Delta_x;
        std::cout << "if 3 ";
    }
    if (6.5 > duration && duration > 3 ) {
        Delta_z = -0.007f + Delta_z;
        std::cout << "if 4 ";
    }
    if (duration == 5.5 ) {
        Delta_z = -Delta_z;
        std::cout << "if 5 ";
    }
    if (10 > duration && duration > 6.5) {
        Delta_z = +0.007f + Delta_z;
        std::cout << "if 6 ";
    }
    if (duration == 10) {
        Delta_z = - Delta_z;
        std::cout << "if 7 ";
    }
    if (15 > duration && duration> 10) {
        Delta_x = +0.01f + Delta_x;
        std::cout << "if 8 ";
    }
    if (12.5 > duration && duration > 11.5) {
        Delta_y = +0.003f + Delta_y;
        std::cout << "if 9 ";
    }
    if (12.5 == duration) {
        Delta_y = -Delta_y;
        std::cout << "if 10 ";
    }
    if (13 > duration && duration > 12.5) {
        Delta_y = -0.003f + Delta_y;
        std::cout << "if 11 ";
    }
    if (13 == duration) {
        Delta_y = - Delta_y;
        std::cout << "if 12 ";
    }

    Scale_x += Delta_x;
    Scale_y += Delta_y;
    Scale_z += Delta_z;

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO2);
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

    std::cout << "scale x= " << Scale_x << " ";
    glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
    
    glutPostRedisplay();
    
    
    glutSwapBuffers();
   
}




static void KeyboardCB(unsigned char key, int mouse_x, int mouse_y)
{
    if (key == 'q' || key == 27) {
        exit(0);
    }

    GameCamera.OnKeyboard(key);
}


static void SpecialKeyboardCB(int key, int mouse_x, int mouse_y)
{
    GameCamera.OnKeyboard(key);
}


static void PassiveMouseCB(int x, int y)
{
    GameCamera.OnMouse(x, y);
}


static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutKeyboardFunc(KeyboardCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
}


struct Vertex {
    Vector3f pos;
    Vector3f color;

    Vertex() {}

    
    Vertex(float x, float y, float z)
    {
        pos = Vector3f(x, y, z);

        float red = (float)rand() / (float)RAND_MAX;
        float green = (float)rand() / (float)RAND_MAX;
        float blue = (float)rand() / (float)RAND_MAX;
        color = Vector3f(red, green, blue);
        //color = Vector3f(1.0f, 1.0f, 1.0f);
        
        
    }
    
};
struct Vertex1 {
    Vector3f pos;
    Vector3f color;
    Vector3f mov;

    Vertex1() {}


    Vertex1(float x, float y, float z, float a, float b, float c)
    {
        pos = Vector3f(x, y, z);

        float red = (float)rand() / (float)RAND_MAX;
        float green = (float)rand() / (float)RAND_MAX;
        float blue = (float)rand() / (float)RAND_MAX;
        color = Vector3f(red, green, blue);
        //color = Vector3f(1.0f, 1.0f, 1.0f);

        mov = Vector3f(1.0f, 1.0f, 1.0f);

    }

};




static void CreateVertexBuffer()
{
    Vertex Vertices[200];
    Vertex1 Vertices2[200];
    //zemin
    Vertices[0] = Vertex(12.0f, -5.0f, 8.0f);
    Vertices[1] = Vertex(-12.0f, -5.0f, -8.0f);
    Vertices[2] = Vertex(-12.0f, -5.0f, 8.0f);
    Vertices[5] = Vertex(12.0f, -5.0f, -8.0f);

    //sað duvar
    Vertices[3] = Vertex(12.0f, 5.0f, 8.0f);
    Vertices[4] = Vertex(12.0f, 5.0f, -8.0f);
    //sol duvar
    Vertices[8] = Vertex(-12.0f, 5.0f, 8.0f);
    Vertices[9] = Vertex(-12.0f, 5.0f, -8.0f);

    //masa
    Vertices[6] = Vertex(-3.0f, -1.0f, -3.0f);
    Vertices[7] = Vertex( 3.0f, -1.0f, -3.0f);
    Vertices[10]= Vertex(-3.0f, -1.0f, 3.0f);
    Vertices[11]= Vertex( 3.0f, -1.0f, 3.0f);
    Vertices[20] = Vertex(-3.0f,-0.99f, -3.0f);
    Vertices[21] = Vertex(3.0f, -0.99f, -3.0f);
    Vertices[22] = Vertex(-3.0f,-0.99f, 3.0f);
    Vertices[23] = Vertex(3.0f, -0.99f, 3.0f);
    //masa ayak
    Vertices[12] = Vertex(-0.25f,-5.0f, 0.25f);
    Vertices[13] = Vertex(-0.25f,-5.0f,-0.25f);
    Vertices[14] = Vertex( 0.25f,-5.0f, 0.25f);
    Vertices[15] = Vertex( 0.25f,-5.0f, -0.25f);
    Vertices[16] = Vertex(-0.25f,-1.0f, 0.25f);
    Vertices[17] = Vertex(-0.25f,-1.0f, -0.25f);
    Vertices[18] = Vertex( 0.25f,-1.0f, 0.25f);
    Vertices[19] = Vertex( 0.25f,-1.0f, -0.25f);

    //kamera sandalye 1.ayak
    Vertices[24] = Vertex( -1.0f, -5.0f, -4.0f);
    Vertices[25] = Vertex( -1.0f, -5.0f, -4.1f);
    Vertices[26] = Vertex( -0.9f, -5.0f, -4.0f);
    Vertices[27] = Vertex( -0.9f, -5.0f, -4.1f);
    Vertices[28] = Vertex( -1.0f, -2.8f, -4.0f);
    Vertices[29] = Vertex( -1.0f, -2.8f, -4.1f);
    Vertices[30] = Vertex( -0.9f, -2.8f, -4.0f);
    Vertices[31] = Vertex( -0.9f, -2.8f, -4.1f);
    //kamera sandalye 2.ayak
    Vertices[32] = Vertex(-1.0f, -5.0f, -5.9f);
    Vertices[33] = Vertex(-1.0f, -5.0f, -6.0f);
    Vertices[34] = Vertex(-0.9f, -5.0f, -5.9f);
    Vertices[35] = Vertex(-0.9f, -5.0f, -6.0f);
    Vertices[36] = Vertex(-1.0f, -2.8f, -5.9f);
    Vertices[37] = Vertex(-1.0f, -2.8f, -6.0f);
    Vertices[38] = Vertex(-0.9f, -2.8f, -5.9f);
    Vertices[39] = Vertex(-0.9f, -2.8f, -6.0f);
    //kamera sandalye 3.ayak
    Vertices[40] = Vertex( 1.0f, -5.0f, -4.0f);
    Vertices[41] = Vertex( 1.0f, -5.0f, -4.1f);
    Vertices[42] = Vertex( 0.9f, -5.0f, -4.0f);
    Vertices[43] = Vertex( 0.9f, -5.0f, -4.1f);
    Vertices[44] = Vertex( 1.0f, -2.8f, -4.0f);
    Vertices[45] = Vertex( 1.0f, -2.8f, -4.1f);
    Vertices[46] = Vertex( 0.9f, -2.8f, -4.0f);
    Vertices[47] = Vertex( 0.9f, -2.8f, -4.1f);
    //kamera sandalye 4.ayak
    Vertices[48] = Vertex( 1.0f, -5.0f, -5.9f);
    Vertices[49] = Vertex( 1.0f, -5.0f, -6.0f);
    Vertices[50] = Vertex( 0.9f, -5.0f, -5.9f);
    Vertices[51] = Vertex( 0.9f, -5.0f, -6.0f);
    Vertices[52] = Vertex( 1.0f, -2.8f, -5.9f);
    Vertices[53] = Vertex( 1.0f, -2.8f, -6.0f);
    Vertices[54] = Vertex( 0.9f, -2.8f, -5.9f);
    Vertices[55] = Vertex( 0.9f, -2.8f, -6.0f);
    //kamera sandalye yaslanmak
    Vertices[56] = Vertex(-1.0f, -0.6f, -6.0f);
    Vertices[57] = Vertex( 1.0f, -0.6f, -6.0f);

    //polis sandalye 1.ayak
    Vertices[58] = Vertex( 1.0f, -5.0f, 4.0f);
    Vertices[59] = Vertex( 1.0f, -5.0f, 4.1f);
    Vertices[60] = Vertex( 0.9f, -5.0f, 4.0f);
    Vertices[61] = Vertex( 0.9f, -5.0f, 4.1f);
    Vertices[62] = Vertex( 1.0f, -2.8f, 4.0f);
    Vertices[63] = Vertex( 1.0f, -2.8f, 4.1f);
    Vertices[64] = Vertex( 0.9f, -2.8f, 4.0f);
    Vertices[65] = Vertex( 0.9f, -2.8f, 4.1f);
    //polis sandalye 2.ayak
    Vertices[66] = Vertex( 1.0f, -5.0f, 5.9f);
    Vertices[67] = Vertex( 1.0f, -5.0f, 6.0f);
    Vertices[68] = Vertex( 0.9f, -5.0f, 5.9f);
    Vertices[69] = Vertex( 0.9f, -5.0f, 6.0f);
    Vertices[70] = Vertex( 1.0f, -2.8f, 5.9f);
    Vertices[71] = Vertex( 1.0f, -2.8f, 6.0f);
    Vertices[72] = Vertex( 0.9f, -2.8f, 5.9f);
    Vertices[73] = Vertex( 0.9f, -2.8f, 6.0f);
    //polis sandalye 3.ayak
    Vertices[74] = Vertex(-1.0f, -5.0f, 4.0f);
    Vertices[75] = Vertex(-1.0f, -5.0f, 4.1f);
    Vertices[76] = Vertex(-0.9f, -5.0f, 4.0f);
    Vertices[77] = Vertex(-0.9f, -5.0f, 4.1f);
    Vertices[78] = Vertex(-1.0f, -2.8f, 4.0f);
    Vertices[79] = Vertex(-1.0f, -2.8f, 4.1f);
    Vertices[80] = Vertex(-0.9f, -2.8f, 4.0f);
    Vertices[81] = Vertex(-0.9f, -2.8f, 4.1f);
    //kamera sandalye 4.ayak
    Vertices[82] = Vertex(-1.0f, -5.0f, 5.9f);
    Vertices[83] = Vertex(-1.0f, -5.0f, 6.0f);
    Vertices[84] = Vertex(-0.9f, -5.0f, 5.9f);
    Vertices[85] = Vertex(-0.9f, -5.0f, 6.0f);
    Vertices[86] = Vertex(-1.0f, -2.8f, 5.9f);
    Vertices[87] = Vertex(-1.0f, -2.8f, 6.0f);
    Vertices[88] = Vertex(-0.9f, -2.8f, 5.9f);
    Vertices[89] = Vertex(-0.9f, -2.8f, 6.0f);
    //kamera sandalye yaslanmak
    Vertices[90] = Vertex(1.0f, -0.6f, 6.0f);
    Vertices[91] = Vertex(-1.0f, -0.6f, 6.0f);
    //cam
    Vertices[92] = Vertex(11.999f, 9.5f, 7.5f);
    Vertices[93] = Vertex(11.999f, 9.5f,-7.5f);
    Vertices[94] = Vertex(11.999f,-1.8f, 7.5f);
    Vertices[95] = Vertex(11.999f,-1.8f,-7.5f);
    //lamba kablo
    Vertices[96] = Vertex(0.01f, 10.0f, 0.01f);
    Vertices[97] = Vertex(-0.01f, 10.0f, 0.01f);
    Vertices[98] = Vertex( 0.01f, 2.0f, 0.01f);
    Vertices[99] = Vertex(-0.01f, 2.0f, 0.01f);
    //lamba
    Vertices[100] = Vertex( 0.25f, 2.0f, 0.25f);
    Vertices[101] = Vertex( 0.25f, 2.0f,-0.25f);
    Vertices[102] = Vertex(-0.25f, 2.0f, 0.25f);
    Vertices[103] = Vertex(-0.25f, 2.0f,-0.25f);
    Vertices[104] = Vertex( 0.25f, 1.5f, 0.25f);
    Vertices[105] = Vertex( 0.25f, 1.5f,-0.25f);
    Vertices[106] = Vertex(-0.25f, 1.5f, 0.25f);
    Vertices[107] = Vertex(-0.25f, 1.5f,-0.25f);

    //Kamera 1.bacak
    Vertices[108] = Vertex(-1.0f, -5.0f,-3.7f);
    Vertices[109] = Vertex(-1.0f, -5.0f,-3.9f);
    Vertices[110] = Vertex(-0.8f, -5.0f,-3.7f);
    Vertices[111] = Vertex(-0.8f, -5.0f,-3.9f);

    Vertices[112] = Vertex(-1.0f, -2.5f,-3.7f);
    Vertices[113] = Vertex(-1.0f, -2.5f,-3.9f);

    Vertices[114] = Vertex(-0.8f, -2.5f,-3.7f);
    Vertices[115] = Vertex(-0.8f, -2.5f,-3.9f);
    //kamera 2.bacak
    Vertices[116] = Vertex( 1.0f, -5.0f,-3.7f);
    Vertices[117] = Vertex( 1.0f, -5.0f,-3.9f);
    Vertices[118] = Vertex( 0.8f, -5.0f,-3.7f);
    Vertices[119] = Vertex( 0.8f, -5.0f,-3.9f);

    Vertices[120] = Vertex( 1.0f, -2.5f,-3.7f);//sa
    Vertices[121] = Vertex( 1.0f, -2.5f,-3.9f);

    Vertices[122] = Vertex( 0.8f, -2.5f,-3.7f);//sol
    Vertices[123] = Vertex( 0.8f, -2.5f,-3.9f);
    //kamera omurga
    Vertices[124] = Vertex(-0.1f, -2.5f,-5.6f);
    Vertices[125] = Vertex(-0.1f, -2.5f,-5.8f);
    Vertices[126] = Vertex( 0.1f, -2.5f,-5.6f);
    Vertices[127] = Vertex( 0.1f, -2.5f,-5.8f);
    Vertices[128] = Vertex(-0.1f, 0.0f, -5.6f);
    Vertices[129] = Vertex(-0.1f, 0.0f, -5.8f);
    Vertices[130] = Vertex( 0.1f, 0.0f, -5.6f);
    Vertices[131] = Vertex( 0.1f, 0.0f, -5.8f);

    //kamera sol kol
    Vertices[132] = Vertex(-1.6f,-2.7f, -5.6f);
    Vertices[133] = Vertex(-1.8f,-2.7f, -5.6f);
    Vertices[134] = Vertex(-1.8f,-2.7f, -5.8f);
    Vertices[135] = Vertex(-0.1f, 0.0f, -5.6f);
    Vertices[136] = Vertex(-0.1f, 0.0f, -5.8f);
    Vertices[137] = Vertex(-0.1f,-0.2f, -5.6f);

    //kamera sað kol
    Vertices[138] = Vertex( 1.6f, -2.7f,-5.6f);
    Vertices[139] = Vertex( 1.8f, -2.7f,-5.6f);
    Vertices[140] = Vertex( 1.8f, -2.7f,-5.8f);
    Vertices[141] = Vertex( 0.1f, 0.0f, -5.6f);
    Vertices[142] = Vertex( 0.1f, 0.0f, -5.8f);
    Vertices[143] = Vertex( 0.1f, -0.2f,-5.6f);
    //polis body
    Vertices2[144] = Vertex1(-0.75f, -2.8f, 4.25f, 1.0f, 1.0f, 1.0f);
    Vertices2[145] = Vertex1(-0.75f, -2.8f, 5.75f, 1.0f, 1.0f, 1.0f);
    Vertices2[146] = Vertex1(0.75f, -2.8f, 4.25f, 1.0f, 1.0f, 1.0f);
    Vertices2[147] = Vertex1(0.75f, -2.8f, 5.75f, 1.0f, 1.0f, 1.0f);
    Vertices2[148] = Vertex1(-0.75f, 0.2f, 4.25f, 1.0f, 1.0f, 1.0f);
    Vertices2[149] = Vertex1(-0.75f, 0.2f, 5.75f, 1.0f, 1.0f, 1.0f);
    Vertices2[150] = Vertex1(0.75f, 0.2f, 4.25f, 1.0f, 1.0f, 1.0f);
    Vertices2[151] = Vertex1(0.75f, 0.2f, 5.75f, 1.0f, 1.0f, 1.0f);

    //polis kafa
    Vertices2[152] = Vertex1(-0.375f, 0.2f, 4.625f, 1.0f, 1.0f, 1.0f);
    Vertices2[153] = Vertex1(-0.375f, 0.2f, 5.375f, 1.0f, 1.0f, 1.0f);
    Vertices2[154] = Vertex1(0.375f, 0.2f, 4.625f, 1.0f, 1.0f, 1.0f);
    Vertices2[155] = Vertex1(0.375f, 0.2f, 5.375f, 1.0f, 1.0f, 1.0f);
    Vertices2[156] = Vertex1(-0.375f, 0.95f, 4.625f, 1.0f, 1.0f, 1.0f);
    Vertices2[157] = Vertex1(-0.375f, 0.95f, 5.375f, 1.0f, 1.0f, 1.0f);
    Vertices2[158] = Vertex1(0.375f, 0.95f, 4.625f, 1.0f, 1.0f, 1.0f);
    Vertices2[159] = Vertex1(0.375f, 0.95f, 5.375f, 1.0f, 1.0f, 1.0f);
    // polis sol goz
    Vertices2[160] = Vertex1(-0.25f, 0.70f, 4.6249f, 1.0f, 1.0f, 1.0f);
    Vertices2[161] = Vertex1(-0.25f, 0.5f, 4.6249f, 1.0f, 1.0f, 1.0f);
    Vertices2[162] = Vertex1(-0.05f, 0.70f, 4.6249f, 1.0f, 1.0f, 1.0f);
    Vertices2[163] = Vertex1(-0.05f, 0.5f, 4.6249f, 1.0f, 1.0f, 1.0f);

    // polis sag goz
    Vertices2[164] = Vertex1(0.25f, 0.70f, 4.6249f, 1.0f, 1.0f, 1.0f);
    Vertices2[165] = Vertex1(0.25f, 0.5f, 4.6249f, 1.0f, 1.0f, 1.0f);
    Vertices2[166] = Vertex1(0.05f, 0.70f, 4.6249f, 1.0f, 1.0f, 1.0f);
    Vertices2[167] = Vertex1(0.05f, 0.5f, 4.6249f, 1.0f, 1.0f, 1.0f);

    // polis agiz
    Vertices2[168] = Vertex1(-0.25f,0.3910f, 4.6249f, 1.0f, 1.0f, 1.0f);
    Vertices2[169] = Vertex1(-0.25f,0.2840f, 4.6249f, 1.0f, 1.0f, 1.0f);
    Vertices2[170] = Vertex1( 0.25f,0.3910f, 4.6249f, 1.0f, 1.0f, 1.0f);
    Vertices2[171] = Vertex1( 0.25f,0.2840f, 4.6249f, 1.0f, 1.0f, 1.0f);

        
    
    
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glGenBuffers(2, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
    unsigned int Indices[] = {
                              //zemin
                              0, 1, 2,
                              0, 5, 1,
                              //sol duvar
                              8, 2, 9,
                              9, 2, 1,  
                              //sað duvar
                              3, 4, 0,
                              4, 5, 0,
                              //karÞý
                              8, 3, 2,
                              2, 3, 0,
                              //arka
                              9, 1, 4,
                              1, 5, 4,
                              //tavan
                              3, 8, 4,
                              4, 8, 9,
                              //masa
                              11,10,6,
                              11,6, 7,
                              23,20,22,
                              23,21,20,
                              11,23,10,
                              23,22,10,
                              6 ,20,7,
                              20,21,7,
                              7, 21,11,
                              21,23,11,
                              6, 10,20,
                              20,10,22,
                              //masa ayak 
                              12,16,13,
                              13,16,17,
                              12,14,16,
                              14,18,16,
                              13,17,15,
                              15,17,19,
                              14,15,18,
                              15,19,18,

                              //kamera sandalye 1.ayak
                              24,28,25,
                              25,28,29,
                              24,26,28,
                              26,30,28,
                              25,29,27,
                              27,29,31,
                              26,27,30,
                              27,31,30,
                              //kamera sandalye 2.ayak
                              32,36,33,
                              33,36,37,
                              32,34,36,
                              34,38,36,
                              33,37,35,
                              35,37,39,
                              34,35,38,
                              35,39,38,
                              //kamera sandalye 3.ayak
                              40,44,41,
                              41,44,45,
                              40,42,44,
                              42,46,44,
                              41,45,43,
                              43,45,47,
                              42,43,46,
                              43,47,46,
                              //kamera sandalye 4.ayak
                              48,52,49,
                              49,52,53,
                              48,50,52,
                              50,54,52,
                              49,53,51,
                              51,53,55,
                              50,51,54,
                              51,55,54,
                              //kamera sandalye oturak
                              53,28,44,
                              53,37,28,
                              //kamera sandalye yaslanmak
                              57,37,56,
                              57,53,37,
                              57,56,37,
                              57,37,53,

                              //polis sandalye 1.ayak
                              58,62,59,
                              59,62,63,
                              58,60,62,
                              60,64,62,
                              59,63,61,
                              61,63,65,
                              60,61,64,
                              61,65,64,
                              //polis sandalye 2.ayak
                              66,70,67,
                              67,70,71,
                              66,68,70,
                              68,72,70,
                              67,71,69,
                              69,71,73,
                              68,69,72,
                              69,73,72,
                              //polis sandalye 3.ayak
                              74,78,75,
                              75,78,79,
                              74,76,78,
                              76,80,78,
                              75,79,77,
                              77,79,81,
                              76,77,80,
                              77,81,80,
                              //polis sandalye 4.ayak
                              82,86,83,
                              83,86,87,
                              82,84,86,
                              84,88,86,
                              83,87,85,
                              85,87,89,
                              84,85,88,
                              85,89,88,
                              //kamera sandalye oturak
                              87,62,78,
                              87,71,62,
                              //kamera sandalye yaslanmak
                              91,71,90,
                              91,87,71,
                              91,90,71,
                              91,71,87,
                              //cam
                              92,93,94,
                              95,94,93,
                              //lamba kablo 
                              /*96,98,97,
                              99,97,98,
                              //lamba 104 105 106 107
                              100,104,101,
                              101,104,105,
                              100,102,104,
                              102,106,104,
                              101,105,103,
                              103,105,107,
                              102,103,106,
                              103,107,106,
                              104,106,105,
                              107,105,106,*/
                              //kamera 1.ayak
                              108,112,109,
                              109,112,113,
                              108,110,112,
                              110,114,112,
                              109,113,111,
                              111,113,115,
                              110,111,114,
                              111,115,114,
                              //kamera 2.ayak
                              116,117,120,
                              117,121,120,
                              116,120,118,
                              118,120,122,
                              117,119,121,
                              119,123,121,
                              118,122,119,
                              119,122,123,
                              //ust bacak 1.
                              113,127,125,
                              113,114,127,
                              //ust bacak 2. 121 122 124 126
                              121,127,125,
                              121,125,122,
                              //omurga
                              124,128,125,
                              125,128,129,
                              124,126,128,
                              126,130,128,
                              125,129,127,
                              127,129,131,
                              126,127,130,
                              127,131,130,
                              // kamera sol kol
                              135,132,137,
                              133,132,135,
                              133,135,134,
                              135,136,134,
                              // kamera sað kol
                              141,143,138,
                              139,141,138,
                              139,140,141,
                              141,140,142,
                              /*//polis body 148 149 150 151
                              144,145,148,
                              145,149,148,
                              144,148,146,
                              146,148,150,
                              145,147,149,
                              147,151,149,
                              146,150,147,
                              147,150,151,
                              148,149,150,
                              150,149,151,
                              //polis head
                              152,153,156,
                              153,157,156,
                              152,156,154,
                              154,156,158,
                              153,155,157,
                              155,159,157,
                              154,158,155,
                              155,158,159,
                              156,157,158,
                              158,157,159,*/


                              
    };
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    unsigned int Indices2[]{
        //polis body 148 149 150 151
        144,145,148,
        145,149,148,
        144,148,146,
        146,148,150,
        145,147,149,
        147,151,149,
        146,150,147,
        147,150,151,
        148,149,150,
        150,149,151,
        //polis head
        152,153,156,
        153,157,156,
        152,156,154,
        154,156,158,
        153,155,157,
        155,159,157,
        154,158,155,
        155,158,159,
        156,157,158,
        158,157,159,
        //polis agiz 168 169 170 171
        168,170,169,
        169,170,171,
        //polis sol goz 
        160,162,161,
        161,162,163,
        //polis sag giz 164 165 166 167
        164,165,166,
        165,167,166,

    };




    glGenBuffers(2, &IBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(1);
    }

    const GLchar* p[1];
    p[0] = pShaderText;

    GLint Lengths[1];
    Lengths[0] = (GLint)strlen(pShaderText);

    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    std::string vs, fs;

    if (!ReadFile(pVSFileName, vs)) {
        exit(1);
    };

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    if (!ReadFile(pFSFileName, fs)) {
        exit(1);
    };

    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    if (gWVPLocation == -1) {
        printf("Error getting uniform location of 'gWVP'\n");
        exit(1);
    }

    gTranslationLocation = glGetUniformLocation(ShaderProgram, "gTranslation");
    if (gTranslationLocation == -1) {
        printf("Error getting uniform location of 'gTranslation'\n");
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);
}

int main(int argc, char** argv)
{
#ifdef _WIN64
    srand(GetCurrentProcessId());
#else
    srandom(getpid());
#endif

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    int x = 200;
    int y = 100;
    glutInitWindowPosition(x, y);
    int win = glutCreateWindow("Tutorial 15");
    printf("window id: %d\n", win);

    char game_mode_string[64];
    // Game mode string example: <Width>x<Height>@<BPP>
    snprintf(game_mode_string, sizeof(game_mode_string), "%dx%d@32", WINDOW_WIDTH, WINDOW_HEIGHT);
    glutGameModeString(game_mode_string);
    if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
        glutEnterGameMode();
    }
    else {
        fprintf(stderr, "Error: Requested game mode, '%s', not available.\n", game_mode_string);
    }

    InitializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    CreateVertexBuffer();
    CreateIndexBuffer();

    CompileShaders();
    
    
    start = std::clock();
    
    glutMainLoop();

    return 0;
}
