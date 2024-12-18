#include "fileReader.h"
#include <stdexcept>

// Constructor that opens the file
FileReader::FileReader(const std::string& filePath) : file(filePath) {
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file.");
    }
}

// Reads the next character from the file and updates last position
char FileReader::readNextChar() {
    char ch;
    if (file.get(ch)) {
        lastPosition = file.tellg();
        return ch;
    }
    return 0;
}

// Moves the file pointer backward by 'steps' characters
void FileReader::goBack(int steps) {
    std::streamoff newPos = static_cast<std::streamoff>(lastPosition) - steps;
    if (newPos >= 0) {
        file.seekg(newPos);
        lastPosition = file.tellg();
    } else {
        throw std::runtime_error("Cannot go back further.");
    }
}

// Moves the file pointer forward by 'steps' characters
void FileReader::goForward(int steps) {
    std::streamoff newPos = static_cast<std::streamoff>(lastPosition) + steps;
    file.seekg(newPos);
    lastPosition = file.tellg();
}

// Checks if the end of the file has been reached
bool FileReader::isEOF() {
    return file.eof();
}

// Gets the current file pointer position
std::streampos FileReader::getPosition() const {
    return lastPosition;
}
