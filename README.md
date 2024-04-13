# BSD-GDF 라이브러리

## 소개
BSD-GDF는 BSD 기반 Unix 운영 체제를 위한 C++ 라이브러리입니다. 커널 이벤트 처리를 지원하며 서버 프로그래밍에 중점을 둔 설계가 특징입니다. 이 라이브러리는 네트워킹, 이벤트 관리, 로깅을 위한 종합적인 도구 제공을 통해 서버 애플리케이션의 개발 과정을 간소화하는 것을 목표로 합니다.

## 특징
- `KernelEvent`, `KernelQueue`를 통한 커널 이벤트 처리
- `Network`를 통한 서버 중심의 네트워킹 유틸리티
- `GlobalLogger`를 이용한 전역 로깅시스템
- `Display`를 통한 효율적인 디스플레이 버퍼링, 콘솔 디스플레이 출력
- `AssertStream`를 통한 간편한 스트림 지원 어설션

## 시작하기

### 요구사항
- BSD 계열 유닉스 운영체제
- C++98 호환 컴파일러
- GNU계통 Makefile

### 설치
```
make
```

### 사용법
```cpp
#Include <BSD-GDF/Network.hpp>

#define PORT 6667

int main()
{
	gdf::Network server;

	server.Init(PORT);
	...
}

```
컴파일
```
c++ -I<this repo>/include -c main.cpp -o main.o
```
```
c++ -L<this repo>/lib -Wl,-rpath,<this repo>/lib -lbsd-gdf-network main.o
```
