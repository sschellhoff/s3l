#pragma once

class AutoCounter {
private:
    unsigned int startPos;
    unsigned int &currentPos;
    unsigned short &counter;
public:
    AutoCounter(unsigned int startPos, unsigned int &currentPos, unsigned short &counter) : startPos(startPos), currentPos(currentPos), counter(counter) {}
    ~AutoCounter() {
        counter += currentPos - startPos;
    }
};