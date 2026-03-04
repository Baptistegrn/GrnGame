#ifndef Ttf_Module_GrnGame_Arjun
#define Ttf_Module_GrnGame_Arjun // includ gaurd no error

#ifdef __cplusplus
extern "C" {
#endif

void draw_ttf(const char *font, const char *text, int size, int x, int y, int r, int g, int b,
              int a); // add trff to queue

void update_ttf(); // draw real ttf call in actualiser.c

#ifdef __cplusplus
}
#endif

#endif