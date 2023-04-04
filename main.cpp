#include <iostream>

using std::cin, std::cout, std::endl, std::string;

bool Display(string& command) {
	if (command == "--version" || command == "-v") {
		cout << "version 1.0.0.0" << endl;
		return false;
	}
	if (command == "--help" || command == "-h") {
		cout << "calculate" << endl;
		cout << endl;
		cout << "> calculate <file>" << endl;
		cout << "> calculate --help | -h" << endl;
		cout << "> calculate --version | -v" << endl;
		return false;
	}
	return true;
}

int main(int number, const char* args[]) {
	if (number != 2) {
		cout << "Invalid number of arguments, please use --help (-h) for more informations." << endl;
		return 1;
	}

	string command = args[1];
	bool calculate = Display(command);
}