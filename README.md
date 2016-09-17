# CU_ComputerGraphics_Examples


# Korean
* 본 프로젝트는 가톨릭대학교 미디어기술콘텐츠학과의 컴퓨터그래픽스 수업에서 사용되는 예제 프로그램의 모음이다.

* 과제 제출용으로 사용되는 프젝트의 경우 불완전한 코드를 포함할 수도있다.

* Visual Studio 2010에서 작성되었다.

## 의존성 

빌드하기 위해서는 다음의 라이브러리가 설치되어야한다.

1. glew
2. freeglut
3. GLM


## 라이브러리 설치

현재 작성되어있는 VS 프로젝트 설정을 그대로 사용하려면 다음과 같은 방법으로 설치하면 된다.

1. glew
 * static lib를 사용한다. 
 * glew32s.lib 파일을 ./lib 에 복사한다.
 * glew.h, glxew.h, wglew.h 등의 헤더 파일을 ./GL 에 복사한다.


2. freeglut
 * dynamic lib를 사용한다.
 * freeglut.lib 를 ./lib 에 복사한다.
 * freeglut.h, freeglut_ext.h, freeglut_std.h 등의 해더 파일을 ./GL 에 복사한다.
 * freeglut.dll 파일을 ./ 에 복사한다.


3. GLM
 * GLM은 Header-Only Lib 이다. 
 * 전체 헤더 파일을 ./glm 에 복사한다.




