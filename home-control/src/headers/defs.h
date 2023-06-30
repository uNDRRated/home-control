#define SERIAL_BAUDRATE     9600
#define ROOM_PORT           4
#define NUM_LIVING_ROOM     20
#define NUM_KITCHEN         20
#define NUM_ROOM            NUM_LIVING_ROOM + NUM_KITCHEN
#define COLOR_ORDER         GRB
#define BRIGHTNESS          255
#define LED_TYPE            WS2811
#define DHT_TYPE            DHT11
#define DHT_PORT            5

//EEPROM ADDRESSES
#define EEPROM_ANIMATION_ADDRESS 0
#define EEPROM_COLOR_LIVING_ADDRESS (EEPROM_ANIMATION_ADDRESS + sizeof(int))
#define EEPROM_COLOR_KITCHEN_ADDRESS (EEPROM_COLOR_LIVING_ADDRESS + sizeof(CRGB))

//ANIMATIONS
#define ANIMATION_STATIC            0
#define ANIMATION_INCREMENT         1
#define ANIMATION_LIGHT_UP          2
#define ANIMATION_REVERSE_INCREMENT 3