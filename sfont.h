#ifndef SFONT_H
#define SFONT_H

/*************************************/
/* Second Law Games                  */
/*   ##### ##### ##### #   # #####   */
/*   #     #     #   # ##  #   #     */
/*   ##### ####  #   # # # #   #     */
/*       # #     #   # #  ##   #     */
/*   ##### #     ##### #   #   #     */
/*                            BETA 2 */
/*************************************/

class sfont
{
public:
  
  int init();
  int load(const char* filename);
  int draw(double x, double y, unsigned char character);
  int text(double x, double y, const char* text, long length = -1, short scale = 1);

private:
  unsigned char map[3][256];
  unsigned char width;
  unsigned char height;
  unsigned char cHeight;
  unsigned char spacing;
  GLuint sheet;
};

#endif
