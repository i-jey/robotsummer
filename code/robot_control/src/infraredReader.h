#ifndef INFRAREDREADER_H
#define INFRAREDREADER_H

class IRReader { 
    private: 
        int pin1k;  
        int pin10k; 
    public: 
        IRReader(); 
        IRReader(int pin1k, int pin10k); 
        void begin(); 
        bool read1k(); 
        bool read10k(); 
}; 

#endif