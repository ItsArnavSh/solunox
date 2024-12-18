// FileReader.h
#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <string>

class FileReader {
public:
    // Constructor
    FileReader(const std::string& filePath);

    // Reads the next character from the file
    char readNextChar();

    // Moves the file pointer backward by 'steps' characters
    void goBack(int steps = 1);

    // Moves the file pointer forward by 'steps' characters
    void goForward(int steps = 1);

    // Checks if the end of the file is reached
    bool isEOF();

    // Gets the current file pointer position
    std::streampos getPosition() const;

private:
    std::ifstream file;
    std::streampos lastPosition = 0;
};

#endif // FILEREADER_H
