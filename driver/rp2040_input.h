#ifndef RP2040_INPUT_H
#define RP2040_INPUT_H

#include <hardware/adc.h>
#include <hardware/gpio.h>

/**
 * Input low = pressed, input high = released.
 * I generally prefer not having too many interrupts in a straight-forward
 * enviroment. The interrupt based input system maybe faster but handling them
 * becomes a mess.
 */

#ifndef INPUT_BUTTON_Y
#define INPUT_BUTTON_Y 2
#endif

#ifndef INPUT_BUTTON_X
#define INPUT_BUTTON_X 3
#endif

#ifndef INPUT_BUTTON_A
#define INPUT_BUTTON_A 4
#endif

#ifndef INPUT_BUTTON_B
#define INPUT_BUTTON_B 5
#endif

#ifndef INPUT_BUTTON_UP
#define INPUT_BUTTON_UP 6
#endif

#ifndef INPUT_BUTTON_LEFT
#define INPUT_BUTTON_LEFT 7
#endif

#ifndef INPUT_BUTTON_DOWN
#define INPUT_BUTTON_DOWN 8
#endif

#ifndef INPUT_BUTTON_RIGHT
#define INPUT_BUTTON_RIGHT 9
#endif

#ifndef JOYSTICK_X
#define JOYSTICK_X 26
#endif

#ifndef JOYSTICK_Y
#define JOYSTICK_Y 27
#endif

#ifndef JOYSTICK_BUTTON
#define JOYSTICK_BUTTON 22
#endif

#ifndef INPUT_BUTTON_START
#define INPUT_BUTTON_START 14
#endif

#ifndef INPUT_BUTTON_SELECT
#define INPUT_BUTTON_SELECT 15
#endif

typedef struct {
  bool up, down, left, right;
  bool a, b, x, y;
  bool start, select;
  bool joystick_pressed;
  int joystick_x_raw;
  int joystick_y_raw;
} InputState;

/**
 * Init all defined gpios. Call this before using any input related functions.
 */
void input_init(void);

/**
 * Returns true if input is low.
 */
bool input_button_pressed(uint gpio);

/**
 * Returns true if input is high.
 */
bool input_button_released(uint gpio);

/**
 * TODO: convert raw value, which is a unint16_t to the desired x, y position.
 */
int input_joystic_raw(uint gpio);

/**
 * Polling all states of gpio then save these in a state. Example:
 *
 * InputState input;
 *
 * while (true) {
 *  input_poll (&input);
 *  if(input.a && input.b){
 *    // action when a and b are pressed
 *  }
 * }
 *
 * This is useful combo related logic are required.
 */
void input_poll(InputState *state);

#endif // !RP2040_INPUT_H
