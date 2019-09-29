#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QMainWindow>
#include <QGraphicsScene>

uchar greyscalePixel(QRgb pixel);

void greyscale(QImage& image);

void flip(QImage& image, bool isHorizontal);

void quantize(QImage& image, int shades);

void makeHistogram(QImage& image, int isGrayScale);

void calculateGrayHistogram(QImage& image, int histogram[256], bool isGrayscale);

void showHistogram(int numPixels, int histogram[256], QColor color);

void negative(QImage& image);

void adjust(QImage& image, int val, bool isBrightness);

void equalizeHistogram(QImage& image);

void makeCumulative(int histogram[256]);

void equalizeColorHistogram(QImage& image);

void histogramMatching(QImage& src, QImage& target, bool isGrayscale);

int closestPosition(int src, int array[], int tam);

void zoomOut(QImage& image, int sx, int sy);

#endif // IMAGEMANAGER_H
