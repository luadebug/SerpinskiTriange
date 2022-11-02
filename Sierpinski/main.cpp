#include <GL/glut.h>
#include <imgui.h>
#include <imgui_impl_glut.h>
#include <imgui_impl_opengl2.h>
#include <Windows.h>


typedef GLfloat point2[2];                                      // define 2d point
typedef GLfloat point3[3];                                      // define 3d point
float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f, zoom = 1.0f;
point3 v3f[] = { {0.0f, 0.0f, 1.0f},                               // 3d tetrahedron end points
                 {0.0f, 0.942809f, -0.333333f},
                 {-0.816497f, -0.471405f, -0.333333f},
                 {0.816497f, -0.471405f, -0.333333f} };
GLfloat colors3f[4][3] = { {1.0f, 0.0f, 0.0f},                     // color indexes
                           {0.0f, 1.0f, 0.0f},
                           {0.0f, 0.0f, 1.0f},
                           {0.0f, 0.0f, 0.0f} };

int n = 3;                                                      // recursive depth
int h = 800;                                                    // window height
int w = 800;                                                // window width



void triangle3(point3 a, point3 b, point3 c);                   // draw 3d triangle
void divide_triangle3(point3 a, point3 b, point3 c, int m);     // find 3d middle points recursively

void Sierpinski3(int m);                                        // draw 3d Sierpinski triangle
void mydisplay();                                               // display callback funciton
void init();                                                    // initialize window environment



INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    
    glutInit(&__argc, __argv);                                     // initialize GLUT

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);   // set window configs

    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);

    w = desktop.right, h = desktop.bottom - 100;
    glutInitWindowSize(w, h);                                   // set window size
    glutCreateWindow(u8"Пирамида Серпинского");                             // name window
    glutDisplayFunc(mydisplay);                                 // set display callback

    //IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ImeWindowHandle = GetCurrentProcess();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    io.WantCaptureMouse = true;
    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    init();                                                     // initialize window

 

    glutMainLoop();                                             // start main event loop

    return 1;

}




void init()
{
    
    
    glEnable(GL_DEPTH_TEST);                                    // enable z-buffer algorithm for hidden-surface removal
    glClearColor(1.0, 1.0, 1.0, 1.0);                           // set background color to white
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    glViewport(0, 0, desktop.right, desktop.bottom);
    

    
    glMatrixMode(GL_PROJECTION);                                // set projection range 1, 2, 3, 4
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);


}

void my_display_code()
{
    ImGui::SetNextWindowSize(ImVec2(400, 400));
    if (ImGui::Begin(u8"Меню"))
    {
        if (ImGui::SliderInt(u8"Глубина", &n, 1, 7))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Центр X", &v3f[0][0], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Центр Y", &v3f[0][1], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Центр Z", &v3f[0][2], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }

        if (ImGui::SliderFloat(u8"Верхняя X", &v3f[1][0], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Верхняя Y", &v3f[1][1], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Верхняя Z", &v3f[1][2], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }

        if (ImGui::SliderFloat(u8"Левая X", &v3f[2][0], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Левая Y", &v3f[2][1], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Левая Z", &v3f[2][2], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Правая X", &v3f[3][0], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Правая Y", &v3f[3][1], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Правая Z", &v3f[3][2], -1.0f, 1.0f, "%.1f", 1.0f))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Поворот X", &angleX, 0.0f, 90.0f, "%.1f", 1.0f))
        {
            glRotatef(angleX, 1, 0, 0);
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Поворот Y", &angleY, 0.0f, 90.0f, "%.1f", 1.0f))
        {
            glRotatef(angleY, 0, 1, 0);
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Поворот Z", &angleZ, 0.0f, 90.0f, "%.1f", 1.0f))
        {
            glRotatef(angleZ, 0, 0, 1);
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::SliderFloat(u8"Масштаб", &zoom, 0.01f, 2.0f, "%.2f", 1.0f))
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-1.0 - zoom, 1.0 + zoom, -1.0 - zoom, 1.0 + zoom, -20.0, 20.0);
            glMatrixMode(GL_MODELVIEW);
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::ColorPicker3(u8"Цвет 1 грани", &colors3f[0][0]))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::ColorPicker3(u8"Цвет 2 грани", &colors3f[1][0]))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::ColorPicker3(u8"Цвет 3 грани", &colors3f[2][0]))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        if (ImGui::ColorPicker3(u8"Цвет 4 грани", &colors3f[3][0]))
        {
            glutSwapBuffers();
            glutPostRedisplay();
        }
        ImGui::End();
    }
}

void mydisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // clear color and depth buffer with previously set values
    int w, h;                                                   // get current window width and height
    
    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

 

    my_display_code();

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    io.ImeWindowHandle = GetCurrentProcess();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    io.WantCaptureMouse = true;
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glBegin(GL_TRIANGLES);                                      // begin draw triangle
    Sierpinski3(n);                                             // 3d Sierpinski triangle

    glEnd();                                                    // end draw
    glutSwapBuffers();
    glutPostRedisplay();
}


void triangle3(point3 a, point3 b, point3 c)
{
    glVertex3fv(a);
    glVertex3fv(b);
    glVertex3fv(c);
}

void divide_triangle3(point3 a, point3 b, point3 c, int m)
{
    point3 v0{}, v1{}, v2{};
    if (m > 0) {                                                // expected depth not achieved
        for (char j = char(0); j < char(3); ++j)                             // update three end points of the triangle
        {
            v0[j] = (a[j] + b[j]) / 2.f;
            v1[j] = (a[j] + c[j]) / 2.f;
            v2[j] = (b[j] + c[j]) / 2.f;
        }
        divide_triangle3(a, v0, v1, m - 1);                     // divide three new triangles respectively
        divide_triangle3(b, v0, v2, m - 1);
        divide_triangle3(c, v1, v2, m - 1);
    }
    else {                                                      // expected depth achieved, draw triangle
        triangle3(a, b, c);
    }
}

void Sierpinski3(int m)
{
    glColor3fv(colors3f[0]);                                    // set 3d triangle color
    divide_triangle3(v3f[0], v3f[1], v3f[2], m);                // divide and draw triangles recursively
    glColor3fv(colors3f[1]);
    divide_triangle3(v3f[1], v3f[2], v3f[3], m);
    glColor3fv(colors3f[2]);
    divide_triangle3(v3f[0], v3f[2], v3f[3], m);
    glColor3fv(colors3f[3]);
    divide_triangle3(v3f[0], v3f[1], v3f[3], m);
}
