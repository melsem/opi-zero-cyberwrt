#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

void handle_gpio_event(const char *marker, const char *command);
void handle_encoder_movement(int value);

#endif // GPIO_HANDLER_H
