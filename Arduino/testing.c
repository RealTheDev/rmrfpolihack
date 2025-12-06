#include <stdio.h>

void display_trash_icons_float(float respons_percent) {
  // You must use if-else if for floating-point comparisons in C
  if (respons_percent == 0.0) {
    printf("Trash_can_0\n");
  } else if (respons_percent == 12.5) {
    printf("Trash_can_12.5\n");
  } else if (respons_percent == 25.0) {
    printf("Trash_can_25\n");
  } else if (respons_percent == 37.5) {
    printf("Trash_can_37.5\n");
  } else if (respons_percent == 50.0) {
    printf("Trash_can_50\n");
  } else if (respons_percent == 65.5) {
    printf("Trash_can_65.5\n");
  } else if (respons_percent == 75.0) {
    printf("Trash_can_75\n");
  } else if (respons_percent == 87.5) {
    printf("Trash_can_87.5\n");
  } else if (respons_percent == 100.0) {
    printf("Trash_can_100\n");
  } else { // This acts as the 'default'
    printf("Unknown_trash_can\n");
  }
}

int main() {
  float respons;
  printf("Trash Can Persent: ");
  scanf("%g", &respons);
  printf("%g\n",respons);
  display_trash_icons_float(respons);
  return 0;
}
