# carProject

자동차 부품 조립 시뮬레이터 콘솔 애플리케이션입니다.

## 개요

사용자가 단계별로 자동차 부품을 선택하여 조립하고, 조립된 차량의 유효성을 검증하거나 실행해볼 수 있는 C++ 콘솔 프로그램입니다.

## 빌드 환경

- **언어**: C++
- **IDE**: Visual Studio 2022 (v143 toolset)
- **플랫폼**: Windows (Win32 / x64)
- **테스트 프레임워크**: Google Mock (Debug 빌드 시 활성화)

## 빌드 방법

Visual Studio에서 `carProject.sln`을 열고 빌드합니다.

| 구성 | 설명 |
|------|------|
| Debug | Google Mock 기반 단위 테스트 실행 |
| Release | 자동차 조립 시뮬레이터 실행 |

## 사용 방법

Release 빌드 실행 후 단계별 메뉴에서 번호를 입력합니다.

### 조립 단계

1. **차량 타입 선택** — Sedan / SUV / Truck
2. **엔진 선택** — GM / TOYOTA / WIA / 엔진 없음
3. **브레이크 시스템 선택** — MANDO / CONTINENTAL / BOSCH
4. **스티어링 시스템 선택** — BOSCH / MOBIS
5. **RUN 또는 Test** 실행

> 각 단계에서 `0` 입력 시 이전 단계로 돌아갑니다.  
> `exit` 입력 시 프로그램을 종료합니다.

## 부품 호환성 규칙

아래 조합은 유효하지 않으며 RUN 실패 또는 Test FAIL 처리됩니다.

| 차량 타입 | 호환 불가 부품 | 사유 |
|-----------|---------------|------|
| Sedan | CONTINENTAL 브레이크 | 조합 불가 |
| SUV | TOYOTA 엔진 | 조합 불가 |
| Truck | WIA 엔진 | 조합 불가 |
| Truck | MANDO 브레이크 | 조합 불가 |
| (모든 타입) | BOSCH 브레이크 + BOSCH 이외 스티어링 | BOSCH 브레이크는 BOSCH 스티어링과만 사용 가능 |

## 프로젝트 구조

```
carProject/
├── carProject.sln          # Visual Studio 솔루션
└── carProject/
    ├── assemble.cpp        # 메인 소스 (조립 로직 전체)
    ├── carProject.vcxproj  # 프로젝트 설정
    └── carProject.vcxproj.filters
```
