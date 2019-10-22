#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QMainWindow>
#include <QGraphicsScene>

uchar greyscalePixel(QRgb pixel);

void greyscale(QImage& image);

void flip(QImage& image, short int typeOfFlip);

void horizontalFlip(QImage& image);

void verticalFlip(QImage& image);

void rotate(QImage& image, bool isClockwise);

void quantize(QImage& image, int shades);

void makeHistogram(QImage& image, int isGrayScale);

void calculateHistogram(QImage& image, int histogram[256], bool isGrayscale);

void showHistogram(int numPixels, int histogram[256], QColor color);

void negative(QImage& image);

void adjust(QImage& image, int val, bool isBrightness);

void equalizeHistogram(QImage& image);

void makeCumulative(int histogram[256]);

void equalizeColorHistogram(QImage& image);

void histogramMatching(QImage& src, QImage& target, bool isGrayscale);

int closestPosition(int src, int array[], int tam);

void zoomOut(QImage& image, int sx, int sy);

void zoomIn(QImage& image);

void convolution(QImage& image, double kernel[3][3]);

#endif // IMAGEMANAGER_H
