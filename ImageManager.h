#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QMainWindow>
#include <QGraphicsScene>


uchar greyscalePixel(QRgb pixel);

void greyscale(QImage& image);

void flip(QImage& image, bool isHorizontal);

void quantize(QImage& image, int shades);

#endif // IMAGEMANAGER_H
