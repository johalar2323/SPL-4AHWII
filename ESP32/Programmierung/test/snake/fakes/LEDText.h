#pragma once
#define COLR_RGB 0
#define COLR_SINGLE 0
extern bool testTextEnde;
extern int testFensterY;
struct cLEDText {
  void SetFont(const unsigned char *) {}
  void Init(void *, int width, int height, int, int y) {
    assert(width == 32 && height == 8 && (y == 0 || y == 8)); testFensterY = y;
  }
  void SetText(unsigned char *, unsigned int) {}
  void SetTextColrOptions(int, int, int, int) {}
  int UpdateText() { if (!testTextEnde) return 0; testTextEnde = false; return -1; }
};
