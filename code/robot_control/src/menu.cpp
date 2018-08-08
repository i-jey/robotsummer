// #include "includes.h"

// extern uint8_t SmallFont[];
// constexpr int numMenus = 3; 
// constexpr int debounce = 100; 

// // EEPROM / menu reference 
// enum MenuItems {
//     menu_qrdThreshold,
//     menu_p,
//     menu_d, 
//     menu_gain,  
//     menu_defaultSpeed, 
//     edgeThresh, 
//     firstBridgeLowerAngle, 
//     firstBridgeUpperAngle, 
//     edgeReverseDistance, 
//     dropBridgeDistance, 
//     driveOverDistance, 
//     s3Tilt, 
//     s3Reverse, 
//     s3Drop, 
//     s3Pullback, 
//     bridgeBias, 
// };

// Menu::Menu(OLED &oled, int startBtn, int menuPlus, int menuToggle, int menuMinus, int menuPot) 
// : oled(oled)
// { 
//     this->startBtn = startBtn; 
//     this->menuPlus = menuPlus; 
//     this->menuToggle = menuToggle; 
//     this->menuMinus = menuMinus; 
//     this->menuPot = menuPot; 
// }

// void Menu::begin() { 
//     pinMode(startBtn, INPUT_PULLDOWN);
//     pinMode(menuPlus, INPUT_PULLDOWN); 
//     pinMode(menuToggle, INPUT_PULLDOWN);
//     pinMode(menuMinus, INPUT_PULLDOWN); 
//     pinMode(menuPot, INPUT); 

//     oled.begin(); 
//     oled.setFont(SmallFont); 
// }

// void Menu::pidMenu() { 

// }

// void Menu::bridgeMenu() { 
    
// }

// void Menu::postTrooperMenu() { 
    
// }

// void Menu::menuSystem() { 
//     if (digitalRead(menuToggle)) { 
//         if (chooseMenu > ) { 
//             chooseMenu = 0; 
//         }
//         else { 
//             chooseMenu++; 
//         }
//     }


// }

