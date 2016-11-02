/*
 * Screensavers from Shadertoy for Kodi
 * Copyright (c) 2015 Team Kodi
 *
 * Ver 1.0 12 May 2015	Anton Fedchin (afedchin)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "xbmc_scr_dll.h"
#if defined(HAS_GLES)
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <assert.h>
#define TO_STRING(...) #__VA_ARGS__
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <streambuf>
#include <ctime>
#include <p8-platform/util/timeutils.h>
#include <math.h>
#include <complex.h>
#include <limits.h>
#include <fstream>
#include <sstream>
#include "lodepng.h"


std::string g_pathPresets;

struct Preset {
  std::string name;
  std::string file;
  int channel[4];
};

const std::vector<Preset> g_presets =
{
  { "Ball",                     "ball.frag.glsl",              99, -1, -1, -1 },
  { "Bleepy Blocks",            "bleepyblocks.frag.glsl",      99, -1, -1, -1 },
  { "Bob Scanline",             "blobandscsanline.glsl",       -1, -1, -1, -1 },
  { "Blobs",                    "blobs.frag.glsl",             -1, -1, -1, -1 },
  { "Blue Fire",                "bluefire.frag.glsl",          -1, -1, -1, -1 },
  { "Classic RGB Plasma",       "rgbplasma.frag.glsl",         -1, -1, -1, -1 },
  { "Dance Floor",              "dancefloor.frag.glsl",        -1, -1, -1, -1 },
  { "Effect Ringblobs",         "ringblobs.frag.glsl",         -1, -1, -1, -1 },
  //{ "Ether",                    "ether.frag.glsl",             -1, -1, -1, -1 },
  { "Fire",                     "fire.frag.glsl",              -1, -1, -1, -1 },
  { "Flaring 1",                "flaringtype1.frag.glsl",      -1, -1, -1, -1 },
  { "Flaring 2",                "flaringtype2.frag.glsl",      -1, -1, -1, -1 },
  { "Flaring 3",                "flaringtype3.frag.glsl",      -1, -1, -1, -1 },
  { "Flaring 4",                "flaringtype4.frag.glsl",      -1, -1, -1, -1 },
  { "Flaring 5",                "flaringtype5.frag.glsl",      -1, -1, -1, -1 },
  { "Interstellar",             "stellar.frag.glsl",           -1, -1, -1, -1 },
  { "Interweaving Sine bands",  "sinebands.frag.glsl",         -1, -1, -1, -1 },
  { "Julia Trap",               "juliatrap.frag.glsl",         -1, -1, -1, -1 },
  { "Layers Of Plasma",         "layersplasma.frag.glsl",      -1, -1, -1, -1 },
  { "Liquid Spectrum",          "liquidspectrum.frag.glsl",    -1, -1, -1, -1 },
  { "Noise Animation Electric", "noiseanimelectric.frag.glsl", -1, -1, -1, -1 },
  { "Noise Animation Flow",     "noiseanimflow.frag.glsl",     -1, -1, -1, -1 },
  { "Noise Animation Lava",     "noiseanimlava.frag.glsl",     -1, -1, -1, -1 },
  { "Overly Satisfying",        "overlysatisfying.frag.glsl",  -1, -1, -1, -1 },
  { "Plasma Circles",           "plasmacircles.frag.glsl",     -1, -1, -1, -1 },
  { "Plasma Triangles",         "plasmatriangle.frag.glsl",    -1, -1, -1, -1 },
  { "Plas.mAtrix. Spin",        "plasmatrixspin.frag.glsl",    -1, -1, -1, -1 },
  { "Pixellated Plasma",        "pixellated.frag.glsl",        -1, -1, -1, -1 },
  { "Silexarst Creation",       "silexarst.frag.glsl",         -1, -1, -1, -1 },
  { "Sky at Night",             "skyatnight.frag.glsl",        -1, -1, -1, -1 },
  { "Sphere",                   "sphere.frag.glsl",            99, -1, -1, -1 },
  { "Star Tunnel",              "startunnel.frag.glsl",        -1, -1, -1, -1 },
  { "Vector Field",             "vectorfield.frag.glsl",       -1, -1, -1, -1 },
  { "Venus",                    "venus.frag.glsl",             -1, -1, -1, -1 },
  { "Warp",                     "warp.frag.glsl",              99, -1, -1, -1 },
  { "Watch It Burn",            "watchitburn.frag.glsl",       -1, -1, -1, -1 },
  { "Water Caustic",            "watercaustic.frag.glsl",      -1, -1, -1, -1 },
  { "Worley Noise Waters",      "worleynoisewaters.frag.glsl"  -1, -1, -1, -1 },
  //"balloffire.frag.glsl",
  //"noiseanimwatery.frag.glsl",
  //"tweets.frag.glsl",
  //"wiggly.frag.glsl",
};

const char *g_fileTextures[] = {
  "tex00.png",
  "tex01.png",
  "tex02.png",
  "tex03.png",
  "tex04.png",
  "tex05.png",
  "tex06.png",
  "tex07.png",
  "tex08.png",
  "tex09.png",
  "tex10.png",
  "tex11.png",
  "tex12.png",
  "tex15.png",
  "tex16.png",
  "tex14.png",
};

#if defined(HAS_GLES)
struct
{
  GLuint vertex_buffer;
  GLuint attr_vertex_e;
  GLuint attr_vertex_r, uTexture;
  GLuint effect_fb;
  GLuint framebuffer_texture;
  GLuint render_program;
  GLuint uScale;
  int fbwidth, fbheight;
} state_g, *state = &state_g;
#endif

bool needsUpload = true;
int g_numberTextures = 17;
GLint g_textures[17] = { };

int                       g_numberPresets = g_presets.size();
int                       g_currentPreset = 0;

bool initialized = false;

GLuint shadertoy_shader = 0;

GLint iResolutionLoc        = 0;
GLint iGlobalTimeLoc        = 0;
GLint iChannelTimeLoc       = 0;
GLint iMouseLoc             = 0;
GLint iDateLoc              = 0;
GLint iSampleRateLoc        = 0;
GLint iChannelResolutionLoc = 0;
GLint iChannelLoc[4];
GLuint iChannel[4];

int width = 0;
int height = 0;

int64_t initial_time;
int bits_precision = 0;

GLuint createTexture(GLint format, unsigned int w, unsigned int h, const GLvoid * data) {
  GLuint texture = 0;
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
  return texture;
}

GLuint createTexture(const GLvoid *data, GLint format, unsigned int w, unsigned int h, GLint internalFormat, GLint scaling, GLint repeat) {
  GLuint texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaling);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaling);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);

#if defined(HAS_GLES)
  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
#else
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, data);
#endif
  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

GLuint createTexture(const char *file, GLint internalFormat, GLint scaling, GLint repeat) {
  std::ostringstream ss;
  ss << g_pathPresets << "/resources/" << file;
  std::string fullPath = ss.str();

  unsigned error;
  unsigned char* image;
  unsigned width, height;

  error = lodepng_decode32_file(&image, &width, &height, fullPath.c_str());
  if (error) {
    printf("error %u: %s\n", error, lodepng_error_text(error));
    return 0;
  }

  GLuint texture = createTexture(image, GL_RGBA, width, height, internalFormat, scaling, repeat);
  free(image);
  return texture;
}

GLuint compileShader(GLenum shaderType, const char *shader) {
  GLuint s = glCreateShader(shaderType);
  if (s == 0) {
    std::cerr << "Failed to create shader from\n====" << std::endl;
    std::cerr << shader << std::endl;
    std::cerr << "===" << std::endl;

    return 0;
  }

  glShaderSource(s, 1, &shader, NULL);
  glCompileShader(s);

  GLint param;
  glGetShaderiv(s, GL_COMPILE_STATUS, &param);
  if (param != GL_TRUE) {
    std::cerr << "Failed to compile shader source\n====" << std::endl;
    std::cerr << shader << std::endl;
    std::cerr << "===" << std::endl;

    int infologLength = 0;
    char *infoLog;

    glGetShaderiv(s, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0) {
      infoLog = new char[infologLength];
      glGetShaderInfoLog(s, infologLength, NULL, infoLog);
      std::cerr << "<log>" << std::endl << infoLog << std::endl << "</log>" << std::endl;
      delete [] infoLog;
    }

    glDeleteShader(s);

    return 0;
  }

  return s;
}

GLuint compileAndLinkProgram(const char *vertexShader, const char *fragmentShader) {
  GLuint program = glCreateProgram();
  if (program == 0) {
    std::cerr << "Failed to create program" << std::endl;
    return 0;
  }

  GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  if (vs && fs) {
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint param;
    glGetProgramiv(program, GL_LINK_STATUS, &param);
    if (param != GL_TRUE) {
      std::cerr << "Failed to link shader program " << std::endl;
      glGetError();
      int infologLength = 0;
      char *infoLog;

      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);

      if (infologLength > 0) {
        infoLog = new char[infologLength];
        glGetProgramInfoLog(program, infologLength, NULL, infoLog);
        std::cerr << "<log>" << std::endl << infoLog << std::endl << "</log>" << std::endl;
        delete [] infoLog;
      }

      GLchar errorLog[1024] = {0};
      glGetProgramInfoLog(program, 1024, NULL, errorLog);

      std::cerr << "<vertexShader>" << std::endl << vertexShader << std::endl << "</vertexShader>" << std::endl;
      std::cerr << "<fragmentShader>" << std::endl << fragmentShader << std::endl << "</fragmentShader>" << std::endl;

      glDetachShader(program, vs);
      glDeleteShader(vs);

      glDetachShader(program, fs);
      glDeleteShader(fs);

      glDeleteProgram(program);
      return 0;
    }
  } else {
  	glDeleteProgram(program);
  }

  glUseProgram(0);

  if (vs)
    glDeleteShader(vs);

  if (fs)
    glDeleteShader(fs);

  return program;
}


#if defined(HAS_GLES)

std::string vsSource = TO_STRING(
         precision mediump float;
         precision mediump int;
         attribute vec4 vertex;
         varying vec2 vTextureCoord;
         uniform vec2 uScale;
         void main(void)
         {
            gl_Position = vertex;
            vTextureCoord = vertex.xy*0.5+0.5;
            vTextureCoord.x = vTextureCoord.x * uScale.x;
            vTextureCoord.y = vTextureCoord.y * uScale.y;
         }
  );

std::string render_vsSource = TO_STRING(
         precision mediump float;
         precision mediump int;
         attribute vec4 vertex;
         varying vec2 vTextureCoord;
         void main(void)
         {
            gl_Position = vertex;
            vTextureCoord = vertex.xy*0.5+0.5;
         }
  );

std::string render_fsSource = TO_STRING(
         precision mediump float;
         precision mediump int;
         varying vec2 vTextureCoord;
         uniform sampler2D uTexture;
         void main(void)
         {
            gl_FragColor = texture2D(uTexture, vTextureCoord);
         }
  );
#else
std::string vsSource = "void main() { gl_Position = ftransform(); }";
#endif

std::string fsHeader =
"#extension GL_OES_standard_derivatives : enable\n"
#ifdef HAS_GLES
"precision mediump float;\n"
"precision mediump int;\n"
#endif
"uniform vec3      iResolution;\n"
"uniform float     iGlobalTime;\n"
"uniform float     iChannelTime[4];\n"
"uniform vec4      iMouse;\n"
"uniform vec4      iDate;\n"
"uniform float     iSampleRate;\n"
"uniform vec3      iChannelResolution[4];\n"
"uniform sampler2D iChannel0;\n"
"uniform sampler2D iChannel1;\n"
"uniform sampler2D iChannel2;\n"
"uniform sampler2D iChannel3;\n";

std::string fsFooter =
"void main(void)\n"
"{\n"
"  vec4 color = vec4(0.0, 0.0, 0.0, 1.0);\n"
"  mainImage(color, gl_FragCoord.xy);\n"
"  color.w = 1.0;\n"
"  gl_FragColor = color;\n"
"}\n";

void unloadTextures() {
  for (int i=0; i<4; i++) {
    if (iChannel[i]) {
      glDeleteTextures(1, &iChannel[i]);
      iChannel[i] = 0;
    }
  }
}

void unloadPreset() {
  if (shadertoy_shader) {
    glDeleteProgram(shadertoy_shader);
    shadertoy_shader = 0;
  }
#if defined(HAS_GLES)
  if (state->framebuffer_texture)
  {
    glDeleteTextures(1, &state->framebuffer_texture);
    state->framebuffer_texture = 0;
  }
  if (state->effect_fb)
  {
    glDeleteFramebuffers(1, &state->effect_fb);
    state->effect_fb = 0;
  }
  if (state->render_program) {
    glDeleteProgram(state->render_program);
    state->render_program = 0;
  }
#endif
}

std::string createShader(const std::string &file)
{
  std::ostringstream ss;
  ss << g_pathPresets << "/resources/" << file;
  std::string fullPath = ss.str();

  std::ifstream t(fullPath);
  std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

  //std::string str = "void mainImage( out vec4 fragColor, in vec2 fragCoord ) { fragColor = texture2D(iChannel1, fragCoord.xy / iResolution.xy); }";

  std::string fsSource = fsHeader + "\n" + str + "\n" + fsFooter;
  return fsSource;
}

GLint loadTexture(int number)
{
  if (number >= 0 && number < g_numberTextures) {
    GLint format = GL_RGBA;
    GLint scaling = GL_LINEAR;
    GLint repeat = GL_REPEAT;
    return createTexture(g_fileTextures[number], format, scaling, repeat);
  }
  else if (number == 99) { // framebuffer
    static std::vector<GLubyte> tmp(1024, 127);
    return createTexture(GL_LUMINANCE, 512, 2, tmp.data());
  }
  return 0;
}

void loadPreset(int preset, std::string vsSource, std::string fsSource)
{
  unloadPreset();
  shadertoy_shader = compileAndLinkProgram(vsSource.c_str(), fsSource.c_str());

  iResolutionLoc        = glGetUniformLocation(shadertoy_shader, "iResolution");
  iGlobalTimeLoc        = glGetUniformLocation(shadertoy_shader, "iGlobalTime");
  iChannelTimeLoc       = glGetUniformLocation(shadertoy_shader, "iChannelTime");
  iMouseLoc             = glGetUniformLocation(shadertoy_shader, "iMouse");
  iDateLoc              = glGetUniformLocation(shadertoy_shader, "iDate");
  iSampleRateLoc        = glGetUniformLocation(shadertoy_shader, "iSampleRate");
  iChannelResolutionLoc = glGetUniformLocation(shadertoy_shader, "iChannelResolution");
  iChannelLoc[0]        = glGetUniformLocation(shadertoy_shader, "iChannel0");
  iChannelLoc[1]        = glGetUniformLocation(shadertoy_shader, "iChannel1");
  iChannelLoc[2]        = glGetUniformLocation(shadertoy_shader, "iChannel2");
  iChannelLoc[3]        = glGetUniformLocation(shadertoy_shader, "iChannel3");

#if defined(HAS_GLES)
  state->uScale         = glGetUniformLocation(shadertoy_shader, "uScale");
  state->attr_vertex_e  = glGetAttribLocation(shadertoy_shader,  "vertex");

  state->render_program = compileAndLinkProgram(render_vsSource.c_str(), render_fsSource.c_str());
  state->uTexture       = glGetUniformLocation(state->render_program, "uTexture");
  state->attr_vertex_r  = glGetAttribLocation(state->render_program,  "vertex");
  // Prepare a texture to render to
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &state->framebuffer_texture);
  glBindTexture(GL_TEXTURE_2D, state->framebuffer_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state->fbwidth, state->fbheight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Prepare a framebuffer for rendering
  glGenFramebuffers(1, &state->effect_fb);
  glBindFramebuffer(GL_FRAMEBUFFER, state->effect_fb);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, state->framebuffer_texture, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
  initial_time = P8PLATFORM::GetTimeMs();
}

static uint64_t GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}

static void RenderTo(GLuint shader, GLuint effect_fb)
{
  glUseProgram(shader);

#if !defined(HAS_GLES)
  glDisable(GL_BLEND);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glClear(GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
#endif

  if (shader == shadertoy_shader) {
    GLuint w = width, h = height;
#if defined(HAS_GLES)
    if (state->fbwidth && state->fbheight)
      w = state->fbwidth, h = state->fbheight;
#endif
    int64_t intt = P8PLATFORM::GetTimeMs() - initial_time;
    if (bits_precision)
      intt &= (1<<bits_precision)-1;

    if (needsUpload) {
      for (int i=0; i<4; i++) {
        if (g_presets[g_currentPreset].channel[i] == 99) {
          glActiveTexture(GL_TEXTURE0 + i);
          glBindTexture(GL_TEXTURE_2D, iChannel[i]);
          static std::vector<GLubyte> tmp(16384, 127);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 512, 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tmp.data());
        }
      }
      needsUpload = false;
    }

    float t = intt / 1000.0f;
    GLfloat tv[] = { t, t, t, t };

    glUniform3f(iResolutionLoc, w, h, 0.0f);
    glUniform1f(iGlobalTimeLoc, t);
    glUniform1f(iSampleRateLoc, 0);
    glUniform1fv(iChannelTimeLoc, 4, tv);
#if defined(HAS_GLES)
    glUniform2f(state->uScale, (GLfloat)width/state->fbwidth, (GLfloat)height/state->fbheight);
#endif
    time_t now = time(NULL);
    tm *ltm = localtime(&now);

    float year = 1900 + ltm->tm_year;
    float month = ltm->tm_mon;
    float day = ltm->tm_mday;
    float sec = (ltm->tm_hour * 60 * 60) + (ltm->tm_min * 60) + ltm->tm_sec;

    glUniform4f(iDateLoc, year, month, day, sec);

    for (int i=0; i<4; i++) {
      glActiveTexture(GL_TEXTURE0 + i);
#if !defined(HAS_GLES)
      glEnable(GL_TEXTURE_2D);
#endif
      glUniform1i(iChannelLoc[i], i);
      glBindTexture(GL_TEXTURE_2D, iChannel[i]);
    }
  } else {
#if defined(HAS_GLES)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state->framebuffer_texture);
    glUniform1i(state->uTexture, 0); // first currently bound texture "GL_TEXTURE0"
#endif
  }

#if defined(HAS_GLES)
  // Draw the effect to a texture or direct to framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, effect_fb);

  GLuint attr_vertex = shader == shadertoy_shader ? state->attr_vertex_e : state->attr_vertex_r;
  glBindBuffer(GL_ARRAY_BUFFER, state->vertex_buffer);
  glVertexAttribPointer(attr_vertex, 4, GL_FLOAT, 0, 16, 0);
  glEnableVertexAttribArray(attr_vertex);
  glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
  glDisableVertexAttribArray(attr_vertex);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
  glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f( 1.0f, 1.0f, 0.0f);
    glVertex3f( 1.0f,-1.0f, 0.0f);
    glVertex3f(-1.0f,-1.0f, 0.0f);
  glEnd();
#endif

  for (int i=0; i<4; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
#if !defined(HAS_GLES)
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
#endif
  glUseProgram(0);
}

////////////////////////////////////////////////////////////////////////////
// Kodi has loaded us into memory, we should set our core values
// here and load any settings we may have from our config file
//
extern "C" ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!props)
    return ADDON_STATUS_UNKNOWN;

  SCR_PROPS* scrprops = (SCR_PROPS*)props;

  width = scrprops->width;
  height = scrprops->height;
  g_pathPresets = scrprops->presets;

  if (!initialized)
  {
#if defined(HAS_GLES)
    static const GLfloat vertex_data[] = {
        -1.0,1.0,1.0,1.0,
        1.0,1.0,1.0,1.0,
        1.0,-1.0,1.0,1.0,
        -1.0,-1.0,1.0,1.0,
    };
    glGetError();
    // Upload vertex data to a buffer
    glGenBuffers(1, &state->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, state->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
#endif
    initialized = true;
  }

  if (!props)
    return ADDON_STATUS_UNKNOWN;

  return ADDON_STATUS_NEED_SAVEDSETTINGS;
}

#ifdef HAS_GLES
static int determine_bits_precision()
{
  std::string vsPrecisionSource = TO_STRING(
	void mainImage( out vec4 fragColor, in vec2 fragCoord )
	{
	    float y = ( fragCoord.y / iResolution.y ) * 26.0;
	    float x = 1.0 - ( fragCoord.x / iResolution.x );
	    float b = fract( pow( 2.0, floor(y) ) + x );
	    if (fract(y) >= 0.9)
		b = 0.0;
	    fragColor = vec4(b, b, b, 1.0 );
	}
  );
  std::string fsPrecisionSource = fsHeader + "\n" + vsPrecisionSource + "\n" + fsFooter;

  state->fbwidth = 32, state->fbheight = 26*10;
  loadPreset(0, vsSource, fsPrecisionSource);
  RenderTo(shadertoy_shader, state->effect_fb);
  glFinish();

  unsigned char *buffer = new unsigned char[state->fbwidth * state->fbheight * 4];
  if (buffer)
    glReadPixels(0, 0, state->fbwidth, state->fbheight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
  #if 0
  for (int j=0; j<state->fbheight; j++) {
    for (int i=0; i<state->fbwidth; i++) {
      printf("%02x ", buffer[4*(j*state->fbwidth+i)]);
    }
    printf("\n");
  }
  #endif
  int bits = 0;
  unsigned char b = 0; 
  for (int j=0; j<state->fbheight; j++) {
    unsigned char c = buffer[4*(j*state->fbwidth+(state->fbwidth>>1))];
    if (c && !b)
      bits++;
    b = c;
  }
  delete buffer;
  unloadPreset();
  return bits;
}
#endif

extern "C" void Start()
{
  int preset = g_currentPreset < 0 ? (rand() % g_numberPresets) : g_currentPreset;
#ifdef HAS_GLES
  bits_precision = determine_bits_precision();
  // mali-400 has only 10 bits which means milliseond timer wraps after ~1 second.
  // we'll fudge that up a bit as having a larger range is more important than ms accuracy
  bits_precision = max(bits_precision, 13);
  printf("bits=%d\n", bits_precision);
#endif
  
  unloadTextures();
  for (int i=0; i<4; i++) {
    if (g_presets[preset].channel[i] >= 0)
      iChannel[i] = loadTexture(g_presets[preset].channel[i]);
  }

#ifdef HAS_GLES
  const int size1 = 256, size2=512;
  double t1 = measure_performance(preset, size1);
  double t2 = measure_performance(preset, size2);
 
  double expected_fps = 40.0;
  // time per pixel for rendering fragment shader
  double B = (t2-t1)/(size2*size2-size1*size1);
  // time to render to screen
  double A = t2 - size2*size2 * B;
  // how many pixels get the desired fps
  double pixels = (1000.0/expected_fps - A) / B;
  state->fbwidth = sqrtf(pixels * width / height);
  if (state->fbwidth * 4 >= width * 3)
    state->fbwidth = 0;
  else if (state->fbwidth < 320)
    state->fbwidth = 320;
  state->fbheight = state->fbwidth * height / width;

  printf("expected fps=%f, pixels=%f %dx%d (A:%f B:%f t1:%.1f t2:%.1f)\n", expected_fps, pixels, state->fbwidth, state->fbheight, A, B, t1, t2);
#endif

  std::string fsSource = createShader(g_presets[preset].file);
  loadPreset(preset, vsSource, fsSource);
}

extern "C" void Render()
{
  glGetError();
  if (initialized) {
#if defined(HAS_GLES)
    if (state->fbwidth && state->fbheight) {
      RenderTo(shadertoy_shader, state->effect_fb);
      RenderTo(state->render_program, 0);
    } else {
      RenderTo(shadertoy_shader, 0);
    }
#else
    RenderTo(shadertoy_shader, 0);
#endif
    static int frames = 0;
    static uint64_t ts;
    if (frames == 0) {
      ts = GetTimeStamp();
    }
    frames++;
    uint64_t ts2 = GetTimeStamp();
    if (ts2 - ts > 1e6)
    {
     printf("%d fps\n", frames);
     ts += 1e6;
     frames = 0;
    }
  }
}

extern "C" void Stop()
{
}

// Kodi tells us to stop the screensaver
// we should free any memory and release
// any resources we have created.
extern "C" void ADDON_Stop()
{
  unloadPreset();
  unloadTextures();
}

void ADDON_Destroy()
{
#if defined(HAS_GLES)
  glDeleteBuffers(1, &state->vertex_buffer);
#endif
}

ADDON_STATUS ADDON_GetStatus()
{
  return ADDON_STATUS_OK;
}

bool ADDON_HasSettings()
{
  return true;
}

unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
  return 0;
}

ADDON_STATUS ADDON_SetSetting(const char *strSetting, const void *value)
{
  if (!strSetting || !value)
    return ADDON_STATUS_UNKNOWN;

  if (strcmp(strSetting, "preset") == 0)
  {
    int c = atoi((char*)value);
    if (c < 0) c = 0;
    if (c > g_presets.size()) c = g_presets.size();
    g_currentPreset = c - 1;
    return ADDON_STATUS_OK;
  }

  return ADDON_STATUS_UNKNOWN;
}

void ADDON_FreeSettings()
{
}

void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data)
{
}

void GetInfo(SCR_INFO *info)
{
}

