// #ifndef MENU_H
// #define MENU_H 
// #include "OLED_I2C.h"

// class Menu { 
//     private:
//         OLED &oled; 
//         int startBtn; 
//         int menuPlus; 
//         int menuToggle; 
//         int menuMinus; 
//         int menuPot;  
//         int chooseMenu = 0; 
//         int optionState = 0; 

//         void initializeFromEEPROM(); 
//         int16_t readFromEEPROM(); 
    
//         void pidMenu(); 
//         void bridgeMenu(); 
//         void postTrooperMenu(); 
//     public: 
//         Menu(OLED &oled, int startBtn, int menuPlus, int menuToggle, int menuMinus, int menuPot); 
//         void begin(); 
//         void menuSystem(); 
// }; 

// #endif