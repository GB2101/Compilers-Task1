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

enum Errors {
	NotExists,
	WrongToken,
	WrongExpression
};

std::tuple<double, bool> Calculate(string& filename) {
	std::ifstream input(filename);
	
	if (input.fail()) {
		return { Errors::NotExists, true };
	}

	std::stack<double> values;
	string line;
	
	while (input >> line) {
		if (isNumber(line)) {
			values.push(stod(line));
			continue;
		}

		if (line.length() > 1) {
			return { Errors::WrongToken, true };
		}

		if (values.size() < 2) {
			return { Errors::WrongExpression, true };
		}

		double operand_2 = values.top();
		values.pop();

		double operand_1 = values.top();
		values.pop();

		double result;
		switch (line[0]) {
			case '+':
				result = operand_1 + operand_2;
				break;

			case '-':
				result = operand_1 - operand_2;
				break;
			
			case '*':
				result = operand_1 * operand_2;
				break;

			case '/':
				result = operand_1 / operand_2;
				break;
			
			default:
				return { Errors::WrongToken, true };
				break;
		}

		values.push(result);
	}

	if (values.size() > 1) {
		return { Errors::WrongExpression, true };
	}
	
	return {values.top(), false};
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
			switch (static_cast<int>(result)) {
				case Errors::NotExists:
					std::cerr << "The specified file does not exists!" << endl;
					break;

				case Errors::WrongToken:
					std::cerr << "There is a wrong line at the specified file" << endl;
					break;

				case Errors::WrongExpression:
					std::cerr << "The expression does not evaluate to a final result" << endl;
					break;
			}
			
			return 1;
		}

		cout << result << endl;
	}
}