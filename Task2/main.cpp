#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <stack>

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
   public:
	string lexeme;
	TokenType type;

   public:
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

bool isNumber(const string& number) {
	for (char ch: number) {
		if (std::isdigit(ch) == 0 && ch != '.') {
			return false;
		}
	}
	return true;
}

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
	string line;

	while (input >> line) {
		if (isNumber(line)) {
			tokens.push_back({ line, TokenType::Number });
			continue;
		}

		if (line.length() > 1) {
			tokens.push_back({ line, TokenType::Unknown });
			continue;
		}

		switch (line[0]) {
			case '+':
				tokens.push_back({ line, TokenType::Plus });
				break;

			case '-':
				tokens.push_back({ line, TokenType::Minus });
				break;
			
			case '*':
				tokens.push_back({ line, TokenType::Star });
				break;

			case '/':
				tokens.push_back({ line, TokenType::Slash });
				break;
			
			default:
				tokens.push_back({ line, TokenType::Unknown });
				break;
		}
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
	if (number != 2 && number != 3) {
		std::cerr << "Error:" << endl;
		cout << "Invalid number of arguments, please use --help (-h) for more informations." << endl;
		return 1;
	}

	if (number == 3 && string(args[2]) != "--tokens") {
		cout << args[0] << endl;
		cout << args[1] << endl;
		cout << args[2] << endl;
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