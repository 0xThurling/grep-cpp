#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>


bool match_pattern(const std::string& input_line, const std::string& pattern, std::optional<std::reference_wrapper<int>> acc = std::nullopt) {
    if (acc) {
        acc->get() += 1;
    }

    if (pattern[0] == '\0') {
        return true;
    } else if (pattern.front() == '(') {
        int index = 0;
        int innerRegexIndex = 1;

        while (pattern[index] != ')') {
            index++;
        }

        while (std::isalnum(pattern[innerRegexIndex])) {
            innerRegexIndex++;
        }

        int left_acc = 0;
        int right_acc = 0;

        if (pattern[innerRegexIndex] == '|') {
            if (match_pattern(input_line, pattern.substr(1, innerRegexIndex - 1), left_acc) 
                || match_pattern(input_line, pattern.substr(innerRegexIndex + 1, index - innerRegexIndex - 1), right_acc)) {
                int further_check = left_acc > right_acc ? left_acc : right_acc;
                return match_pattern(input_line.substr(further_check - 1), pattern.substr(index + 1));
            }
        }

        return false;
    } else if (pattern[0] =='.') {
        return match_pattern(input_line.substr(1), pattern.substr(1), acc);
    } else if (pattern[0] == input_line[0] && pattern[1] == '+') {
        int count = 0;
        while (input_line[count] == pattern[0]) {
            count++;
        }
        return match_pattern(input_line.substr(count), pattern.substr(2), acc);
    } else if (pattern[1] == '?') {
        if(match_pattern(input_line.substr(1), pattern.substr(2))) {
            return true;
        } else {
            return match_pattern(input_line.substr(0), pattern.substr(2));
        }
    } else if (pattern[0] == input_line[0] && pattern.front() != '\0') {
        return match_pattern(input_line.substr(1), pattern.substr(1), acc);
    } else if (pattern[0] == '\\' && pattern[1] == 'd') {
        if (std::isdigit(input_line.front())) {
            return match_pattern(input_line.substr(1), pattern.substr(2), acc);
        }
        return false;
    }
    else if (pattern[0] == '\\' && pattern[1] == 'w') {
        if (std::isalnum(input_line.front())) {
            return match_pattern(input_line.substr(1), pattern.substr(2), acc);
        }
        return false;
    }
    else if (pattern.front() == '[' && pattern.at(1) == '^' && pattern.back() == ']' && pattern.length() > 3) {
        std::string extracted_string = pattern.substr(2, pattern.length() - 3);
        return input_line.find_first_of(extracted_string) == std::string::npos;
    }
    else if (pattern.front() == '[' && pattern.back() == ']' && pattern.length() > 2) {
        std::string extracted_string = pattern.substr(1, pattern.length() - 2);
        return input_line.find_first_of(extracted_string) != std::string::npos;
    } else {
        std::cout << "Unhandled Pattern: " << pattern << std::endl;
        return false;
    }
}

bool match(std::string &input_string, const std::string& pattern) {
    std::string temp = input_string;
    if (pattern.front() == '^') {
        for (int i = 0; i < pattern.length(); i++) {
            if (pattern[i + 1] != input_string[i]) {
                return false;
            }
        }
        return true;
    }
    if (pattern.back() == '$') {
        for (int i = 0; i < pattern.length(); i++) {
            if (pattern[pattern.length() - (2 + i)] != input_string[input_string.length() - (1 + i)]) {
                return false;
            }
        }
        return true;
    }
    do {
      if (match_pattern(temp, pattern)) {
        return true;
      } else {
        temp = temp.substr(1);
      }
    } while (temp.front() != '\0');
    return false;
}

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here" << std::endl;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string pattern = argv[2];

    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    // Uncomment this block to pass the first stage
    //
     std::string input_line;
     std::getline(std::cin, input_line);
    
     try {
         if (match(input_line, pattern)) {
             return 0;
         } else {
             return 1;
         }
     } catch (const std::runtime_error& e) {
         std::cerr << e.what() << std::endl;
         return 1;
     }
}
