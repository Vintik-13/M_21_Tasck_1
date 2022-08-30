/*Задание 1. Реализация ведомости учёта
 
Что нужно сделать
Вы уже создавали программу чтения и записи ведомости. Теперь её надо
будет обобщить и дополнить использованием структур.
Формат ведомости прежний: сначала идёт имя и фамилия получателя
денежных средств, далее — дата выдачи в формате ДД.ММ.ГГГГ, затем —
сумма выплаты в рублях. Данные разделяются пробелами. В конце каждой
записи должен быть перевод строки.
Структура данных записи должна соответствовать указанному формату.
При старте программы пользователь указывает команду, которую следует
осуществить: list или add. Команда list осуществляет чтение из файла
ведомости, как и прежде, только уже в структуры данных, и отображает
их на экране.
Команда add добавляет новую запись в конец ведомости.*/

#include <iostream>
#include <string>
#include <algorithm>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

namespace constants {
    const int STR_SIZE{ 256 };
    const int DATE_LENGTH{ 11 };
    const int DAY_LENGTH{ 2 };
    const int MONTH_LENGTH{ 2 };
    const int YEAR_LENGTH{ 4 };
}

//Структура для хранения даты, она не нужна ососбо. Так попрактиковаться
struct MyDate
{
    int day{0};
    int month{0};
    int year{0};
};

//Структура строка ведомости
struct LineStatement {
    std::string name = "";
    std::string surname = "";    
    std::string salary = "";
    MyDate my_date;
};

