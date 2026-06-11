#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "interfaces/ICarType.h"
#include "PartRegistry.h"

class ConsoleUI {
public:
    std::shared_ptr<ICarType> askCarType(const PartRegistry& reg) {
        const auto& types = reg.carTypes();
        while (true) {
            clearScreen();
            std::cout << "        ______________\n";
            std::cout << "       /|            | \n";
            std::cout << "  ____/_|_____________|____\n";
            std::cout << " |                      O  |\n";
            std::cout << " '-(@)----------------(@)--'\n";
            std::cout << "===============================\n";
            std::cout << "어떤 차량 타입을 선택하시겠습니까?\n";
            for (int i = 0; i < static_cast<int>(types.size()); ++i)
                std::cout << i + 1 << ". " << types[i]->getName() << "\n";
            std::cout << "===============================\n";

            int input = readInt(1, static_cast<int>(types.size()));
            if (input != -1)
                return types[input - 1];
        }
    }

    void clearScreen() {
        std::cout << "\033[H\033[2J" << std::flush;
    }

private:
    int readInt(int min, int max) {
        std::string line;
        std::cout << "INPUT > ";
        if (!std::getline(std::cin, line)) return -1;

        if (line == "exit") {
            std::cout << "프로그램을 종료합니다.\n";
            exit(0);
        }

        try {
            size_t pos = 0;
            int val = std::stoi(line, &pos);
            if (pos != line.size()) {
                std::cout << "ERROR :: 숫자를 입력해주세요.\n";
                return -1;
            }
            if (val < min || val > max) {
                std::cout << "ERROR :: " << min << " ~ " << max
                          << " 사이의 숫자를 입력해주세요.\n";
                return -1;
            }
            return val;
        }
        catch (...) {
            std::cout << "ERROR :: 숫자를 입력해주세요.\n";
            return -1;
        }
    }
};
