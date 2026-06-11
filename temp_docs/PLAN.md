# 차량조립 KATA - 리팩토링 구현 방안 (OCP 적용)

## 목표

절차지향식 Legacy 코드를 **OCP(Open/Closed Principle)** 기반 인터페이스 구조로 리팩토링한다.
- CarType / Engine / BrakeSystem / SteeringSystem을 각각 인터페이스로 추상화
- 새 차량 타입 또는 부품 추가 시 **기존 코드 수정 없이 클래스 추가만으로** 확장 가능
- Google Mock 기반 유닛테스트 추가

---

## 도메인 명세 (PDF p.20~23)

### 제조 순서
1. **자동차 타입 선택** — Sedan / SUV / Truck (향후 타입 추가 가능)
2. **부품 선택** — 엔진 / 제동장치 / 조향장치 각각 선택
3. **호환성 검증** — 선택한 부품 조합이 해당 차량 타입에 유효한지 검사

### 부품 목록

| 부품 | 선택지 |
|------|--------|
| 엔진 (Engine) | GM / TOYOTA / WIA |
| 제동장치 (BrakeSystem) | MANDO / Continental / Bosch |
| 조향장치 (SteeringSystem) | Bosch / Mobis |

### 호환성 제한 조건

| # | 제한 | 내용 |
|---|------|------|
| 1 | 부품 간 | Bosch 제동장치 사용 시 → Bosch 조향장치만 사용 가능 |
| 2 | 타입 × 부품 | Sedan + Continental 제동장치 불가 |
| 3 | 타입 × 부품 | SUV + TOYOTA 엔진 불가 |
| 4 | 타입 × 부품 | Truck + WIA 엔진 불가 |
| 5 | 타입 × 부품 | Truck + MANDO 제동장치 불가 |

---

## 현재 코드 문제점 (PDF p.24)

| 항목 | 문제 |
|------|------|
| 구조 | 절차지향. `int stack[10]` 전역 상태, 단일 파일에 모든 로직 혼재 |
| 안전성 | `strtok_s`, C스타일 문자열, 전역 변수 |
| 확장성 | 타입/부품 추가 시 `isValidCheck` `testProducedCar` `runProducedCar` `main` 전체 수정 |
| 테스트 | Google Mock 틀만 존재, 테스트 케이스 없음 |

---

## OCP 설계 원칙

> **"확장에는 열려 있고, 수정에는 닫혀 있어야 한다."**

각 부품 타입을 인터페이스로 추상화하고, **호환성 로직을 각 구체 클래스 내부에 캡슐화**한다.
새 부품(예: `KiaEngine`)을 추가할 때 인터페이스를 구현하는 클래스만 작성하면 되며,
`CompatibilityChecker` 나 `CarAssembler` 등 기존 코드는 수정하지 않는다.

---

## 인터페이스 설계

### `ICarType`

```cpp
class ICarType {
public:
    virtual std::string getName() const = 0;
    virtual ~ICarType() = default;
};
```

### `IEngine`

```cpp
class IEngine {
public:
    virtual std::string getName() const = 0;
    virtual bool isCompatibleWith(const ICarType& carType) const = 0;
    virtual ~IEngine() = default;
};
```

### `IBrakeSystem`

```cpp
class IBrakeSystem {
public:
    virtual std::string getName() const = 0;
    virtual bool isCompatibleWithCarType(const ICarType& carType) const = 0;
    virtual bool isCompatibleWithSteering(const ISteeringSystem& steering) const = 0;
    virtual ~IBrakeSystem() = default;
};
```

### `ISteeringSystem`

```cpp
class ISteeringSystem {
public:
    virtual std::string getName() const = 0;
    virtual ~ISteeringSystem() = default;
};
```

---

## 구체 클래스 설계

### CarType 구현체

```cpp
class Sedan : public ICarType {
    std::string getName() const override { return "Sedan"; }
};
class SUV : public ICarType {
    std::string getName() const override { return "SUV"; }
};
class Truck : public ICarType {
    std::string getName() const override { return "Truck"; }
};
```

