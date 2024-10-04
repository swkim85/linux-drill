#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// 모델 로딩 함수
void loadModel(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  // 모델 처리 로직
}

// 렌더링 함수
void render() {
    // OpenGL 렌더링 로직
}

// 이미지 저장 함수
void saveImage(const char* filename, int width, int height) {
  GLubyte* pixels = new GLubyte[3 * width * height];
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  stbi_write_png(filename, width, height, 3, pixels, width * 3);
  delete[] pixels;
}

int main() {
  int windowWidth = 200;
  int windowHeight = 200;

  // GLFW 초기화 및 윈도우 생성
  // OpenGL 컨텍스트 설정
  // 모델 로딩
  loadModel("../table.obj");
  // 렌더링
  render();
  // 이미지 저장
  saveImage("output.png", windowWidth, windowHeight);
  return 0;
}
