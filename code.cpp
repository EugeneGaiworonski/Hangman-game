#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <windows.h>
 
std::string encode(std::string toCode, const char key)
{
    std::string result = "";
    for (int i = 0; i< toCode.size(); i++)
    result += toCode[i]^key;
    return result;
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::string Path(std::filesystem::current_path().generic_string());
	std::vector<std::string> Words;
	std::string Buffer;
	Path += "\\hangman.wrd";
	std::cout << "File: " << Path << std::endl;
	std::ifstream File;
	
	File.open(Path,std::ifstream::in);
	if (File.is_open()) 
		while(!File.eof()) {
			getline(File,Buffer);
			Words.push_back(Buffer);
		}
	else std::cout << "\nInput file error.";
	for (auto i = 0; i <Words.size();i++) {
		Words.at(i) = encode(Words.at(i),'*');
		}
	if(File.is_open()) File.close();

    std::ofstream File_;
    File_.open(Path);
    if (File_.is_open()) {
        for (auto i = 0; i < Words.size(); i++)
            File_ << Words.at(i) << '\n';
        File_.close();
    }
    else std::cout << "Output file error.";
	return 0;
}