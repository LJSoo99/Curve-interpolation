# Curve Interpolation Viewer
**Curve Interpolation Viewer**는 다양한 곡선 보간(interpolation) 기법을 시각화하고 제어할 수 있는 2D 그래픽 도구입니다. 이 프로그램은 **JGL2 그래픽 라이브러리**를 사용하여 구현되었으며, 곡선 타입 선택, 드로잉 모드 전환, 제어점 조작 등을 직관적으로 사용할 수 있는 UI를 제공합니다.

## 주요 기능

- **지원 곡선 타입**:
  - 선형 보간 (Linear Interpolation)
  - 라그랑지 다항식 (Lagrangian Polynomial)
  - 베지에 곡선 (Bézier Curve)
  - Catmull-Rom 스플라인
  - 자연 스플라인 (Natural Cubic Spline)
  - B-스플라인 (B-Spline)

- **드로잉 모드**:
  - 곡선을 선으로 연결하여 표현.
  - 곡선을 개별 점으로 표현.

- **인터랙티브 UI**:
  - 제어점을 드래그하여 실시간으로 곡선 모양을 업데이트.
  - 드롭다운 메뉴를 통해 곡선 타입과 드로잉 모드 전환 가능.

## 시작하기

### 사전 준비
- **C++ 컴파일러**: C++17 이상을 지원하는 최신 C++ 컴파일러 필요 (예: GCC, Clang, MSVC).
- **JGL2 라이브러리**: 이 프로그램은 JGL2 라이브러리를 사용하므로 프로젝트 빌드 시 해당 라이브러리가 필요합니다.

### 설치 및 실행

1. 이 저장소를 클론합니다:
   ```bash
   git clone https://github.com/username/curve-interpolation-viewer.git
   cd curve-interpolation-viewer
   ```

2. 프로젝트를 빌드합니다:
   ```bash
   g++ -std=c++17 curveinter.cpp -o curveinter -Ipath_to_JGL2_library -Lpath_to_JGL2_library -lJGL2
   ```

3. 프로그램을 실행합니다:
   ```bash
   ./curveinter
   ```

### UI 사용법
1. **곡선 타입 선택**:
   - 상단의 드롭다운 메뉴에서 원하는 곡선 타입을 선택합니다.

2. **드로잉 모드 전환**:
   - 드롭다운 메뉴에서 "Lines" 또는 "Dots"를 선택하여 곡선을 선 또는 점으로 표시합니다.

3. **제어점 조작**:
   - 마우스로 제어점을 클릭 후 드래그하여 곡선을 변경할 수 있습니다.

## 코드 구조

- **evaluateCurve**: 선택한 곡선 타입에 따라 곡선 포인트를 계산.
- **updateCurve**: 곡선 포인트를 업데이트하고 화면에 다시 그리기.
- **CurveWidget**: 곡선을 렌더링하고 사용자 입력을 처리하는 UI 위젯.
- **main**: 프로그램 초기화, UI 구성, 이벤트 루프 실행.

## 추가 기능 아이디어

- 제어점 추가/삭제 기능.
- 곡선을 파일로 저장하거나 불러오는 기능.
- 추가 곡선 타입 (예: Hermite Curve, NURBS) 지원.

## 기여하기

이 프로젝트에 기여하고 싶다면 다음 단계를 따라주세요:

1. 이 저장소를 포크합니다.
2. 새로운 기능을 개발하거나 버그를 수정합니다.
3. 변경 사항을 푸시한 뒤 Pull Request를 생성합니다.