### Engine 구현체 — 호환성 로직을 클래스 내부에 캡슐화

```cpp
class GMEngine : public IEngine {
    std::string getName() const override { return "GM"; }
    bool isCompatibleWith(const ICarType&) const override { return true; }
};

class ToyotaEngine : public IEngine {
    std::string getName() const override { return "TOYOTA"; }
    bool isCompatibleWith(const ICarType& car) const override {
        return car.getName() != "SUV";  // SUV용 엔진 미생산
    }
};

class WIAEngine : public IEngine {
    std::string getName() const override { return "WIA"; }
    bool isCompatibleWith(const ICarType& car) const override {
        return car.getName() != "Truck";  // Truck용 엔진 미생산
    }
};
```

### BrakeSystem 구현체

```cpp
class MandoBrake : public IBrakeSystem {
    std::string getName() const override { return "MANDO"; }
    bool isCompatibleWithCarType(const ICarType& car) const override {
        return car.getName() != "Truck";
    }
    bool isCompatibleWithSteering(const ISteeringSystem&) const override { return true; }
};

class ContinentalBrake : public IBrakeSystem {
    std::string getName() const override { return "Continental"; }
    bool isCompatibleWithCarType(const ICarType& car) const override {
        return car.getName() != "Sedan";
    }
    bool isCompatibleWithSteering(const ISteeringSystem&) const override { return true; }
};

class BoschBrake : public IBrakeSystem {
    std::string getName() const override { return "Bosch"; }
    bool isCompatibleWithCarType(const ICarType&) const override { return true; }
    bool isCompatibleWithSteering(const ISteeringSystem& s) const override {
        return s.getName() == "Bosch";  // 타사 조향장치와 호환 불가
    }
};
```

### SteeringSystem 구현체

```cpp
class BoschSteering : public ISteeringSystem {
    std::string getName() const override { return "Bosch"; }
};
class MobisSteering : public ISteeringSystem {
    std::string getName() const override { return "Mobis"; }
};
```

---

## 지원 클래스 설계

### `Car` (선택된 부품 보유)

```cpp
class Car {
public:
    std::shared_ptr<ICarType>        carType;
    std::shared_ptr<IEngine>         engine;
    std::shared_ptr<IBrakeSystem>    brakeSystem;
    std::shared_ptr<ISteeringSystem> steeringSystem;
};
```

### `PartRegistry` (사용 가능한 부품 목록 관리)

```cpp
class PartRegistry {
public:
    const std::vector<std::shared_ptr<ICarType>>&        carTypes()        const;
    const std::vector<std::shared_ptr<IEngine>>&         engines()         const;
    const std::vector<std::shared_ptr<IBrakeSystem>>&    brakeSystems()    const;
    const std::vector<std::shared_ptr<ISteeringSystem>>& steeringSystems() const;
};
```

### `CompatibilityChecker` (얇은 조율자)

```cpp
class CompatibilityChecker {
public:
    bool        isValid(const Car& car) const;
    std::string getFailReason(const Car& car) const;
};

// 구현 — 규칙을 직접 알지 못하고, 각 인터페이스에게 위임
bool CompatibilityChecker::isValid(const Car& car) const {
    return car.engine->isCompatibleWith(*car.carType)
        && car.brakeSystem->isCompatibleWithCarType(*car.carType)
        && car.brakeSystem->isCompatibleWithSteering(*car.steeringSystem);
}
```

### `ConsoleUI` (동적 메뉴 출력)

```cpp
class ConsoleUI {
public:
    std::shared_ptr<ICarType>        askCarType(const PartRegistry& reg);
    std::shared_ptr<IEngine>         askEngine(const PartRegistry& reg);
    std::shared_ptr<IBrakeSystem>    askBrakeSystem(const PartRegistry& reg);
    std::shared_ptr<ISteeringSystem> askSteeringSystem(const PartRegistry& reg);
    int  askRunOrTest();
    void printCarResult(const Car& car);
    void printTestResult(bool valid, const std::string& reason);
    void clearScreen();
private:
    int readInt(int min, int max);
};
```

