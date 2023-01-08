#include <iostream>
#include <cmath>
#include "tgaimage.h"
using namespace std;

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);

void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(y, x, color);
}

float line(float x0, float y0, float x1, float y1, float x, float y)
{
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

void line_naive(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    for (float t = 0; t < 1; t += 0.01)
    {
        int x = x0 * (1.0f - t) + x1 * t;
        int y = x0 * (1.0f - t) + y1 * t;
        set_color(x, y, image, color);
    }
}

void line_midpoint(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color, bool invert)
{
    int y = y0;
    int d = line(x0, y0, x1, y1, x0 + 1, y0 + 0.5);

    int dx = (x1 - x0);
    int dy = (y1 - y0);
    int increment = 1;

    if (dy < 0)
    {
        // pravac ide od gore prema dolje
        dy = -dy;
        increment = -increment;
    }

    for (int x = x0; x <= x1; ++x)
    {
        if (invert)
        {
            set_color(x, y, image, color);
        }
        else
        {
            set_color(y, x, image, color);
        }

        if (d < 0)
        {
            y = y + increment;
            d = d + dx - dy;
        }
        else
        {
            d = d - dy;
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // 'transponiraj' duzinu ako je veci od 1
    bool invert = false;
    if (abs(x1 - x0) < abs(y1 - y0))
    {
        swap(x0, y0);
        swap(x1, y1);
        invert = true;
    }

    // zamijeni tocke ako pravac ide s desna na lijevo
    if (x1 < x0)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    // nacrtaj duzinu
    line_midpoint(x0, y0, x1, y1, image, color, invert);
}



float povrsina_trokuta(int x1, int y1, int x2, int y2, int x3, int y3) {
    return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

bool point_is_in_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) {

    float A = povrsina_trokuta(x1, y1, x2, y2, x3, y3);
    float A1 = povrsina_trokuta(x, y, x2, y2, x3, y3);
    float A2 = povrsina_trokuta(x1, y1, x, y, x3, y3);
    float A3 = povrsina_trokuta(x1, y1, x2, y2, x, y);

    return (A == A1 + A2 + A3);
}



//                          5       5       10      5      20      20
void draw_triangle_2d(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color){

    int a = x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1);
    if (a == 0) cout << "Tocke ne tvore trokut";

    draw_line(x0, y0, x1, y1, image, color);
    draw_line(x1, y1, x2, y2, image, color);
    draw_line(x2, y2, x0, y0, image, color);

    for(int x = x0; x <= x1; x++) {
        for(int y = y0; y <= y1; y++) {
            if (point_is_in_triangle(x0, y0, x1, y1, x2, y2, x, y)) {
                draw_line(x2, y2, x, y, image, color);
            }
        }
    }


}

/*
void draw_triangle_2d_gouraurd(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor c0, TGAColor c1, TGAColor c2) {
    int a = x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1);
    if (a == 0) cout << "Tocke ne tvore trokut";

    TGAColor true_color;
    int br1 = -1;
    int br2 = -1;
    int br3 = -1;

    for(int x = x0; x <= x1; x++) {
        br1++;
        for(int y = y0; y <= y1; y++) {
            br2++;
            for (int i = 0; i < 256; i++) {
                br3++;

                true_color = TGAColor((unsigned char)br1,(unsigned char)br2,(unsigned char)br3,255);

                if (point_is_in_triangle(x0, y0, x1, y1, x2, y2, x, y)) {
                    draw_line(x2, y2, x, y, image, true_color);
                }
            }
        }
    }

}
*/

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);

    // nacrtaj nekoliko duzina
    /*
    draw_line(10, 20, 180, 60, image, white);
    draw_line(180, 80, 10, 40, image, white);

    draw_line(20, 180, 140, 170, image, red);

    draw_line(80, 40, 110, 120, image, blue);
    */


    // (5,5) (100, 5) (100, 100)
    /*
    draw_line(5, 5, 100, 5, image, white);
    draw_line(100, 100, 100, 5, image, white);
    draw_line(5, 5, 100, 100, image, white);
    */

    draw_triangle_2d(120, 120, 170, 120, 145, 160, image, blue);
    draw_triangle_2d(300, 25, 500, 45, 450, 100, image, white);
    //draw_triangle_2d_gouraurd(300, 25, 500, 45, 450, 100, image, red, blue, white);



    // spremi sliku
    image.flip_vertically();
    image.write_tga_file("linije.tga");
}