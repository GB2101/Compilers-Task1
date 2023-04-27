#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <stack>
#include <regex>

using std::cin, std::cout, std::endl, std::string;

enum TokenType {
	Number,
	Minus,
	Plus,
	Slash,
	Star,
	Unknown,
};

struct Token {
	string lexeme;
	TokenType type;

	Token(string lex, TokenType t): lexeme(lex), type(t) {}

	string toString() {
		string stringType = "";
		switch (this->type) {
			case TokenType::Number:
				stringType = "NUMBER";
				break;
			case TokenType::Plus:
				stringType = "PLUS";
				break;
			case TokenType::Minus:
				stringType = "MINUS";
				break;
			case TokenType::Star:
				stringType = "STAR";
				break;
			case TokenType::Slash:
				stringType = "SLASH";
				break;
		}
		return "Token [type=" + stringType + ", lexeme=" + this->lexeme + "]";
	}
};

struct Regex {
	static bool isNumber(string token) {
		return std::regex_match(token, std::regex("[0-9]+(\\.[0-9]+)?"));
	}

	static bool isOP(string token) {
		return std::regex_match(token, std::regex("[*/+\\-]"));
	}

	static TokenType getOP(string token) {
		if (std::regex_match(token, std::regex("[+]"))) 
			return TokenType::Plus;
		
		if (std::regex_match(token, std::regex("[\\-]"))) 
			return TokenType::Minus;

		if (std::regex_match(token, std::regex("[*]"))) 
			return TokenType::Star;

		if (std::regex_match(token, std::regex("[/]"))) 
			return TokenType::Slash;

		return TokenType::Unknown;
	}
};

bool isNumber(const Token& token) {
	return token.type == TokenType::Number;
}

bool ValidateFile(string& filename) {
	std::ifstream input(filename);
	
	return !input.fail();
}

std::vector<Token> ReadFile(string& filename) {
	std::ifstream input(filename);

	std::vector<Token> tokens;
	string token;

	while (input >> token) {
		if (Regex::isNumber(token)) {
			tokens.push_back({ token, TokenType::Number });
			continue;
		}

		if (Regex::isOP(token)) {
			tokens.push_back({ token, Regex::getOP(token) });
			continue;
		}

		tokens.push_back({ token, TokenType::Unknown });
	}

	return tokens;
}

std::tuple<bool, std::vector<Token>> ValidateTokens(std::vector<Token>& tokens) {
	std::vector<Token> invalid_tokens;

	for (Token token: tokens) {
		if (token.type == TokenType::Unknown) {
			invalid_tokens.push_back(token);
		}
	}

	bool error = invalid_tokens.size() > 0;
	return { error, invalid_tokens };
}

std::tuple<double, bool> Calculate(std::vector<Token>& tokens) {
	std::stack<double> values;
		
	for (Token token: tokens) {
		if (isNumber(token)) {
			values.push(stod(token.lexeme));
			continue;
		}

		if (values.size() < 2) {
			return { 0, true };
		}

		double operand_2 = values.top();
		values.pop();

		double operand_1 = values.top();
		values.pop();

		double result;
		switch (token.type) {
			case TokenType::Plus:
				result = operand_1 + operand_2;
				break;

			case TokenType::Minus:
				result = operand_1 - operand_2;
				break;
			
			case TokenType::Star:
				result = operand_1 * operand_2;
				break;

			case TokenType::Slash:
				result = operand_1 / operand_2;
				break;
		}

		values.push(result);
	}

	if (values.size() > 1) {
		return { 0, true };
	}
	
	return { values.top(), false };
}

bool Display(string& command) {
	if (command == "--version" || command == "-v") {
		cout << "version 2.0.0" << endl;
		return false;
	}
	if (command == "--help" || command == "-h") {
		cout << "calculate" << endl;
		cout << endl;
		cout << "> calculate <file> [--tokens]" << endl;
		cout << "> calculate --help | -h" << endl;
		cout << "> calculate --version | -v" << endl;
		return false;
	}
	return true;
}

void PrintTokens(std::vector<Token>& tokens) {
	cout << endl;
	for (Token token: tokens) {
		cout << token.toString() << endl;
	}
}

int main(int number, const char* args[]) {
// int main() {
// 	int number = 3;
// 	const char* args[] = { "program", "../Inputs/input8.txt", "--tokens" };
	if (number != 2 && number != 3) {
		std::cerr << "Error:" << endl;
		cout << "Invalid number of arguments, please use --help (-h) for more informations." << endl;
		return 1;
	}

	if (number == 3 && string(args[2]) != "--tokens") {
		std::cerr << "Error:" << endl;
		cout << "Invalid argument, please use --help (-h) for more informations." << endl;
		return 1;
	}

	string command = args[1];
	bool calc = Display(command);

	if (calc) {
		bool valid = ValidateFile(command);
		
		if (!valid) {
			std::cerr << "Error:" << endl;
			std::cerr << "The specified file does not exists!" << endl;
			return 1;
		}

		auto tokens = ReadFile(command);
		auto [errorToken, invalid] = ValidateTokens(tokens);

		if (errorToken) {
			std::cerr << "Error:" << endl;
			for (Token token: invalid) {
				std::cerr << "Unexpected character: " << token.lexeme << endl;
			}
			return 1;
		} 

		auto[result, errorExp] = Calculate(tokens);

		if (errorExp) {
			std::cerr << "Error:" << endl;
			std::cerr << "The expression does not evaluate to a final result" << endl;
			
			return 1;
		}

		cout << result << endl;

		if (number == 3) {
			PrintTokens(tokens);
		}
	}
}