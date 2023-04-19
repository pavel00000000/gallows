#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <windows.h>
#include <chrono>

using namespace std;

//Создайте консольную версию игры «Виселица».
//Правила игры можно посмотреть по этой ссылке.
//Важно:
//■ Слово выбирается компьютером из списка слов.
//■ Список слов находится в файле в зашифрованном виде.
//■ По завершении игры на экран выводится статистика игры :
//• количество времени;
//• количество попыток;
//• искомое слово;
//• буквы игрока.

class WordList
{
public:
    WordList(const string& filename) : m_filename(filename) {}
    void add(const string& word)
    {
        m_words.push_back(word);
    }
    void decrypt()
    {
        for (auto& word : m_words)
        {
            for (auto& letter : word)
            {
                letter = letter ^ key;
            }
        }
    }
    const vector<string>& getWords() const
    {
        return m_words;
    }

private:
    string m_filename;
    vector<std::string> m_words;
    const char key = 5;
};

void display(int num)
{
    cout << "   ____ " << endl;
    cout << "  |    |" << endl;
    cout << "  |    ";
    if (num >= 1) {
        cout << "O";
    }
    cout << endl;
    cout << "  |   ";
    if (num >= 2)
    {
        cout << "/";
    }
    if (num >= 1)
    {
        cout << "|";
    }
    if (num >= 3)
    {
        cout << "\\";
    }
    cout << endl;
    cout << "  |   ";
    if (num >= 4)
    {
        cout << "/";
    }
    if (num >= 5)
    {
        cout << " \\";
    }
    cout << endl;
    cout << "  |" << endl;
    cout << "__|__" << endl;
}

int main()
{
    setlocale(LC_ALL, "ru");
    srand(time(nullptr));
    ofstream outputFile("encrypted.txt");
    if (!outputFile.is_open()) {
        cerr << "Не удалось открыть файл для записи" << endl;
        return 1;
    }
    
    string word1 = "ajb";
    string word2 = "hdk";
    string word3 = "mjwv`";
    outputFile << word1 << "\n" << word2 << "\n" << word3 << "\n";;
   
    outputFile.close();
    WordList wordList("encrypted.txt");
    ifstream inputFile("encrypted.txt");
    string word;
    while (inputFile >> word)
    {
        wordList.add(word);
    }
    wordList.decrypt();
    const auto& words = wordList.getWords();
    if (words.empty()) {
        cerr << "Error" << endl;
        return 1;
    }
    const auto& chosenWord = words[rand() % words.size()];
    auto begin = chrono::steady_clock::now();
    const int max = 6;
    int num = 0;
    vector<char> guesses;
    cout << "Добро пожаловать в игру «Виселица»!" << endl;
    cout << "Предупреждение в игре зашифрованы три английских слова" << endl;
    while (num < max)
    {
        display(num); 
        cout << "Угадайте букву: ";
        char letter;
        cin >> letter;

        if (find(guesses.begin(), guesses.end(), letter) != guesses.end())
        {
            cout << "Вы уже угадали эту букву, попробуйте еще раз." << endl;
            continue;
        }

        guesses.push_back(letter);

        bool d = false;
        for (char c : chosenWord)
        {
            if (c == letter)
            {
                d = true;
                break;
            }
        }

        if (d)
        {
            cout << "Верно!" << endl;
        }
        else
        {
            cout << "Не верно!" << endl;
            num++;

        }

        bool w = true;
        for (char c : chosenWord)
        {
            if (c != ' ' && find(guesses.begin(), guesses.end(), c) == guesses.end())
            {
                w = false;
                break;
            }
        }

        if (w)
        {
            cout << "Вы отгадали!" << endl;
            cout << "Вы проиграли. Загаданное слово было: " << chosenWord << endl;
            auto end = chrono::steady_clock::now();
            auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
            cout << "Игра заняла у вас: " << elapsed_ms.count() << " мс\n";
            cout << "Количество не верных попыток: " << num << endl;
            cout << "Введенные буквы: ";
            for (char c : guesses)
            {
                cout << c << " ";
            }
            cout << endl;
            return 0;


        }
    }

    display(num); 
    cout << "Вы проиграли. Загаданное слово было: " << chosenWord << endl;
    auto end = chrono::steady_clock::now();
    auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
    cout << "Игра заняла у вас: " << elapsed_ms.count() << " мс\n";
    cout << "Количество попыток: " << num << endl;
    cout << "Введенные буквы: ";
    for (char c : guesses)
    {
        cout << c << " ";
    }
    cout << endl;
    return 0;

}

