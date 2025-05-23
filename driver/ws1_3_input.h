#ifndef WS1_3_INPUT_H
#define WS1_3_INPUT_H

#include <hardware/adc.h>
#include <hardware/gpio.h>

/**
 * Input low = pressed, input high = released.
 * I generally prefer not having too many interrupts in a straight-forward
 * enviroment. The interrupt based input system maybe faster but handling them
 * becomes a mess.
 */

#ifndef INPUT_BUTTON_K1
#define INPUT_BUTTON_K1 21
#endif

#ifndef INPUT_BUTTON_K2
#define INPUT_BUTTON_K2 20
#endif

#ifndef INPUT_BUTTON_K3
#define INPUT_BUTTON_K3 16
#endif

#ifndef INPUT_BUTTON_UP
#define INPUT_BUTTON_UP 6
#endif

#ifndef INPUT_BUTTON_LEFT
#define INPUT_BUTTON_LEFT 15
#endif

#ifndef INPUT_BUTTON_DOWN
#define INPUT_BUTTON_DOWN 19
#endif

#ifndef INPUT_BUTTON_RIGHT
#define INPUT_BUTTON_RIGHT 26
#endif

#ifndef JOYSTICK_BUTTON
#define JOYSTICK_BUTTON 13
#endif

typedef struct {
  bool up, down, left, right;
  bool k1, k2, k3;
  bool joystick_pressed;
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

#endif // !WS1_3_INPUT_H
