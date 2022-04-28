// hangman.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <array> 
#include <windows.h>
#include <time.h>
#include <ctime>

class Game {
public:
    static unsigned GamesQty;
    static unsigned LooseQty;
    std::vector<std::string> Words;
    std::string Word; //загаданное слово
    std::string WordCopy; // копия загаданного слова
    std::string NamedChars; //названные буквы
    std::string GuessedChars; //угаданные буквы
    char round{ 7 }; //количество попыток
    size_t WordLength{ 0 };//длина загаданного слова 
    std::array <std::string, 7> picture;
    Game();
    bool readWords(std::string);
    void decodeWords();
    void selectWord();
    void writeWord();
    void doGame();
    void doError(char);
    void draw();
    void createPicture();
};
    //игровая статистика
unsigned Game::GamesQty = 0;
unsigned Game::LooseQty = 0;

int main() {
    char choice{ 'д' };
    unsigned StartTime{ 0 };
    unsigned EndTime{ 0 };

    system("cls");
    do {
        //начали
        StartTime = clock();
        Game game;
        //game.selectWord();//выбираем слово
        std::cout << "Слово: ";
        while (game.round) game.doGame();
        //закончили
        EndTime = clock();
        unsigned int GameTime = EndTime - StartTime;
        Game::GamesQty++;
        std::cout << "Время игры: " << (int) ((EndTime - StartTime) / 1000) << " секунд(ы)" << std::endl;
        std::cout << "Игр сыграно: " << Game::GamesQty << ", побед: " <<  Game::GamesQty-Game::LooseQty <<std::endl;
        std::cout << "Введенные буквы: " << game.NamedChars << std::endl;
        std::cout << "Cыграть ещё раз (д/н)?\n";
        std::cin >> choice;
        choice = tolower(choice);
    } while (choice == 'д');
    std::cout << "И Г Р А   О К О Н Ч Е Н А";
    std::cin.get();
    return 0;
} //main

void Game::doGame() { 
    writeWord();
    std::cout << "Угадай букву: ";
    char letter;
    std::cin >> letter;
    std::cin.ignore(32767, '\n');
    letter = toupper(letter);
    std::string::size_type pos{0};
    pos = Word.find(letter);
    if (pos == std::string::npos) doError(letter);
    else {
        GuessedChars[pos] = letter;
        Word[pos] = '*'; //угадана - затираем
        NamedChars += letter;
        --WordLength;
        std::cout << "\a!УГАДАНО!\n";
        if (!WordLength) { writeWord(); std::cout << "Поздравляю! Победа!\n\n"; round = 0; }

    }
    NamedChars += letter;
} //doGame

void Game::doError(char letter) {
    if (NamedChars.find(letter) == std::string::npos) {
        --round;
        std::cout << "\a\a\aНЕТ ТАКОЙ БУКВЫ...\n";
        switch (round) {
        case 0:
            Game::LooseQty++;
            std::cout << "Игра проиграна...\n\n" << picture[0] << "\nБыло загадано: " << WordCopy << "\n";
            return;
        case 1:
            std::cout << "ПОСЛЕДНЯЯ ПОПЫТКА!\n\n" << picture[1];
            break;
        case 2: case 3: case 4: case 5: case 6:
            draw();
            break;
        default:
            std::cout << "...";
            break;
        }
    }
    else std::cout << "Эта буква уже была названа,\nдругая буква:\n\nСлово: ";
} //doError

void Game::draw() {
    std::cout << '\n' << picture[round] << '\n';
}

void Game::writeWord() {
    for (auto letter : GuessedChars)
        std::cout << letter << ' ';                
    std::cout << "\n\n";
} //writeWord

void Game::selectWord() {
    srand(time(NULL));
    Word = Words.at(rand() % (Words.size()));
    WordLength = Word.length();
    for (auto letter : Word) {
        GuessedChars += '_';
        WordCopy += letter;
        WordCopy += ' '; }
} //selectWord

Game::Game() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    createPicture();
    Words.clear();
    std::filesystem::path pPath(std::filesystem::current_path());
    std::string Path = pPath.generic_string();
    std::replace(Path.begin(), Path.end(), '/', '\\');
    Path += "\\hangman.wrd";
    if (!readWords(Path)) {
        std::cout << "Файл " << Path << " не найден или поврежден.";
        exit(0); 
        std::cin.get();
    }
    decodeWords();//декодируем
    selectWord();//выбираем слово
} //Game

void Game::decodeWords() {
    for (int i = 0; i < Words.size(); i++) {
        std::string result = "";
        for (int j = 0; j < Words.at(i).size(); j++)
            Words.at(i).at(j) = Words.at(i).at(j) ^ '*';
    }
}

bool Game::readWords(std::string path) {
    std::ifstream file;
    std::string buffer;
    file.open(path);
    if (file.is_open()) {
        while (getline(file, buffer)) Words.push_back(buffer); 
        file.close();
        return true; }
    else return false;
} //readWords

void Game::createPicture() {
    picture[0] = "\t ===========\n"\
                 "\t  || /  |   \n"\
                 "\t  ||/   #   \n"\
                 "\t  ||   |#|  \n"\
                 "\t  ||   ||   \n"\
                 "\t _||________\n";
    picture[1] = "\t ===========\n"\
                 "\t  || /  |   \n"\
                 "\t  ||/   0   \n"\
                 "\t  ||   /#\\ \n"\
                 "\t  ||   /_\\ \n"    
                 "\t _||____П___\n";
    picture[2] = "\t ===========\n"\
                 "\t  || /  |   \n"\
                 "\t  ||/   O   \n"\
                 "\t  ||        \n"\
                 "\t  ||        \n"\
                 "\t  ||    _   \n"\
                 "\t _||____П___\n";
    picture[3] = "\t ===========\n"\
                 "\t  || /  |   \n"\
                 "\t  ||/   O   \n"\
                 "\t  ||        \n"\
                 "\t  ||        \n"\
                 "\t  ||        \n"\
                 "\t _||________\n";
    picture[4] = "\t ===========\n"\
                 "\t  || /      \n"\
                 "\t  ||/       \n"\
                 "\t  ||        \n"\
                 "\t  ||        \n"\
                 "\t  ||        \n"\
                 "\t _||________\n";
    picture[5] = "\t            \n"\
                 "\t  ||        \n"\
                 "\t  ||        \n"\
                 "\t  ||        \n"\
                 "\t  ||        \n"\
                 "\t  ||        \n"\
                 "\t _||________\n";
    picture[5] = "\t            \n"\
                 "\t            \n"\
                 "\t            \n"\
                 "\t            \n"\
                 "\t            \n"\
                 "\t            \n"\
                 "\t ___________\n";
}