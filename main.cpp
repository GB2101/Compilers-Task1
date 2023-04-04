#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <stack>

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

bool isNumber(const string& number) {
	for (char ch: number) {
		if (std::isdigit(ch) == 0) {
			return false;
		}
	}
	return true;
}

std::tuple<double, bool> Calculate(string& filename) {
	std::ifstream input(filename);
	
	if (input.fail()) {
		return {0, true};
	}

	std::stack<double> values;
	string line;
	
	int i = 0;
	while (input >> line) {
		if (isNumber(line)) {
			values.push(stod(line));
			continue;
		}

		double operand_1 = values.top();
		values.pop();

		double operand_2 = values.top();
		values.pop();

		double result;
		switch (line[0]) {
			case '+':
				result = operand_1 + operand_2;
				break;

			case '-':
				result = operand_1 - operand_2;
			
			case '*':
				result = operand_1 * operand_2;
				break;

			case '/':
				result = operand_1 / operand_2;
			
			default:
				result = 0;
				break;
		}

		values.push(result);
	}

	double final = values.top();

	return {final, false};
}

int main(int number, const char* args[]) {
	if (number != 2) {
		cout << "Invalid number of arguments, please use --help (-h) for more informations." << endl;
		return 1;
	}

	string command = args[1];
	bool calc = Display(command);

	if (calc) {
		auto[result, error] = Calculate(command);

		if (error) {
			cout << "The specified file does not exists!" << endl;
			return 1;
		}

		cout << result << endl;
	}
}