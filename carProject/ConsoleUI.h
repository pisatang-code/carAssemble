#pragma once
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Car.h"
#include "PartRegistry.h"

class ConsoleUI {
public:
    std::shared_ptr<ICarType> askCarType(const PartRegistry& reg) {
        const auto& list = reg.carTypes();
        while (true) {
            clearScreen();
            printf("        ______________\n");
            printf("       /|            | \n");
            printf("  ____/_|_____________|____\n");
            printf(" |                      O  |\n");
            printf(" '-(@)----------------(@)--'\n");
            printMenu("어떤 차량 타입을 선택하시겠습니까?", list);
            int idx = readChoice((int)list.size(), false);
            if (idx >= 0) return list[idx];
        }
    }

    std::shared_ptr<IEngine> askEngine(const PartRegistry& reg) {
        const auto& list = reg.engines();
        while (true) {
            clearScreen();
            printMenu("어떤 엔진을 탑재하시겠습니까?", list);
            int idx = readChoice((int)list.size(), false);
            if (idx >= 0) return list[idx];
        }
    }

    void printSelectionResult(const Car& car) {
        clearScreen();
        printf("===============================\n");
        printf("선택하신 부품:\n");
        printf("  차량 타입 : %s\n", car.carType->getName().c_str());
        printf("  엔진      : %s\n", car.engine->getName().c_str());
        printf("===============================\n");
        printf("Enter를 누르면 처음으로 돌아갑니다.\n");
        printf("INPUT > ");
        std::string line;
        std::getline(std::cin, line);
        if (line == "exit") { printf("프로그램을 종료합니다.\n"); exit(0); }
    }

    void clearScreen() { printf("\033[H\033[2J"); }

private:
    template<typename T>
    void printMenu(const std::string& prompt, const std::vector<std::shared_ptr<T>>& items) {
        printf("%s\n", prompt.c_str());
        for (int i = 0; i < (int)items.size(); ++i)
            printf("%d. %s\n", i + 1, items[i]->getName().c_str());
        printf("===============================\n");
    }

    // 유효한 선택지를 받을 때까지 반복. allowBack=true이면 0 입력 시 -1 반환
    int readChoice(int count, bool allowBack) {
        while (true) {
            printf("INPUT > ");
            std::string line;
            if (!std::getline(std::cin, line)) exit(0);

            if (line == "exit") { printf("프로그램을 종료합니다.\n"); exit(0); }
            if (allowBack && line == "0") return -1;

            try {
                int val = std::stoi(line);
                if (val >= 1 && val <= count) return val - 1;
            } catch (...) {}

            printf("ERROR :: 1 ~ %d 범위의 숫자를 입력하세요\n", count);
        }
    }
};
