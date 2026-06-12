#include "polynomial.h"

Polynomial::Polynomial() : head(nullptr), variable('\0') {}  // Конструктор по умолчанию

// Конструктор копирования
Polynomial::Polynomial(const Polynomial& other) : head(nullptr), variable(other.variable) {
    using namespace std;
    PolyNode* current = other.head;
    while (current != nullptr) {
        addTerm(current->coeff, current->power);
        current = current->next;
    }
}

// Оператор присваивания
Polynomial& Polynomial::operator=(const Polynomial& other) {
    using namespace std;
    if (this != &other) {
        clear();
        variable = other.variable;
        PolyNode* current = other.head;
        while (current != nullptr) {
            addTerm(current->coeff, current->power);
            current = current->next;
        }
    }
    return *this;
}

// Деструктор
Polynomial::~Polynomial() {
    clear();
}

// Очистка списка
void Polynomial::clear() {
    using namespace std;
    PolyNode* current = head;
    while (current != nullptr) {
        PolyNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    variable = '\0';
}

// Проверка на пустоту
bool Polynomial::isEmpty() const {
    return head == nullptr;
}

// Получение переменной
char Polynomial::getVariable() const {
    return variable;
}

// Проверка наличия переменной
bool Polynomial::hasVariable() const {
    return variable != '\0';
}

// Проверка, является ли переданный символ c допустимой буквой для имени переменной в многочлене
bool Polynomial::isVariableChar(char c) const {
    using namespace std;
    return isalpha(c) && c != '^' && c != '+' && c != '-';
}

// Определение переменной из строки
char Polynomial::detectVariable(const std::string& str) const {
    using namespace std;
    for (char c : str) {
        if (isVariableChar(c)) {
            return c;
        }
    }
    return '\0';
}

// Добавление члена в конец списка
void Polynomial::addTerm(int coeff, int power) {
    if (coeff == 0) return;

    PolyNode* newNode = new PolyNode(coeff, power);

    if (head == nullptr) {
        head = newNode;
    } else {
        PolyNode* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Получение длины списка
int Polynomial::getLength() const {
    int len = 0;
    PolyNode* current = head;
    while (current != nullptr) {
        len++;
        current = current->next;
    }
    return len;
}

// Получение узла по индексу
PolyNode* Polynomial::getNodeAt(int index) const {
    if (index < 0) return nullptr;
    PolyNode* current = head;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current->next;
    }
    return current;
}

// Парсинг строки в список
void Polynomial::parseFromString(const std::string& str) {
    using namespace std;
    clear();

    string s = str;
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

    if (s.empty()) {
        throw ParseErrorException("Пустая строка");
    }

    char detectedVar = detectVariable(s);
    variable = detectedVar;

    vector<string> terms;
    string currentTerm;

    for (size_t i = 0; i < s.length(); i++) {
        if (i > 0 && (s[i] == '+' || s[i] == '-')) {
            if (!currentTerm.empty()) {
                terms.push_back(currentTerm);
            }
            currentTerm = s[i];
        } else {
            currentTerm += s[i];
        }
    }
    if (!currentTerm.empty()) {
        terms.push_back(currentTerm);
    }

    for (const string& term : terms) {
        int coeff = 1;
        int power = 0;
        
        size_t posVar = string::npos;
        
        // Проверяем корректность члена: не должно быть двух переменных
        int varCount = 0;
        for (size_t i = 0; i < term.length(); i++) {
            if (isVariableChar(term[i])) {
                varCount++;
                posVar = i;
            }
        }
        
        if (varCount > 1) {
            throw ParseErrorException("Некорректный член: несколько переменных в '" + term + "'");
        }
        
        if (variable == '\0' && varCount == 1) {
            variable = term[posVar];
        }
        
        if (variable != '\0' && varCount == 1 && term[posVar] != variable) {
            throw MultipleVariableException(variable, term[posVar]);
        }

        if (varCount == 0) {
            // Свободный член
            try {
                coeff = stoi(term);
                power = 0;
            } catch (...) {
                throw ParseErrorException("Некорректный свободный член: " + term);
            }
        } else {
            // Есть переменная
            string coeffStr = term.substr(0, posVar);
            if (coeffStr.empty() || coeffStr == "+") {
                coeff = 1;
            } else if (coeffStr == "-") {
                coeff = -1;
            } else {
                try {
                    coeff = stoi(coeffStr);
                } catch (...) {
                    throw ParseErrorException("Некорректный коэффициент: " + coeffStr);
                }
            }

            // Проверяем, что после переменной
            string afterVar = term.substr(posVar + 1);
            
            if (afterVar.empty()) {
                power = 1;
            } else if (afterVar[0] == '^') {
                // Есть степень
                if (afterVar.length() < 2) {
                    throw ParseErrorException("Отсутствует показатель степени после ^ в: " + term);
                }
                
                size_t powStart = 1;
                size_t powEnd = powStart;
                while (powEnd < afterVar.length() && isdigit(afterVar[powEnd])) {
                    powEnd++;
                }
                
                if (powStart == powEnd) {
                    throw ParseErrorException("Отсутствует число после ^ в: " + term);
                }
                
                string powerStr = afterVar.substr(powStart, powEnd - powStart);
                try {
                    power = stoi(powerStr);
                    if (power < 0) {
                        throw ParseErrorException("Степень не может быть отрицательной");
                    }
                } catch (...) {
                    throw ParseErrorException("Некорректная степень: " + powerStr);
                }
                
                // Проверяем, что после степени нет лишних символов
                if (powEnd < afterVar.length()) {
                    throw ParseErrorException("Некорректный член '" + term + "': лишние символы после степени '" + afterVar.substr(powEnd) + "'");
                }
            } else {
                // После переменной идёт что-то, но не ^
                throw ParseErrorException("Некорректный член '" + term + "': после переменной ожидается ^ или конец члена");
            }
        }
        addTerm(coeff, power);
    }
}

// Сортировка по убыванию степени (пузырьком)
void Polynomial::sortByPowerDesc() {
    if (head == nullptr || head->next == nullptr) return;

    bool swapped;
    PolyNode* ptr1;
    PolyNode* lptr = nullptr;

    do {
        swapped = false;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->power < ptr1->next->power) {
                int tempCoeff = ptr1->coeff;
                int tempPower = ptr1->power;
                ptr1->coeff = ptr1->next->coeff;
                ptr1->power = ptr1->next->power;
                ptr1->next->coeff = tempCoeff;
                ptr1->next->power = tempPower;
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Удаление нулевых членов
void Polynomial::removeZeroTerms() {
    while (head != nullptr && head->coeff == 0) {
        PolyNode* temp = head;
        head = head->next;
        delete temp;
    }

    if (head == nullptr) return;

    PolyNode* current = head;
    while (current->next != nullptr) {
        if (current->next->coeff == 0) {
            PolyNode* temp = current->next;
            current->next = temp->next;
            delete temp;
        } else {
            current = current->next;
        }
    }
}

// Приведение подобных членов
void Polynomial::combineLikeTerms() {
    if (head == nullptr) return;

    sortByPowerDesc();

    PolyNode* current = head;
    while (current != nullptr && current->next != nullptr) {
        if (current->power == current->next->power) {
            current->coeff += current->next->coeff;

            PolyNode* temp = current->next;
            current->next = temp->next;
            delete temp;

            if (current->coeff == 0) {
                if (current == head) {
                    head = current->next;
                    delete current;
                    current = head;
                } else {
                    PolyNode* prev = head;
                    while (prev->next != current) {
                        prev = prev->next;
                    }
                    prev->next = current->next;
                    delete current;
                    current = prev;
                }
            }
        } else {
            current = current->next;
        }
    }

    removeZeroTerms();
}

// Преобразование списка в строку
std::string Polynomial::toString() const {
    using namespace std;
    if (head == nullptr) {
        return "0";
    }

    string result;
    PolyNode* current = head;
    bool first = true;

    while (current != nullptr) {
        int coeff = current->coeff;
        int power = current->power;

        if (coeff == 0) {
            current = current->next;
            continue;
        }

        if (!first) {
            if (coeff > 0) {
                result += " + ";
            } else {
                result += " - ";
                coeff = -coeff;
            }
        } else {
            if (coeff < 0) {
                result += "-";
                coeff = -coeff;
            }
        }

        if (power == 0) {
            result += to_string(coeff);
        } else if (power == 1) {
            if (coeff == 1) {
                result += variable;
            } else {
                result += to_string(coeff) + variable;
            }
        } else {
            if (coeff == 1) {
                result += string(1, variable) + "^" + to_string(power);
            } else {
                result += to_string(coeff) + variable + "^" + to_string(power);
            }
        }

        first = false;
        current = current->next;
    }

    if (result.empty()) {
        result = "0";
    }

    return result;
}

// Вывод многочлена на экран
void Polynomial::show() const {
    using namespace std;
    cout << "Многочлен: " << toString() << endl;
}

// Загрузка из файла
void Polynomial::loadFromFile(const std::string& filename) {
    using namespace std;
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileOpenException(filename);
    }

    string line;
    if (getline(file, line)) {
        cout << "Прочитано из файла: " << line << endl;
        parseFromString(line);
    } else {
        throw ParseErrorException("Файл пуст");
    }

    file.close();
}

// Дозапись в конец файла
void Polynomial::appendToFile(const std::string& filename) {
    using namespace std;
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        throw FileOpenException(filename);
    }

    file << endl << toString() << endl;
    file.close();
}
