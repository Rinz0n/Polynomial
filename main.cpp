#include "poly_node.h"
#include "polynomial_exceptions.h"
#include "polynomial.h"
#include "polynomial.cpp"
#include "poly_node.cpp"


void showMenu() {
    using namespace std;
    cout << "\n========== МЕНЮ ==========" << endl;
    cout << "1. Загрузить многочлен из файла" << endl;
    cout << "2. Показать текущий многочлен" << endl;
    cout << "3. Привести подобные члены" << endl;
    cout << "4. Записать результат в конец файла" << endl;
    cout << "5. Ввести многочлен вручную" << endl;
    cout << "0. Выход" << endl;
    cout << "==========================" << endl;
    cout << "Ваш выбор: ";
}

void clearInputStream() {
    using namespace std;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    using namespace std;
    setlocale(LC_ALL, "ru");

    Polynomial poly;
    string filename;
    int choice;
    bool polynomialLoaded = false;

    do {
        showMenu();
        cin >> choice;

        if (cin.fail()) {
            clearInputStream();
            cout << "Ошибка: введите число от 0 до 5" << endl;
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "Введите имя файла: ";
                cin >> filename;
                try {
                    poly.loadFromFile(filename);
                    polynomialLoaded = true;
                    cout << "Многочлен успешно загружен!" << endl;
                    if (poly.hasVariable()) {
                        cout << "Переменная: " << poly.getVariable() << endl;
                    } else {
                        cout << "Многочлен не содержит переменной (только свободные члены)" << endl;
                    }
                    poly.show();
                } catch (const PolynomialException& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
                break;
            }

            case 2: {
                if (!polynomialLoaded) {
                    cout << "Многочлен не загружен. Сначала выполните пункт 1 или 5." << endl;
                } else {
                    poly.show();
                }
                break;
            }

            case 3: {
                if (!polynomialLoaded) {
                    cout << "Многочлен не загружен. Сначала выполните пункт 1 или 5." << endl;
                } else {
                    try {
                        if (poly.isEmpty()) {
                            throw EmptyPolynomialException();
                        }
                        cout << "Приведение подобных членов..." << endl;
                        poly.combineLikeTerms();
                        cout << "Результат:" << endl;
                        poly.show();
                    } catch (const PolynomialException& e) {
                        cout << "Ошибка: " << e.what() << endl;
                    }
                }
                break;
            }

            case 4: {
                if (!polynomialLoaded) {
                    cout << "Многочлен не загружен. Сначала выполните пункт 1 или 5." << endl;
                } else {
                    if (filename.empty()) {
                        cout << "Не указано имя файла для записи. Сначала загрузите из файла (пункт 1)." << endl;
                    } else {
                        try {
                            poly.appendToFile(filename);
                            cout << "Результат записан в конец файла: " << filename << endl;
                        } catch (const PolynomialException& e) {
                            cout << "Ошибка: " << e.what() << endl;
                        }
                    }
                }
                break;
            }

            case 5: {
                string inputStr;
                cout << "Введите многочлен в формате (например: 52y^10 - 3y^8 + y, 4x^2 + 5x - 2, 0, 5): ";
                clearInputStream();
                getline(cin, inputStr);
                try {
                    poly.parseFromString(inputStr);
                    polynomialLoaded = true;
                    cout << "Многочлен успешно введён!" << endl;
                    if (poly.hasVariable()) {
                        cout << "Переменная: " << poly.getVariable() << endl;
                    } else {
                        cout << "Многочлен не содержит переменной (только свободные члены)" << endl;
                    }
                    poly.show();
                } catch (const PolynomialException& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
                break;
            }

            case 0: {
                cout << "Выход из программы. До свидания!" << endl;
                break;
            }

            default: {
                cout << "Неверный выбор. Пожалуйста, введите число от 0 до 5." << endl;
                break;
            }
        }
    } while (choice != 0);

    return 0;
}