//Функция валидации и преобразования Си строки в объект структуры MyDate
//Попрактиковался с Си строками
bool myDate(const char* date, MyDate* my_date) {

    int count{ 0 };
    for (int i = 0; i < strlen(date); i++) {
        if (*(date + i) >= '0' && *(date + i) <= '9')
            count++;
    }

    if (*(date + 2) != '.' || *(date + 5) != '.' || count != 8)
        return false;
    else {

        char* str_d = new char[constants::DAY_LENGTH + 1];
        char* str_m = new char[constants::MONTH_LENGTH + 1];
        char* str_y = new char[constants::YEAR_LENGTH + 1];
        int j{ 0 };
        for (int i = 0; i < strlen(date) + 1; i++) {

            if (i < constants::DAY_LENGTH) {
                *(str_d + j) = *(date + i);
                j++;
            }
            if (i == constants::DAY_LENGTH) {
                *(str_d + j) = '\0';
                j = 0;
            }
            if (i > constants::DAY_LENGTH && i < (constants::DAY_LENGTH + constants::MONTH_LENGTH + 1)) {
                *(str_m + j) = *(date + i);
                j++;
            }
            if (i == (constants::DAY_LENGTH + constants::MONTH_LENGTH + 1)) {
                *(str_m + j) = '\0';
                j = 0;
            }
            if (i > constants::DAY_LENGTH + constants::MONTH_LENGTH + 1 && i < strlen(date)) {
                *(str_y + j) = *(date + i);
                j++;
            }
            if (i == strlen(date)) {
                *(str_y + j) = '\0';
                j = 0;
            }
        }

        int int_d = atoi(str_d);
        int int_m = atoi(str_m);
        int int_y = atoi(str_y);
        int days_in_month[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
        if (int_y % 4 == 0)
            days_in_month[2] = 29;

        if ((int_m < 1) || (int_m > 12)) {
            delete[]str_d; delete[]str_m; delete[]str_y;
            return false;
        }
        else if ((int_d < 1) || (int_d > days_in_month[int_m])) {
            delete[]str_d; delete[]str_m; delete[]str_y;
            return false;
        }
        else {
            my_date->day = int_d;
            my_date->month = int_m;
            my_date->year = int_y;
            delete[]str_d; delete[]str_m; delete[]str_y;
            return true;
        }
    }
}

//Валидация заработной платы
bool mySalary(std::string& salary) {
    return salary.find_first_not_of("0123456789") == -1 ? true : false;
}

//Функция формирует строку ведомости для сохранения в файл
bool addEntry(LineStatement* str) {

    bool boll_date = true;
    bool bool_salary = true;
    std::string main_str;
    std::cout << "Enter an entry in the statement in the format \"name syrname xx.xx.xxxx salary\":";
    std::getline(std::cin, main_str);
    
    std::string str_date;
    std::stringstream st;
    st.str(main_str);
    int count{ 0 };    

    while (!st.eof()) {
        switch (count)
        {
        case 0:
            st >> str->name;
            count++;
            break;
        case 1:
            st >> str->surname;
            count++;
            break;
        case 2:
            st >> str->salary;
            bool_salary = mySalary(str->salary);
            count++;
            break;            
        case 3:
            st >> str_date;
            boll_date = myDate(str_date.c_str(), &str->my_date);
            count++;
            break;
        default:
            count++;
            break;
        }
    }
    return (boll_date == true && bool_salary == true && count == 4) ? true : false;
}

//Функция сохранения строки ведомости в файл
void saveStr(const LineStatement* str, const std::string path) {

    std::ofstream ofs(path, std::ios::binary | std::ios::app);

    if (!ofs.is_open()) {
        std::cout << "File opening error!";
        return;
    }
    else {

        int len = str->name.length();
        ofs.write((char*)&len, sizeof(len));
        ofs.write(str->name.c_str(), len);
        len = str->surname.length();
        ofs.write((char*)&len, sizeof(len));
        ofs.write(str->surname.c_str(), len);        
        len = str->salary.length();
        ofs.write((char*)&len, sizeof(len));
        ofs.write(str->salary.c_str(), len);
        ofs.write((char*)&str->my_date.day, sizeof(str->my_date.day));
        ofs.write((char*)&str->my_date.month, sizeof(str->my_date.month));
        ofs.write((char*)&str->my_date.year, sizeof(str->my_date.year));
    }
    ofs.close();
}

//Функция считывания строки ведомости из бинарного файла
LineStatement readStr(std::ifstream* ifs) {

    LineStatement str;
    int len = 0;
    ifs->read((char*)&len, sizeof(len));
    str.name.resize(len);
    ifs->read((char*)str.name.c_str(), len);
    ifs->read((char*)&len, sizeof(len));
    str.surname.resize(len);
    ifs->read((char*)str.surname.c_str(), len);
    ifs->read((char*)&len, sizeof(len));
    str.salary.resize(len);
    ifs->read((char*)str.salary.c_str(), len);
    ifs->read((char*)&str.my_date.day, sizeof(str.my_date.day));
    ifs->read((char*)&str.my_date.month, sizeof(str.my_date.month));
    ifs->read((char*)&str.my_date.year, sizeof(str.my_date.year));
    return str;
}

//Функция загрузки ведомости в std::vector <LineStatement>
void loadStatement(std::vector <LineStatement>& vec, const std::string path) {
    
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open()) {
        std::cout << "File opening error!";
        return;
    }
    else {
        LineStatement tmp;

        ifs.seekg(0, std::ios::end);
        int size_tmp = 0;
        int size_max = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        do
        {           
            vec.push_back(readStr(&ifs));          
            
        } while (size_max - ifs.tellg());        
    }
    ifs.close();
}

//Функция вывода ведомости в консоль
void showStatement(std::vector <LineStatement>& vec) {

    for (auto p : vec) {

        std::cout << std::setw(10) << std::left << p.name;
        std::cout << std::setw(10) << std::left << p.surname;
        std::cout << std::setw(6) << std::left << p.salary;
        std::cout << std::setw(10) << std::right << p.my_date.day << ".";
        std::cout << p.my_date.month << ".";
        std::cout << p.my_date.year << std::endl;                                                 
    }
}

int main()
{       
    LineStatement line;    
    std::vector <LineStatement> vec;
    std::string action;

    std::cout << "Enter an action (add/list): ";
    std::getline(std::cin, action);
    std::transform(action.begin(), action.end(), action.begin(), ::toupper);

    if (action == "ADD") {
        if (!addEntry(&line)) {
            std::cout << "Incorrect input date!";
        }
        else
            saveStr(&line, "statement.bin");        
    }
    else if (action == "LIST") {
        loadStatement(vec, "statement.bin");
        showStatement(vec);
    }
    else
        std::cout << "Wrong command!";    
}

