#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CLEAR_SCREEN "\033[H\033[2J"

int stack[10];

enum QuestionType
{
    CarType_Q,
    Engine_Q,
    brakeSystem_Q,
    SteeringSystem_Q,
    Run_Test,
};

enum CarType
{
    SEDAN = 1,
    SUV,
    TRUCK
};

enum Engine
{
    GM = 1,
    TOYOTA,
    WIA
};

enum brakeSystem
{
    MANDO = 1,
    CONTINENTAL,
    BOSCH_B
};

enum SteeringSystem
{
    BOSCH_S = 1,
    MOBIS
};

void delay(int ms)
{
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            for (int t = 0; t < ms; t++)
            {
                sum++;
            }
        }
    }
}

void selectCarType(int answer)
{
    stack[CarType_Q] = answer;
    if (answer == 1)
        printf("차량 타입으로 Sedan을 선택하셨습니다.\n");
    if (answer == 2)
        printf("차량 타입으로 SUV를 선택하셨습니다.\n");
    if (answer == 3)
        printf("차량 타입으로 Truck을 선택하셨습니다.\n");
}

void selectEngine(int answer)
{
    stack[Engine_Q] = answer;
    if (answer == 1)
        printf("GM 엔진을 선택하셨습니다.\n");
    if (answer == 2)
        printf("TOYOTA 엔진을 선택하셨습니다.\n");
    if (answer == 3)
        printf("WIA 엔진을 선택하셨습니다.\n");
}

void selectbrakeSystem(int answer)
{
    stack[brakeSystem_Q] = answer;
    if (answer == 1)
        printf("MANDO 제동장치를 선택하셨습니다.\n");
    if (answer == 2)
        printf("CONTINENTAL 제동장치를 선택하셨습니다.\n");
    if (answer == 3)
        printf("BOSCH 제동장치를 선택하셨습니다.\n");
}

void selectSteeringSystem(int answer)
{
    stack[SteeringSystem_Q] = answer;
    if (answer == 1)
        printf("BOSCH 조향장치를 선택하셨습니다.\n");
    if (answer == 2)
        printf("MOBIS 조향장치를 선택하셨습니다.\n");
}

int isValidCheck()
{
    if (stack[CarType_Q] == SEDAN && stack[brakeSystem_Q] == CONTINENTAL)
        return 0;
    if (stack[CarType_Q] == SUV && stack[Engine_Q] == TOYOTA)
        return 0;
    if (stack[CarType_Q] == TRUCK && stack[Engine_Q] == WIA)
        return 0;
    if (stack[CarType_Q] == TRUCK && stack[brakeSystem_Q] == MANDO)
        return 0;
    if (stack[brakeSystem_Q] == BOSCH_B && stack[SteeringSystem_Q] != BOSCH_S)
        return 0;
    return 1;
}

void runProducedCar()
{
    if (!isValidCheck())
    {
        printf("차량이 작동하지 않습니다\n");
        return;
    }

    if (stack[Engine_Q] == 4)
    {
        printf("엔진이 없어졌습니다.\n");
        printf("차량이 시동되지 않습니다.\n");
        return;
    }

    if (stack[CarType_Q] == 1) printf("Car Type : Sedan\n");
    if (stack[CarType_Q] == 2) printf("Car Type : SUV\n");
    if (stack[CarType_Q] == 3) printf("Car Type : Truck\n");
    if (stack[Engine_Q] == 1)  printf("Engine : GM\n");
    if (stack[Engine_Q] == 2)  printf("Engine : TOYOTA\n");
    if (stack[Engine_Q] == 3)  printf("Engine : WIA\n");
    if (stack[brakeSystem_Q] == 1) printf("Brake System : Mando\n");
    if (stack[brakeSystem_Q] == 2) printf("Brake System : Continental\n");
    if (stack[brakeSystem_Q] == 3) printf("Brake System : Bosch\n");
    if (stack[SteeringSystem_Q] == 1) printf("SteeringSystem : Bosch\n");
    if (stack[SteeringSystem_Q] == 2) printf("SteeringSystem : Mobis\n");
    printf("차량이 시동됩니다.\n");
}

