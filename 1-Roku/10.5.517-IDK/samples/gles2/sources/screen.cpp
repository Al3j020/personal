// Copyright 2021 Roku, Inc.  All rights reserved.
#include "screen.h"
#include "spng.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <map>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void
CheckGLErrorInternal(const char* file, int line)
{
  switch (glGetError()) {
    case GL_NO_ERROR:
      break;
    case GL_INVALID_ENUM:
      fprintf(stderr, "GL_INVALID_ENUM at: %s:%d\n", file, line);
      break;
    case GL_INVALID_VALUE:
      fprintf(stderr, "GL_INVALID_VALUE at: %s:%d\n", file, line);
      break;
    case GL_INVALID_OPERATION:
      fprintf(stderr, "GL_INVALID_OPERATION at: %s:%d\n", file, line);
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      fprintf(
        stderr, "GL_INVALID_FRAMEBUFFER_OPERATION at: %s:%d\n", file, line);
      break;
    case GL_OUT_OF_MEMORY:
      fprintf(stderr, "GL_OUT_OF_MEMORY at: %s:%d\n", file, line);
      break;
  }
}

namespace {
struct MMapFile
{
  void* pMemory;
  size_t zMemory;
  int fd;

  MMapFile(const char* path);
  ~MMapFile();
};

MMapFile::MMapFile(const char* path)
  : pMemory(nullptr)
  , zMemory(0)
  , fd(-1)
{
  fd = open(path, O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH);
  if (fd != -1) {
    lseek(fd, 0, SEEK_SET);
    auto length = lseek(fd, 0, SEEK_END);
    pMemory = mmap(0, length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (pMemory == MAP_FAILED) {
      fprintf(stderr, "mmap(%s) failed: %s\n", path, strerror(errno));
      pMemory = nullptr;
      close(fd);
    } else {
      zMemory = length; // mmap() successful.
    }
  } else {
    fprintf(stderr, "open(%s) failed: %s\n", path, strerror(errno));
  }
}

MMapFile::~MMapFile()
{
  if (pMemory) {
    if (munmap(pMemory, zMemory) == -1) {
      fprintf(stderr, "munmap() failed\n");
    }
  }

  if (fd != -1 && close(fd) == -1) {
    fprintf(stderr, "close() failed: %s\n", strerror(errno));
  }
}

struct ScreenImp : public Screen
{
  std::future<void> swap() override;
  GLuint getPNGTexture(const std::string& file) override;
  GLuint createTexture(int width,
                       int height,
                       std::shared_ptr<uint8_t> pixels) override;

  GLuint compileShader(GLenum type, const char* file);
  void loadShaderProgram();

  ScreenImp(int, int);
  virtual ~ScreenImp();

private:
  EGLConfig pContext;
  EGLDisplay pDisplay;
  EGLSurface pSurface;

  ctpl::thread_pool queueAccess;
  std::map<std::string, GLuint> mapTextures;
};

ScreenImp::ScreenImp(int w, int h)
  : Screen(w, h)
  , queueAccess(1)
{
  auto setup = push([=](int) {
    EGLint major, minor, nconfigs;
    pDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (pDisplay == EGL_NO_DISPLAY) {
      fprintf(stderr, "unable to create display\n");
    } else {
      eglInitialize(pDisplay, &major, &minor);

      EGLint attributes[] = { EGL_RENDERABLE_TYPE,
                              EGL_OPENGL_ES2_BIT,
                              EGL_BUFFER_SIZE,
                              32,
                              EGL_ALPHA_SIZE,
                              8,
                              EGL_RED_SIZE,
                              8,
                              EGL_GREEN_SIZE,
                              8,
                              EGL_BLUE_SIZE,
                              8,
                              EGL_DEPTH_SIZE,
                              16,
                              EGL_STENCIL_SIZE,
                              8,
                              EGL_NONE };
      EGLConfig pConfig;
      eglChooseConfig(pDisplay, attributes, &pConfig, 1, &nconfigs);

      EGLint ctx[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
      pContext = eglCreateContext(pDisplay, pConfig, nullptr, ctx);

      if (pContext == EGL_NO_CONTEXT) {
        fprintf(stderr, "unable to create context\n");
      } else {
        // The following data structures are used for creating framebuffer
        // surfaces backing the display, created by eglCreateWindowSurface().
        enum RPFormat
        {
          RGBA8888 = 0, // 32 bit RGBA, 8 bits each component
          RGB888,       // 24 bit RGB, 8 bits each component, packed into 32 bit
          RGB565,       // 16 bit RGB
          RGBA5551,     // 16 bit RGBA, 5 bit color components, 1 bit alpha
          RGBA4444,     // 16 bit RGBA, 4 bit components
          ALPHA8,       // 8 bit alpha surface, fixed color
          ETC1RGB8,     // ETC1 compressed image
          UYVY,         // 16 bit YUV 4:2:2
          YUY2,         // 16 bit YUV 4:2:2 YCbYCr
          UNKNOWN       // should be last
        };

        struct ScreenInfo
        {
          int32_t width;        // width of framebuffer
          int32_t height;       // height of framebuffer
          int8_t layer;         // always 0
          int8_t buffers;       // 2 for double buffering, 1 for single buffer
          enum RPFormat format; // defined above
          bool preserved;       // always false.
        };

        ScreenInfo window = { w, h, 0, 2, RPFormat::RGBA8888, false };

        pSurface = eglCreateWindowSurface(
          pDisplay, pConfig, (NativeWindowType)&window, nullptr);
        if (pSurface == EGL_NO_SURFACE) {
          fprintf(stderr, "unable to create surface\n");
        } else {
          eglMakeCurrent(pDisplay, pSurface, pSurface, pContext);
          glClearDepthf(1.0f);
          glDepthFunc(GL_ALWAYS);
          glDisable(GL_DEPTH_TEST);
          glEnable(GL_CULL_FACE);
          glCullFace(GL_BACK);
          glFrontFace(GL_CCW);
          glEnable(GL_BLEND);
          glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
          glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          eglSwapBuffers(pDisplay, pSurface);
          glClear(GL_COLOR_BUFFER_BIT);
          eglSwapBuffers(pDisplay, pSurface);
          CheckGLError();
          fprintf(stderr, "GLES2 context ready\n");

          loadShaderProgram();
        }
      }
    }
    fflush(stderr);
  });
  setup.wait();
}

ScreenImp::~ScreenImp()
{
  queueAccess.stop();

  auto shutdown = push([this](int) {
    glDeleteProgram(shader.id);

    eglMakeCurrent(pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (pSurface != EGL_NO_SURFACE) {
      eglDestroySurface(pDisplay, pSurface);
    }
    if (pContext != EGL_NO_CONTEXT) {
      eglDestroyContext(pDisplay, pContext);
    }
    if (pDisplay != EGL_NO_DISPLAY) {
      eglTerminate(pDisplay);
    }
  });

  shutdown.wait();
  stop();
  fprintf(stderr, "GLES2 context released\n");
  fflush(stderr);
}

GLuint
ScreenImp::compileShader(GLenum type, const char* path)
{
  GLuint id = 0;
  MMapFile buffer(path);
  if (buffer.pMemory) {
    GLchar* code[1] = { (GLchar*)buffer.pMemory };
    GLint code_z[1] = { (GLint)buffer.zMemory };
    GLint ok = GL_FALSE;

    id = glCreateShader(type); // deleted in loadShaderProgram().
    glShaderSource(id, 1, code, code_z);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
      GLint log_z = 0;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_z);
      if (auto error_buf = malloc(log_z)) {
        glGetShaderInfoLog(id, log_z, &log_z, (GLchar*)error_buf);
        fprintf(stderr, "Error compiling shader:\n%s\n", (char*)error_buf);
        free(error_buf);
      }
    }
  }
  return id;
}

void
ScreenImp::loadShaderProgram()
{
  // This function is called from thread running GL context.
  GLuint idShaderVertex =
    compileShader(GL_VERTEX_SHADER, "/pkg:/source/vertex.glsl");
  GLuint idShaderPixel =
    compileShader(GL_FRAGMENT_SHADER, "/pkg:/source/pixel.glsl");

  if (idShaderVertex == 0 || idShaderPixel == 0) {
    if (idShaderVertex != 0) {
      glDeleteShader(idShaderVertex);
    }
    if (idShaderPixel != 0) {
      glDeleteShader(idShaderPixel);
    }
  } else {
    GLint ok = GL_FALSE;
    shader.id = glCreateProgram(); // deleted in ~ScreenImp().
    glBindAttribLocation(shader.id, LOCATION_VPOS, "a_pos");
    glBindAttribLocation(shader.id, LOCATION_VCOLOR, "a_color");
    glBindAttribLocation(shader.id, LOCATION_VTEX, "a_uv");
    glAttachShader(shader.id, idShaderVertex);
    glAttachShader(shader.id, idShaderPixel);
    glLinkProgram(shader.id);
    glGetProgramiv(shader.id, GL_LINK_STATUS, &ok);
    if (!ok) {
      GLint log_z = 0;
      glGetProgramiv(shader.id, GL_INFO_LOG_LENGTH, &log_z);
      if (auto error_buf = malloc(log_z)) {
        glGetProgramInfoLog(shader.id, log_z, &log_z, (GLchar*)error_buf);
        fprintf(stderr, "error linking shader:\n%s\n", (char*)error_buf);
        free(error_buf);
      }
      glDeleteProgram(shader.id);
      shader.id = 0;
    }

    shader.uMVP = glGetUniformLocation(shader.id, "u_mvp");
    shader.uCtrlBlur = glGetUniformLocation(shader.id, "u_blur");
    shader.uCtrlMask = glGetUniformLocation(shader.id, "u_mask");
    shader.uCtrlFinal = glGetUniformLocation(shader.id, "u_final");
    shader.uTexMask = glGetUniformLocation(shader.id, "u_tex_mask");
    shader.uTexBlur = glGetUniformLocation(shader.id, "u_tex_blur");
    shader.uTexColor = glGetUniformLocation(shader.id, "u_tex_color");

    glUseProgram(shader.id);
    glDeleteShader(idShaderVertex);
    glDeleteShader(idShaderPixel);

    glm::mat4 identity(1.0f);
    glUniformMatrix4fv(shader.uMVP, 1, GL_FALSE, glm::value_ptr(identity));
  }
  CheckGLError();
  fprintf(stderr, "shader program ready\n");
}

std::future<void>
ScreenImp::swap()
{
  return push([this](int) { eglSwapBuffers(pDisplay, pSurface); });
}

GLuint
ScreenImp::getPNGTexture(const std::string& file)
{
  auto find_result =
    queueAccess.push([=](int) { return mapTextures.find(file); });

  auto it = find_result.get();
  if (it != mapTextures.end()) {
    // Already decoded this file before, so we know its texture ID.
    return it->second;
  }

  // This is a new PNG file we must decode.
  struct PNG
  {
    std::shared_ptr<uint8_t> pixels;
    int width;
    int height;
  } image;

  // We'll wait for results, so it is okay to capture by reference.
  auto decode_png = push([&](int) {
    image.width = 0;
    image.height = 0;
    MMapFile buffer(file.c_str());
    if (buffer.pMemory) {
      if (auto ctx = spng_ctx_new(0)) {
        spng_set_png_buffer(ctx, buffer.pMemory, buffer.zMemory);

        struct spng_ihdr ihdr;
        if (spng_get_ihdr(ctx, &ihdr) != 0) {
          fprintf(stderr, "IHDR chunk not available.\n");
        } else {
          size_t out_size = 0;
          auto e = spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &out_size);
          if (e != 0) {
            fprintf(stderr, "%s\n", spng_strerror(e));
          } else {
            std::shared_ptr<uint8_t> pixels(new uint8_t[out_size],
                                            std::default_delete<uint8_t[]>());
            e = spng_decode_image(
              ctx, pixels.get(), out_size, SPNG_FMT_RGBA8, SPNG_DECODE_TRNS);
            if (e != 0) {
              fprintf(stderr, "%s\n", spng_strerror(e));
            }

            image.pixels = pixels;
            image.width = ihdr.width;
            image.height = ihdr.height;
          }
        }
        spng_ctx_free(ctx);
      }
    }
  });

  decode_png.wait();
  if (image.width > 0 && image.height > 0) {
    auto texture = createTexture(image.width, image.height, image.pixels);
    mapTextures[file] = texture;
    fprintf(stderr, "loaded %s -> %u\n", file.c_str(), texture);
    return texture;
  }
  return 0;
}

GLuint
ScreenImp::createTexture(int w, int h, std::shared_ptr<uint8_t> buffer)
{
  auto result = push([=](int) {
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 (GLsizei)w,
                 (GLsizei)h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 buffer ? buffer.get() : nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CheckGLError();
    return id;
  });
  return result.get();
}
} // anonymous namespace

Screen::Ref
Screen::Create(int w, int h)
{
  return std::make_shared<ScreenImp>(w, h);
}

Screen::Screen(int, int)
  : ctpl::thread_pool(1)
{}

Screen::~Screen()
{
  stop();
}
