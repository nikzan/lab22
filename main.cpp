#include <iostream>
#include <limits>
#include <fstream>
#include "ExpressionTree.h"

using namespace std;


int main() {
    bool flag = true;
    int xValue = 0;

    while (flag) {
        cout << "Выберите операцию:\n"
             << "1. Задание CalcTree1 \n"
             << "2. Задание CalcTree2 \n"
             << "0. Выход\n\n";
        int operation, inputChoice;
        cin >> operation;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите целочисленное значение.\n";
            continue;
        }
        cin.ignore();

        switch (operation) {
            case 1:
            {
                cout << "Задание CalcTree1" << "\n\n";

                cout << "Выберите способ ввода выражения:\n"
                     << "1. Использовать файл по умолчанию (input1.txt)\n"
                     << "2. Ввод с клавиатуры\n";
                cin >> inputChoice;
                cin.ignore();

                Tree tree1;
                if (inputChoice == 2) {
                    string sequence;
                    bool validInput = false;

                    while (!validInput) {
                        cout << "Введите выражение (Выражение должно быть в постфиксной форме)\n";
                        getline(cin, sequence);

                        if (!sequence.empty()) {
                            validInput = true;
                        } else {
                            cout << "Ошибка: пустое выражение. Пожалуйста, введите корректное выражение.\n";
                        }
                    }

                    ofstream outFile("../input1.txt");
                    outFile << sequence;
                }


                tree1.buildFromPostfix("../input1.txt");
                tree1.removeSubtraction();
                tree1.PrintTree();
                continue;
            }

            case 2: {
                cout << "\nЗадание CalcTree23" << "\n\n";
                cout << "Выберите способ ввода выражения:\n"
                     << "1. Использовать файл по умолчанию (FN1.txt)\n"
                     << "2. Ввод с клавиатуры\n";
                cin >> inputChoice;
                cin.ignore();

                Tree tree2;
                if (inputChoice == 2) {
                    string sequence;
                    bool validInput = false;

                    while (!validInput) {
                        cout << "Введите выражение (Выражение должно быть в инфиксной форме)\n";
                        getline(cin, sequence);

                        if (!sequence.empty()) {
                            validInput = true;
                        } else {
                            cout << "Ошибка: пустое выражение. Пожалуйста, введите корректное выражение.\n";
                        }
                    }

                    bool validX = false;
                    while (!validX) {
                        cout << "Введите значение x: ";
                        if (cin >> xValue) {
                            validX = true;
                        } else {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ошибка: введите целочисленное значение для x.\n";
                        }
                    }
                    cout << endl;
                    cin.ignore();

                    ofstream outFile("../FN1.txt");
                    outFile << sequence;
                }
                if (inputChoice == 1) {
                    bool validX = false;
                    while (!validX) {
                        cout << "Введите значение x: ";
                        if (cin >> xValue) {
                            validX = true;
                        } else {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ошибка: введите целочисленное значение для x.\n";
                        }
                    }
                    cout << endl;
                    cin.ignore();
                }
                tree2.buildFromInfix("../FN1.txt");
                tree2.PrintTree();

                cout << "Введите имя файла для вывода (по умолчанию FN2.txt): ";
                string outputFile;
                getline(cin, outputFile);
                if (outputFile.empty()) {
                    outputFile = "FN2.txt";
                }

                tree2.printToFile("../" + outputFile, xValue);
                continue;
            }
            case 0: {
                flag = false;
                continue;
            }
            default: cout << "Ошибка: неверный ввод. Пожалуйста, введите корректный номер операции.\n";
        }

    }

    return 0;
}
