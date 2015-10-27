#include "glview.h"

#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

GLView::GLView() : QOpenGLWidget()
{
//    QSurfaceFormat glFormat;
//    glFormat.setVersion( 3, 3 );
//    glFormat.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
//    glFormat.setSwapBehavior(QSurfaceFormat::SwapBehavior::SingleBuffer);
//    glFormat.setSwapInterval(0);

//    setFormat(glFormat);
}

void GLView::paintGL()
{
    _checkErrors("before draw");
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, _textureBuffer);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    {
       glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
       glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
       glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
       glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    _checkErrors("after draw");
}

void GLView::initializeGL()
{
    // Initialize OpenGL Functions
    initializeOpenGLFunctions();

    glViewport(0, 0, width(), height());

    _checkErrors("Before initializeGL");
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    glGenTextures(1, &_textureBuffer);


    // In windows, here width and height
    if(_textureBuffer)
    {
        int w = 640;
        int h = 480;

        // Chess dummy texture
        int totalBytes = w*h*3;
        unsigned char * dummyTexture = new unsigned char[totalBytes];


        for(int i = 0 ; i < h; i++)
        {
            for(int j = 0 ; j < w; j++)
            {
                int tilex = i / 40 ;
                int tiley = j / 40 ;

                int index = i* (w*3) + (j*3);

                if((tilex % 2 == 0 && tiley % 2 == 0) || (tilex % 2 != 0 && tiley % 2 !=0))
                {
                    dummyTexture[index +0] = 128;
                    dummyTexture[index +1] = 128;
                    dummyTexture[index +2] = 128;
                }
                else
                {
                    dummyTexture[index +0] = 0;
                    dummyTexture[index +1] = 0;
                    dummyTexture[index +2] = 128;
                }
            }
        }

        glBindTexture(GL_TEXTURE_2D, _textureBuffer);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, dummyTexture);
        delete [] dummyTexture;
    }
    else
    {
        std::cout << "Error loading texture " << std::endl;
    }
    _checkErrors("After initializeGL");
}

void GLView::_checkErrors(const std::string & snippet = "")
{
    for(GLenum currError = glGetError(); currError != GL_NO_ERROR; currError = glGetError())
    {
        std::cout << "Error '" << snippet << "' " <<  currError << std::endl;
    }
}