void testProducedCar()
{
    if (stack[CarType_Q] == SEDAN && stack[brakeSystem_Q] == CONTINENTAL)
    {
        printf("차량의 품질 안전 테스트 결과 : FAIL\n");
        printf("Sedan에는 Continental제동장치 사용 불가\n");
    }
    else if (stack[CarType_Q] == SUV && stack[Engine_Q] == TOYOTA)
    {
        printf("차량의 품질 안전 테스트 결과 : FAIL\n");
        printf("SUV에는 TOYOTA엔진 사용 불가\n");
    }
    else if (stack[CarType_Q] == TRUCK && stack[Engine_Q] == WIA)
    {
        printf("차량의 품질 안전 테스트 결과 : FAIL\n");
        printf("Truck에는 WIA엔진 사용 불가\n");
    }
    else if (stack[CarType_Q] == TRUCK && stack[brakeSystem_Q] == MANDO)
    {
        printf("차량의 품질 안전 테스트 결과 : FAIL\n");
        printf("Truck에는 Mando제동장치 사용 불가\n");
    }
    else if (stack[brakeSystem_Q] == BOSCH_B && stack[SteeringSystem_Q] != BOSCH_S)
    {
        printf("차량의 품질 안전 테스트 결과 : FAIL\n");
        printf("Bosch제동장치에는 Bosch조향장치 이외 사용 불가\n");
    }
    else
    {
        printf("차량의 품질 안전 테스트 결과 : PASS\n");
    }
}

// =====================================================================
// main — Debug 빌드: Google Mock 테스트 실행
//        Release 빌드: 차량 조립 시뮬레이터 실행
// =====================================================================

#ifdef _DEBUG

#include "gmock/gmock.h"

TEST(IsValidCheckTest, Sedan_Continental_Fails)
{
    stack[CarType_Q]     = SEDAN;
    stack[Engine_Q]      = GM;
    stack[brakeSystem_Q] = CONTINENTAL;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_EQ(isValidCheck(), 0);
}

TEST(IsValidCheckTest, SUV_Toyota_Fails)
{
    stack[CarType_Q]     = SUV;
    stack[Engine_Q]      = TOYOTA;
    stack[brakeSystem_Q] = MANDO;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_EQ(isValidCheck(), 0);
}

TEST(IsValidCheckTest, Truck_WIA_Fails)
{
    stack[CarType_Q]     = TRUCK;
    stack[Engine_Q]      = WIA;
    stack[brakeSystem_Q] = BOSCH_B;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_EQ(isValidCheck(), 0);
}

TEST(IsValidCheckTest, Truck_Mando_Fails)
{
    stack[CarType_Q]     = TRUCK;
    stack[Engine_Q]      = GM;
    stack[brakeSystem_Q] = MANDO;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_EQ(isValidCheck(), 0);
}

TEST(IsValidCheckTest, BoschBrake_MobisSteering_Fails)
{
    stack[CarType_Q]     = SEDAN;
    stack[Engine_Q]      = GM;
    stack[brakeSystem_Q] = BOSCH_B;
    stack[SteeringSystem_Q] = MOBIS;
    EXPECT_EQ(isValidCheck(), 0);
}

TEST(IsValidCheckTest, Sedan_GM_Mando_Bosch_Passes)
{
    stack[CarType_Q]     = SEDAN;
    stack[Engine_Q]      = GM;
    stack[brakeSystem_Q] = MANDO;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_EQ(isValidCheck(), 1);
}

TEST(IsValidCheckTest, Truck_GM_Bosch_Bosch_Passes)
{
    stack[CarType_Q]     = TRUCK;
    stack[Engine_Q]      = GM;
    stack[brakeSystem_Q] = BOSCH_B;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_EQ(isValidCheck(), 1);
}

TEST(IsValidCheckTest, SUV_GM_Continental_Mobis_Passes)
{
    stack[CarType_Q]     = SUV;
    stack[Engine_Q]      = GM;
    stack[brakeSystem_Q] = CONTINENTAL;
    stack[SteeringSystem_Q] = MOBIS;
    EXPECT_EQ(isValidCheck(), 1);
}

int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

#include "CarAssembler.h"

int main()
{
    CarAssembler().run();
    return 0;
}

