#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>

using namespace std;

class person
{
    public:
        string name, passport, snils, card;
}; // новый тип данных

void reader(string input, vector<string> &vec)
{
    string line;

    ifstream in_file(input);

    while (getline(in_file, line)) vec.push_back(line);

    in_file.close();
}// чтение и запись из файла

string snils_generator()
{
    int sum = 0, num;
    string str = "";

    for (int i = 0; i < 9; ++i)
    {
        num = rand() % 10;

        sum += num * (9 - i);

        str += char(num + '0');

        if ((str.length() == 3) || (str.length() == 7)) str += '-';
    }

    str += ' ';

    if (sum % 100 < 10)
    {
        str += '0';
        str += char(sum % 100 + '0');
    }
    else str += to_string(sum % 100);

    return string(str);
} // генерация снилса

string passport_generator()
{
    random_device rd;
    uniform_int_distribution<int> dist(100000, 1000000);

   string str = to_string(rand() % 90 + 10) + ' ' + to_string(rand() % 90 + 10) + ' ' + to_string(dist(rd));

    return str;
} // генерация паспорта

string card_generator()
{
    string str = "<";

    int system = 1;
    // visa - 1
    // mastercard - 2

    if (system == 1) str += '4';
    else if (system == 2) str += '5';

    str = str + to_string(rand() % 900 + 100) + ' ' + to_string(rand() % 9000 + 1000) + ' ' + to_string(rand() % 9000 + 1000) + ' ' + to_string(rand() % 9000 + 1000) + '>';

    return str;
} // генерация карты

string add_null(int num)
{
    if (num < 10) return '0' + to_string(num);
    else return to_string(num);
} // корректное отображение чисел, состоящих из двух разрядов

string first_date_generator()
{
    string year = to_string(2017 + rand() % 3);

    string month = add_null(rand() % 12 + 1);

    string day;

    if (month == "02") day = add_null(rand() % 28 + 1);
    else if(month == "04" || month == "06" || month == "09" || month == "11") day = add_null(rand() % 30 + 1);
    else day = add_null(rand() % 31 + 1);

    string hour = add_null(rand() % 17 + 6);

    string minute = add_null(rand() % 4 * 15);

    string str = year + '-' + month + '-' + day + 'T' + hour + ':' + minute + "+04:00";

    return str;
} // генерация даты

string second_date_generator(string input)
{
    string str;

    int day, month, year;

    year = (input[2] - '0') * 10 + input[3] - '0';

    month = (input[5] - '0') * 10 + input[6] - '0';

    day = (input[8] - '0') * 10 + input[9] - '0';

    day += rand() % 7 + 1;

    if (month == 2)
    {
        str = "20" + add_null(year);
        if(day > 28) str += "-03-" + add_null(day % 28);
        else str += "-02-" + add_null(day);
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        str = "20" + add_null(year) + '-';
        if (day > 30) str += add_null(month + 1) + '-' + add_null(day % 30);
        else str += add_null(month) + '-' + add_null(day);
    } 
    else
    {
        if (month == 12)
        {
            if (day > 31) str = "20" + add_null(year + 1) + "-01-" + add_null(day % 31);
            else str = "20" + add_null(year) + "-12-" + add_null(day);
        }
        else if (month != 12)
        {
            str = "20" + add_null(year) + '-';
            if (day > 31) str += add_null(month + 1) + '-' + add_null(day % 31);
            else str += add_null(month) + '-' + add_null(day);
        }
    }

    str += 'T' + add_null(rand() % 17 + 6) + ':' + add_null(rand() % 4 * 15) + "+04:00";

    return str;
} // генерация второй даты

int main()
{
    setlocale(LC_ALL, "Russian");

    srand(time(NULL));

    ofstream file_out;

    string _name, _passport, _snils, first_date, second_date, price, _card;

    int dataset_size;

    //------------------------------------------

    vector<string> surname_m;
    reader("surname_m.txt", surname_m);

    vector<string> first_name_m;
    reader("first_name_m.txt", first_name_m);

    vector<string> second_name_m;
    reader("second_name_m.txt", second_name_m);

    vector<string> surname_w;
    reader("surname_w.txt", surname_w);

    vector<string> first_name_w;
    reader("first_name_w.txt", first_name_w);

    vector<string> second_name_w;
    reader("second_name_w.txt", second_name_w);

    vector<string> doctor;
    reader("doctors.txt", doctor);

    vector<string> list_of_symptoms;
    reader("symptoms.txt", list_of_symptoms);

    vector<vector<string>> symptom;

    for (string x : list_of_symptoms)
    {
        vector <string> doc_symp;

        string line;

        istringstream input;

        input.str(x);

        for (line; getline(input, line, ','); ) doc_symp.push_back(line);

        symptom.push_back(doc_symp);
    }

    vector<string> analyze;
    reader("analyzes.txt", analyze);

    //---------------------------------------------;

    file_out.open("Dataset_for_hospital.csv");

    file_out << "ФИО" << ';' << "Паспорт" << ';' << "Снилс" << ';' << "Врач" << ';' << "Симптомы" << ';' << "Анализы" << ';' << "Дата посещения" << ';' << "Дата получения" << ';' << "Стоимость" << ";" << "Карта" << endl;

    vector<person> figure;

    cout << "Введите количество строк для датасета: ";
    cin >> dataset_size;

    for (int i = 0; i < dataset_size / 5; ++i)
    {
        person id;

        // генерация имени
        if (rand() % 2 == 0) _name = surname_m[rand() % size(surname_m)] + ' ' + first_name_m[rand() % size(first_name_m)] + ' ' + second_name_m[rand() % size(second_name_m)];
        else _name = surname_w[rand() % size(surname_w)] + ' ' + first_name_w[rand() % size(first_name_w)] + ' ' + second_name_w[rand() % size(second_name_w)];

        id.name = _name;

        id.passport = passport_generator();

        id.snils = snils_generator();

        id.card = card_generator();

        figure.push_back(id);
    } // создание личностей

    for (int i = 0; i < dataset_size; ++i)
    {
        person id = figure[rand() % size(figure)];

        int t = rand() % size(doctor);
        int t1 = rand() % size(symptom[t]); // выбор случайного первого симптома

        string set_of_symptoms = symptom[t][t1];

        for (string x : symptom[t]) if (rand() % 2 == 0 && x != symptom[t][t1]) set_of_symptoms = set_of_symptoms + ',' + ' ' + x;
        // создание списка симптомов

        string tmp = analyze[rand() % size(analyze)];
        string set_of_analyzes = tmp; // запоминаем случайный первый анализ

        int t2 = rand() % 2 + 1;

        for (int j = 0; j < t2; ++j)
        {
            int tmp_num = rand() % size(analyze);
            if (analyze[tmp_num] != tmp) set_of_analyzes = set_of_analyzes + ',' + ' ' + analyze[tmp_num];
        } // создание списка анализов

        first_date = first_date_generator();

        second_date = second_date_generator(first_date);

        price = to_string((rand() % 8 + 1) * 250);

        file_out << id.name << ';' << id.passport << ';' << id.snils << ';' << doctor[t] << ';' << set_of_symptoms << ';' << set_of_analyzes << ';' << first_date << ';' << second_date << ';' << price << ';' << id.card << endl;
    }

    file_out.close();

    system("Dataset_for_hospital.csv");

    return 0;
}