### `CarAssembler`

```cpp
class CarAssembler {
public:
    void run();
private:
    Car                  m_car;
    PartRegistry         m_registry;
    CompatibilityChecker m_checker;
    ConsoleUI            m_ui;
};
```

---

## 목표 파일 구조

```
carProject/
├── interfaces/
│   ├── ICarType.h
│   ├── IEngine.h
│   ├── IBrakeSystem.h
│   └── ISteeringSystem.h
├── parts/
│   ├── Sedan.h / SUV.h / Truck.h
│   ├── GMEngine.h / ToyotaEngine.h / WIAEngine.h
│   ├── MandoBrake.h / ContinentalBrake.h / BoschBrake.h
│   └── BoschSteering.h / MobisSteering.h
├── Car.h
├── PartRegistry.h
├── CompatibilityChecker.h
├── CarAssembler.h
├── ConsoleUI.h
├── assemble.cpp
└── tests/
    └── AssemblerTest.cpp
```

---

## 구현 Phase

> 각 Phase는 **독립적으로 빌드되고 실행 가능한 SW**를 산출물로 한다.
> 고객님께서 직접 실행하여 테스트하시고, 피드백을 주시면 다음 Phase로 진행합니다.

---

### Phase 1 — 차량 타입 · 엔진 선택 콘솔

**개발 목표**
- 인터페이스 4종 (`ICarType`, `IEngine`, `IBrakeSystem`, `ISteeringSystem`) 정의
- CarType 구현체 3종 (`Sedan`, `SUV`, `Truck`) 작성
- Engine 구현체 3종 (`GMEngine`, `ToyotaEngine`, `WIAEngine`) 작성 — 호환성 로직 포함
- `PartRegistry` 기본 구성 (CarType, Engine 등록)
- `ConsoleUI` : 차량 타입 선택 → 엔진 선택 2단계 메뉴
- `CarAssembler` : Phase 1 범위의 흐름 구동
- `Car` 클래스: carType / engine 상태 보유

**산출물**: Release 빌드 실행 시 차량 타입 → 엔진을 선택할 수 있는 콘솔 앱

**고객님 테스트 포인트**
```
1. 프로그램 실행 후 차량 타입 메뉴(Sedan / SUV / Truck)가 출력되는가?
2. 번호 입력으로 차량 타입을 선택할 수 있는가?
3. 이후 엔진 메뉴(GM / TOYOTA / WIA)가 출력되는가?
4. 엔진을 선택하면 선택한 타입과 엔진 이름이 화면에 출력되는가?
5. 범위 밖 숫자나 문자를 입력했을 때 오류 메시지가 나오고 재입력을 요청하는가?
```

---

### Phase 2 — 제동장치 · 조향장치 선택 + 이전 단계 복귀

**개발 목표**
- BrakeSystem 구현체 3종 (`MandoBrake`, `ContinentalBrake`, `BoschBrake`) 작성 — 호환성 로직 포함
- SteeringSystem 구현체 2종 (`BoschSteering`, `MobisSteering`) 작성
- `PartRegistry`에 BrakeSystem / SteeringSystem 추가 등록
- `ConsoleUI` : 제동장치 / 조향장치 선택 메뉴 추가
- `CarAssembler` : 4단계 전체 흐름 + **`0` 입력 시 이전 단계 복귀** 로직

**산출물**: 4개 부품을 순서대로 선택하고, 언제든 이전 단계로 돌아갈 수 있는 콘솔 앱

**고객님 테스트 포인트**
```
1. 차량 타입 → 엔진 → 제동장치 → 조향장치 4단계가 순서대로 진행되는가?
2. 각 단계에서 0을 입력하면 이전 메뉴로 돌아가는가?
3. 조향장치까지 선택 완료 시 선택한 4개 부품 전체가 화면에 출력되는가?
4. exit 입력 시 프로그램이 종료되는가?
```

---

### Phase 3 — 호환성 검사 + RUN / TEST