// =====================================================================
// 아래는 더 이상 사용되지 않는 구 절차지향 main (참조용 보존)
// =====================================================================
static void legacyMain_unused()
{
    char buf[100];
    int step = CarType_Q;

    while (1)
    {
        if (step == CarType_Q)
        {
            printf(CLEAR_SCREEN);
            printf("        ______________\n");
            printf("       /|            | \n");
            printf("  ____/_|_____________|____\n");
            printf(" |                      O  |\n");
            printf(" '-(@)----------------(@)--'\n");
            printf("===============================\n");
            printf("어떤 차량 타입을 선택하시겠습니까?\n");
            printf("1. Sedan\n");
            printf("2. SUV\n");
            printf("3. Truck\n");
        }
        else if (step == Engine_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 엔진을 탑재하시겠습니까?\n");
            printf("0. 이전으로\n");
            printf("1. GM\n");
            printf("2. TOYOTA\n");
            printf("3. WIA\n");
        }
        else if (step == brakeSystem_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 제동장치를 선택하시겠습니까?\n");
            printf("0. 이전으로\n");
            printf("1. MANDO\n");
            printf("2. CONTINENTAL\n");
            printf("3. BOSCH\n");
        }
        else if (step == SteeringSystem_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 조향장치를 선택하시겠습니까?\n");
            printf("0. 이전으로\n");
            printf("1. BOSCH\n");
            printf("2. MOBIS\n");
        }
        else if (step == Run_Test)
        {
            printf(CLEAR_SCREEN);
            printf("차량 조립이 완성되었습니다.\n");
            printf("어떻게 하시겠습니까?\n");
            printf("0. 처음 화면으로 돌아가기\n");
            printf("1. RUN\n");
            printf("2. Test\n");
        }
        printf("===============================\n");
        printf("INPUT > ");
        fgets(buf, sizeof(buf), stdin);

        char* context = nullptr;
        strtok_s(buf, "\r", &context);
        strtok_s(buf, "\n", &context);

        if (!strcmp(buf, "exit"))
        {
            printf("프로그램을 종료합니다.\n");
            break;
        }

        char* checkNumber;
        int answer = strtol(buf, &checkNumber, 10);

        if (*checkNumber != '\0')
        {
            printf("ERROR :: 숫자를 입력 하세요\n");
            delay(800);
            continue;
        }

        if (step == CarType_Q && !(answer >= 1 && answer <= 3))
        {
            printf("ERROR :: 차량 타입은 1 ~ 3 범위로 입력 하세요\n");
            delay(800);
            continue;
        }
        if (step == Engine_Q && !(answer >= 0 && answer <= 3))
        {
            printf("ERROR :: 엔진은 0 ~ 3 범위로 입력 하세요\n");
            delay(800);
            continue;
        }
        if (step == brakeSystem_Q && !(answer >= 0 && answer <= 3))
        {
            printf("ERROR :: 제동장치는 0 ~ 3 범위로 입력 하세요\n");
            delay(800);
            continue;
        }
        if (step == SteeringSystem_Q && !(answer >= 0 && answer <= 2))
        {
            printf("ERROR :: 조향장치는 0 ~ 2 범위로 입력 하세요\n");
            delay(800);
            continue;
        }
        if (step == Run_Test && !(answer >= 0 && answer <= 2))
        {
            printf("ERROR :: RUN 또는 Test 중 하나를 선택 하세요\n");
            delay(800);
            continue;
        }

        if (answer == 0 && step == Run_Test)
        {
            step = CarType_Q;
            continue;
        }
        if (answer == 0 && step >= 1)
        {
            step -= 1;
            continue;
        }

        if (step == CarType_Q)
        {
            selectCarType(answer);
            delay(800);
            step = Engine_Q;
        }
        else if (step == Engine_Q)
        {
            selectEngine(answer);
            delay(800);
            step = brakeSystem_Q;
        }
        else if (step == brakeSystem_Q)
        {
            selectbrakeSystem(answer);
            delay(800);
            step = SteeringSystem_Q;
        }
        else if (step == SteeringSystem_Q)
        {
            selectSteeringSystem(answer);
            delay(800);
            step = Run_Test;
        }
        else if (step == Run_Test && answer == 1)
        {
            runProducedCar();
            delay(2000);
        }
        else if (step == Run_Test && answer == 2)
        {
            printf("Test...\n");
            delay(1500);
            testProducedCar();
            delay(2000);
        }
    }
}

#endif
