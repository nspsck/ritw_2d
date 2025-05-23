#include "ws1_3_input.h"

void input_init(void) {
  static const uint button_pins[] = {
      INPUT_BUTTON_K1,   INPUT_BUTTON_K2,   INPUT_BUTTON_K3,    INPUT_BUTTON_UP,
      INPUT_BUTTON_DOWN, INPUT_BUTTON_LEFT, INPUT_BUTTON_RIGHT, JOYSTICK_BUTTON,
  };

  for (int i = 0; i < sizeof(button_pins) / sizeof(button_pins[0]); i++) {
    gpio_init(button_pins[i]);
    gpio_set_dir(button_pins[i], GPIO_IN);
    gpio_pull_up(button_pins[i]);
  }
}

bool input_button_pressed(uint gpio) { return !gpio_get(gpio); }
bool input_button_released(uint gpio) { return gpio_get(gpio); }

void input_poll(InputState *state) {
  state->up = input_button_pressed(INPUT_BUTTON_UP);
  state->down = input_button_pressed(INPUT_BUTTON_DOWN);
  state->left = input_button_pressed(INPUT_BUTTON_LEFT);
  state->right = input_button_pressed(INPUT_BUTTON_RIGHT);
  state->k1 = input_button_pressed(INPUT_BUTTON_K1);
  state->k2 = input_button_pressed(INPUT_BUTTON_K2);
  state->k3 = input_button_pressed(INPUT_BUTTON_K3);
  state->joystick_pressed = input_button_pressed(JOYSTICK_BUTTON);
}