**개발 목표**
- `CompatibilityChecker` 구현 (각 인터페이스 메서드에 위임)
- 조립 완료 후 **RUN** / **TEST** / **처음으로** 선택 메뉴 추가
- RUN: 유효한 조합이면 차량 구성 출력 + "차량이 시동됩니다", 무효면 "작동하지 않습니다"
- TEST: 호환성 검사 결과(PASS / FAIL + 실패 사유) 출력
- 조향장치 완료 → RUN/TEST 메뉴까지 전체 흐름 완성

**산출물**: 부품 선택부터 호환성 검사까지 동작하는 완전한 콘솔 앱

**고객님 테스트 포인트**
```
[정상 조합 확인]
1. Sedan + GM + MANDO + Bosch 선택 후 RUN → 차량 구성 출력 + 시동 메시지가 나오는가?
2. 동일 조합 TEST → PASS가 출력되는가?

[비정상 조합 확인]
3. Sedan + GM + Continental + Mobis 선택 후 TEST → FAIL + "Sedan에는 Continental 사용 불가" 메시지가 나오는가?
4. SUV + TOYOTA + MANDO + Bosch 선택 후 RUN → "차량이 작동하지 않습니다" 메시지가 나오는가?
5. Truck + GM + MANDO + Bosch 선택 후 TEST → FAIL + "Truck에는 MANDO 사용 불가" 메시지가 나오는가?
6. Sedan + GM + Bosch(brake) + Mobis 선택 후 TEST → FAIL + "Bosch 제동장치에는 Bosch 조향장치만 사용 가능" 메시지가 나오는가?

[흐름 확인]
7. RUN/TEST 후 '처음으로'(0) 선택 시 차량 타입 선택 화면으로 돌아가는가?
```

---

### Phase 4 — 유닛테스트 완성 (Debug 빌드)

**개발 목표**
- `tests/AssemblerTest.cpp` 에 Google Mock 기반 테스트 케이스 8종 작성
- `assemble.cpp` 최종 정리 (Release: `CarAssembler().run()` 한 줄, Debug: GMock 진입점)

**산출물**: Debug 빌드 시 8개 테스트 케이스가 모두 PASS되는 상태

**고객님 테스트 포인트**
```
1. Visual Studio에서 빌드 구성을 Debug로 변경 후 실행
2. 콘솔에 아래와 유사한 출력이 나오는가?

   [==========] Running 8 tests from 1 test suite.
   [ RUN      ] CompatibilityTest.SedanContinental_Fails
   [       OK ] CompatibilityTest.SedanContinental_Fails
   ...
   [==========] 8 tests ran.
   [  PASSED  ] 8 tests.

3. FAILED 항목이 0개인가?
```

| # | 테스트 케이스 | 기대 결과 |
|---|-------------|----------|
| 1 | Sedan + ContinentalBrake | `isValid` → false |
| 2 | SUV + ToyotaEngine | `isValid` → false |
| 3 | Truck + WIAEngine | `isValid` → false |
| 4 | Truck + MandoBrake | `isValid` → false |
| 5 | BoschBrake + MobisSteering | `isValid` → false |
| 6 | Sedan + GMEngine + MandoBrake + BoschSteering | `isValid` → true |
| 7 | Truck + GMEngine + BoschBrake + BoschSteering | `isValid` → true |
| 8 | SUV + GMEngine + ContinentalBrake + MobisSteering | `isValid` → true |

---

## OCP 확장성 검증 기준

**새 차량 타입 `VAN` 추가 시 수정 범위:**
1. `Van.h` 신규 작성 (`ICarType` 구현)
2. `PartRegistry`에 `Van` 인스턴스 등록
3. VAN 관련 제한이 있는 엔진/브레이크의 `isCompatibleWith` 내부만 수정

`CompatibilityChecker`, `CarAssembler`, `ConsoleUI` — **수정 없음**

**새 엔진 `KiaEngine` 추가 시 수정 범위:**
1. `KiaEngine.h` 신규 작성 (`IEngine` 구현)
2. `PartRegistry`에 `KiaEngine` 인스턴스 등록

그 외 모든 기존 코드 — **수정 없음**
