#include "rp2040_input.h"

void input_init(void) {
  uint button_pins[] = {
      INPUT_BUTTON_Y,     INPUT_BUTTON_X,     INPUT_BUTTON_A,
      INPUT_BUTTON_B,     INPUT_BUTTON_UP,    INPUT_BUTTON_DOWN,
      INPUT_BUTTON_LEFT,  INPUT_BUTTON_RIGHT, JOYSTICK_BUTTON,
      INPUT_BUTTON_START, INPUT_BUTTON_SELECT};

  for (int i = 0; i < sizeof(button_pins) / sizeof(button_pins[0]); i++) {
    gpio_init(button_pins[i]);
    gpio_set_dir(button_pins[i], GPIO_IN);
    gpio_pull_up(button_pins[i]);
  }
  adc_init();
  adc_gpio_init(JOYSTICK_X);
  adc_gpio_init(JOYSTICK_Y);
}

bool input_button_pressed(uint gpio) { return !gpio_get(gpio); }
bool input_button_released(uint gpio) { return gpio_get(gpio); }
int input_joystic_raw(uint gpio) {
  adc_select_input(gpio);
  return adc_read();
}

void input_poll(InputState *state) {
  state->up = input_button_pressed(INPUT_BUTTON_UP);
  state->down = input_button_pressed(INPUT_BUTTON_DOWN);
  state->left = input_button_pressed(INPUT_BUTTON_LEFT);
  state->right = input_button_pressed(INPUT_BUTTON_RIGHT);
  state->a = input_button_pressed(INPUT_BUTTON_A);
  state->b = input_button_pressed(INPUT_BUTTON_B);
  state->x = input_button_pressed(INPUT_BUTTON_X);
  state->y = input_button_pressed(INPUT_BUTTON_Y);
  state->start = input_button_pressed(INPUT_BUTTON_START);
  state->select = input_button_pressed(INPUT_BUTTON_SELECT);
  state->joystick_pressed = input_button_pressed(JOYSTICK_BUTTON);
  state->joystick_pressed = input_button_pressed(JOYSTICK_BUTTON);

  adc_select_input(0);
  state->joystick_x_raw = adc_read();

  adc_select_input(1);
  state->joystick_y_raw = adc_read();
}
