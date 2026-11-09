#pragma once
#define COLR_RGB 0
#define COLR_SINGLE 0
struct cLEDText {
  void SetFont(const unsigned char *) {}
  void Init(void *, int width, int height, int, int) { assert(width == 32 && height == 8); }
  void SetText(unsigned char *, unsigned int) {}
  void SetTextColrOptions(int, int, int, int) {}
  int UpdateText() { return 0; }
};
