#include <iostream>
#include <string>

bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
    }
    else if (pattern == "\\d") {
        return input_line.find_first_of("1234567890") != std::string::npos;
    }
    else if (pattern == "\\w") {
        return input_line.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890") != std::string::npos;
    }
    else if (pattern.front() == '[' && pattern.at(1) == '^' && pattern.back() == ']' && pattern.length() > 3) {
        std::string extracted_string = pattern.substr(2, pattern.length() - 3);
        return input_line.find_first_of(extracted_string) == std::string::npos;
    }
    else if (pattern.front() == '[' && pattern.back() == ']' && pattern.length() > 2) {
        std::string extracted_string = pattern.substr(1, pattern.length() - 2);
        return input_line.find_first_of(extracted_string) != std::string::npos;
    }
    else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
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
         if (match_pattern(input_line, pattern)) {
             return 0;
         } else {
             return 1;
         }
     } catch (const std::runtime_error& e) {
         std::cerr << e.what() << std::endl;
         return 1;
     }
}
