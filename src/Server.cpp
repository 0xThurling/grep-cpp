#include <algorithm>
#include <cctype>
#include <iostream>
#include <ostream>
#include <string>


bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern[0] == '\0') {
        return true;
    } else if (pattern[0] == input_line[0] && pattern[1] == '+') {
        int count = 0;
        while (input_line[count] == pattern[0]) {
            count++;
        }
        return match_pattern(input_line.substr(count), pattern.substr(2));
    } else if (pattern[1] == '?') {
        return match_pattern(input_line.substr(1), pattern.substr(2));
    } else if (pattern[0] == input_line[0] && pattern.front() != '\0') {
        return match_pattern(input_line.substr(1), pattern.substr(1));
    } else if (pattern[0] == '\\' && pattern[1] == 'd') {
        if (std::isdigit(input_line.front())) {
            return match_pattern(input_line.substr(1), pattern.substr(2));
        }
        return false;
    }
    else if (pattern[0] == '\\' && pattern[1] == 'w') {
        if (std::isalnum(input_line.front())) {
            return match_pattern(input_line.substr(1), pattern.substr(2));
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
    } else if (pattern[0] == '^') {
        return match_pattern(input_line.substr(0), pattern.substr(1));
    } else {
        std::cout << "Unhandled Pattern: " << pattern << std::endl;
        return false;
    }
}

bool match(std::string &input_string, const std::string& pattern) {
    std::string temp = input_string;
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
