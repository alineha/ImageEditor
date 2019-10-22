#include <QMainWindow>
#include <QGraphicsScene>
#include <algorithm>
#include <math.h>
#include <QLabel>
#include <cmath>
#include <cstring>
#include "ImageManager.h"

uchar greyscalePixel(QRgb pixel) // Retorna a luminância do pixel
{
    return static_cast<uchar>((qRed(pixel) * 0.299 + qGreen(pixel) * 0.587 + qBlue(pixel) * 0.114));
}

void greyscale(QImage& image) // Converte todos os pixels da imagem para a escala cinza
{
    for (int row = 0; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row); // Função utilizada por questão de eficiência, retorna ponteiro para a linha "row"

        for (int column = 0; column < image.width(); column++) // Loop p/ todos os pixels da imagem
        {
            QRgb* colorPixel = reinterpret_cast<QRgb*>(scan + column*4); // Aponta colorPixel para o pixel da linha "row", coluna "column"
            uchar grey = greyscalePixel(*colorPixel);
            *colorPixel = QColor(grey, grey, grey).rgb(); // Altera o valor RGB do pixel para o desejado
        }
    }
}

void flip(QImage& image, short int typeOfFlip) // Função que realiza o espelhamento/rotação
{
    switch(typeOfFlip) // Se o espelhamento é horizontal
    {
    case 1:
        horizontalFlip(image);
        break;
    case 2:
        verticalFlip(image);
        break;
    case 3:
        rotate(image, true);
        break;
    case 4:
        rotate(image, false);
        break;
    default:
        break;
    }
}

void horizontalFlip(QImage& image)
{
    for (int row = 0; row<image.height(); row++)
    {
        uchar* scanUchar = image.scanLine(row);
        QRgb* scan = reinterpret_cast<QRgb*>(scanUchar);
        for (int column = 0; column < floor(image.width()/2); column++)
        {
              QRgb aux = (QRgb)(*(scan + column)); // Faz uma troca dos pixels um a um
              *(scan + column) = (QRgb)(*(scan + image.width() - column - 1));
              *(scan + image.width() - column - 1) = aux;
        }
    }
}

void verticalFlip(QImage& image)
{
    for (int row = 0; row<floor(image.height()/2); row++)
    {
        uchar* scan = image.scanLine(row);
        uchar* scanLow = image.scanLine(image.height() - row - 1);
        void* aux;

        aux = malloc((size_t)(4*image.width()*sizeof(uchar)));

        memcpy(aux, scan, (4*image.width()*sizeof(uchar))); // Troca linhas inteiras com memcpy
        memcpy(scan, scanLow, (4*image.width()*sizeof(uchar)));
        memcpy(scanLow, aux, (4*image.width()*sizeof(uchar)));

        free(aux);
    }
}

void rotate(QImage& image, bool isClockwise)
{
    QImage newImage = QPixmap(image.height(), image.width()).toImage();

    for (int row = 0; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 0; column < image.width(); column++)
        {
              QRgb* pixel = reinterpret_cast<QRgb*>(scan + column*4);
              if (isClockwise)
                  newImage.setPixel(image.height()-row, column, QColor(qRed(*pixel), qGreen(*pixel), qBlue(*pixel)).rgb());
              else
                  newImage.setPixel(row, image.width()-column, QColor(qRed(*pixel), qGreen(*pixel), qBlue(*pixel)).rgb());
        }
    }
    image = newImage;
}

void quantize(QImage& image, int shades) // Quantiza a imagem em escala cinza para ter um dado número de tons
{
    short int intervalSize = floor(256/shades); // Determina o tamanho dos intervalos (de 1 a 255)

    for (int row = 0; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 0; column < image.width(); column++) // Para cada pixel da imagem
        {
            QRgb* greyPixel = reinterpret_cast<QRgb*>(scan + column*4);

            int aux = floor((qRed(*greyPixel)+1)/intervalSize); // Calcula o "índice" do intervalo em que se encontrará o pixel

            uchar newShade = floor((intervalSize*aux + intervalSize*(aux+1))/2); // Determina o novo valor do pixel
            *greyPixel = QColor(newShade,newShade,newShade).rgb();

        }
    }
}

void makeHistogram(QImage& image, int isGrayScale)
{
    int numberOfPixels = image.width()*image.height();

    int histogram[256];

    if (isGrayScale)
        calculateHistogram(image, histogram, true);
    else
        calculateHistogram(image, histogram, false);

    showHistogram(numberOfPixels, histogram, QColor (0, 0, 0));
}

void calculateHistogram(QImage& image, int histogram[256], bool isGrayscale)
{

    for (int i = 0; i < 256; i++)
        histogram[i] = 0;

    for (int row = 0; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 0; column < image.width(); column++) // Loop p/ todos os pixels da imagem
        {
            QRgb* pixel = reinterpret_cast<QRgb*>(scan + column*4); // Aponta colorPixel para o pixel da linha "row", coluna "column"
            if (isGrayscale)
                histogram[(int)qRed(*pixel)]++;
            else
                histogram[greyscalePixel(*pixel)]++;
        }
    }
}

void showHistogram(int numPixels, int histogram[256], QColor color)
{
    QColor bg (255,255,255);

    float alpha = 255.0 / *std::max_element(histogram,histogram+256);

    QImage hist = QPixmap(256, 256).toImage();

    double normalizedHistogram[256] = {0};

    for (int i = 0; i < 256; i++)
        normalizedHistogram[i] = 0;

    for (int i = 0; i < 256; i++)
    {
        normalizedHistogram[i] = ceil(histogram[i]*alpha);
    }

    for (int row = 0; row < 256; row++)
    {
        uchar* scan = hist.scanLine(row);

        for (int column = 0; column < 256; column++)
        {
            QRgb* pixel = reinterpret_cast<QRgb*>(scan + column*4);
            if (normalizedHistogram[column] >= 256-row)
                *pixel = color.rgb();
            else
                *pixel = bg.rgb();
        }
    }

    QWidget* window = new QWidget();
    window->resize(258,258);
    window->setWindowTitle("Histogram");

    QLabel* label = new QLabel(window);
    label->setPixmap(QPixmap::fromImage(hist));

    window->show();
}

void negative(QImage& image)
{
    for (int row = 0; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 0; column < image.width(); column++) // Loop p/ todos os pixels da imagem
        {
            QRgb* pixel = reinterpret_cast<QRgb*>(scan + column*4);
            *pixel = QColor(255-qRed(*pixel), 255-qGreen(*pixel), 255-qBlue(*pixel)).rgb(); // Altera o valor RGB do pixel para o desejado
        }
    }
}

void adjust(QImage& image, int val, bool isBrightness)
{
    for (int row = 0; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 0; column < image.width(); column++)  // Loop p/ todos os pixels da imagem
        {
            QRgb* pixel = reinterpret_cast<QRgb*>(scan + column*4);

            int newRed, newGreen, newBlue;

            if (isBrightness)
            {
                newRed = qRed(*pixel) + val;
                newGreen = qGreen(*pixel) + val;
                newBlue = qBlue(*pixel) + val;
            }
            else
            {
                newRed = qRed(*pixel) * val;
                newGreen = qGreen(*pixel) * val;
                newBlue = qBlue(*pixel) * val;
            }

            newRed = (newRed < 0 ? 0 : (newRed > 255 ? 255 : newRed));
            newGreen = (newGreen < 0 ? 0 : (newGreen > 255 ? 255 : newGreen));
            newBlue = (newBlue < 0 ? 0 : (newBlue > 255 ? 255 : newBlue));

            *pixel = QColor((uchar)newRed,(uchar)newGreen,(uchar)newBlue).rgb();

        }
    }
}

void equalizeHistogram(QImage& image)
{
    int histogram[256];

    calculateHistogram(image, histogram, true);

    int cumulativeHistogram[256];
    memcpy(cumulativeHistogram, histogram, 256*sizeof(int));
    makeCumulative(cumulativeHistogram);

    float alpha = 255.0 / (image.height()*image.width());

    cumulativeHistogram[0] = round(alpha*(float)histogram[0]);

    for (int i = 1; i < 256; i++)
    {
        cumulativeHistogram[i] = cumulativeHistogram[i-1] + (alpha * histogram[i]);
    }

    for (int row = 0; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 0; column < image.width(); column++) // Loop p/ todos os pixels da imagem
        {
            QRgb* pixel = reinterpret_cast<QRgb*>(scan + column*4);
            uchar newShade = cumulativeHistogram[(int)qRed(*pixel)];
            *pixel = QColor(newShade, newShade, newShade).rgb(); // Altera o valor RGB do pixel para o desejado
        }
    }
}

void makeCumulative(int histogram[256])
{
    for (int i = 1; i < 256; i++)
    {
        histogram[i] += histogram[i-1];
    }
}

void equalizeColorHistogram(QImage& image)
{
    int histogram[256];

    for (int i = 0; i < 256; i++)
        histogram[i] = 0;

    calculateHistogram(image, histogram, false);

    int cumulativeHistogram[256];
    memcpy(cumulativeHistogram, histogram, 256*sizeof(int));
    makeCumulative(cumulativeHistogram);

    float alpha = 255.0 / (image.height()*image.width());

    cumulativeHistogram[0] = round(alpha*(float)histogram[0]);
    for (int i = 1; i < 256; i++)
        cumulativeHistogram[i] = cumulativeHistogram[i-1] + (alpha * histogram[i]);

    for (int row = 0; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 0; column < image.width(); column++) // Loop p/ todos os pixels da imagem
        {
            QRgb* pixel = reinterpret_cast<QRgb*>(scan + column*4);
            uchar newRed = cumulativeHistogram[(int)qRed(*pixel)];
            uchar newGreen = cumulativeHistogram[(int)qGreen(*pixel)];
            uchar newBlue = cumulativeHistogram[(int)qBlue(*pixel)];
            *pixel = QColor(newRed, newGreen, newBlue).rgb(); // Altera o valor RGB do pixel para o desejado
        }
    }

}

void histogramMatching(QImage& src, QImage& target, bool isGrayscale)
{
    int histSrc[256], histTarget[256], cumulativeHistSrc[256], cumulativeHistTarget[256];
    int HM[256];

    for (int i = 0; i < 256; i++)
    {
        histSrc[i] = 0;
        histTarget[i] = 0;
    }

    greyscale(target);

    if (!isGrayscale)
        greyscale(src);

    calculateHistogram(src, histSrc, true);
    calculateHistogram(target, histTarget, true);

    memcpy(cumulativeHistSrc, histSrc, 256*sizeof(int));
    makeCumulative(cumulativeHistSrc);

    memcpy(cumulativeHistTarget, histTarget, 256*sizeof(int));
    makeCumulative(cumulativeHistTarget);

    for (int i = 0; i < 256; i++)
        HM[i] = closestPosition(cumulativeHistSrc[i], cumulativeHistTarget, 256);

    for (int row = 0; row < src.height(); row++)
    {
        uchar* scan = src.scanLine(row);

        for (int column = 0; column < src.width(); column++) // Loop p/ todos os pixels da imagem
        {
            QRgb* pixel = reinterpret_cast<QRgb*>(scan + column*4);
            int newShade = HM[(int)qRed(*pixel)];
            newShade = (newShade < 0 ? 0 : (newShade > 255 ? 255 : newShade));
            *pixel = QColor(newShade, newShade, newShade).rgb(); // Altera o valor RGB do pixel para o desejado
        }
    }
}

int closestPosition(int src, int array[], int tam)
{
    int closest = 0;
    int diff = INT_MAX;

    for (int i = 0; i < tam; i++)
    {
        if (diff > abs(src-array[i]))
        {
            diff = abs(src-array[i]);
            closest = i;
        }
    }

    return closest;
}

void zoomOut(QImage& image, int sx, int sy)
{
    int rectangleSize = sx*sy;

    QImage zoomed = QPixmap(floor(image.width()/sx), floor(image.height()/sy)).toImage();

    for (int row = 0; row < zoomed.height()*sy; row+=sy)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 0; column < zoomed.width()*sx; column+=sx) // Loop p/ todos os pixels da imagem
        {
            int red = 0, green = 0, blue = 0;
            for (int x = 0; x < sx; x++)
            {
                for (int y = 0; y < sy; y++)
                {
                    QRgb* pixel = reinterpret_cast<QRgb*>(scan + x*(4*sizeof(uchar)*image.width()) + (column+y)*4);
                    red += qRed(*pixel);
                    green += qGreen(*pixel);
                    blue += qBlue(*pixel);
                }
            }
            red = red/rectangleSize;
            green = green/rectangleSize;
            blue = blue/rectangleSize;

            zoomed.setPixel(column/sx, row/sy, QColor(red,green,blue).rgb());
        }
    }

    image = zoomed;
}

void zoomIn(QImage& image)
{

    QImage zoomed = QPixmap(image.width()*2, image.height()*2).toImage();

    for (int row = 0; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 0; column < image.width(); column++)
        {
            QRgb* pixel = reinterpret_cast<QRgb*>(scan + column*4);

            int red = qRed(*pixel);
            int green = qGreen(*pixel);
            int blue = qBlue(*pixel);

            zoomed.setPixel(column*2, row*2, QColor(qRed(*pixel), qGreen(*pixel), qBlue(*pixel)).rgb());

            if (column+1 < image.width())
            {
                QRgb* nextPixel = reinterpret_cast<QRgb*>(scan + (column+1)*4);
                red = round((red + qRed(*nextPixel))/2);
                green = round((green + qGreen(*nextPixel))/2);
                blue = round((blue + qBlue(*nextPixel))/2);

                red = (red < 0 ? 0 : (red > 255 ? 255 : red));
                green = (green < 0 ? 0 : (green > 255 ? 255 : green));
                blue = (blue < 0 ? 0 : (blue > 255 ? 255 : blue));
            }

            zoomed.setPixel((column*2)+1,row*2, QColor(red, green, blue).rgb());
        }
    }

    for (int row = 1; (row+1) < zoomed.height(); row+=2)
    {
        for (int column = 0; column < zoomed.width(); column++)
        {

            QColor pixelAbove = zoomed.pixel(column, row-1);

            int red = pixelAbove.red();
            int green = pixelAbove.green();
            int blue = pixelAbove.blue();

            if (row + 1 < zoomed.height())
            {
                QColor pixelBelow = zoomed.pixel(column, row+1);
                red = round((red + pixelBelow.red())/2);
                green = round((green + pixelBelow.green())/2);
                blue = round((blue + pixelBelow.blue())/2);

                red = (red < 0 ? 0 : (red > 255 ? 255 : red));
                green = (green < 0 ? 0 : (green > 255 ? 255 : green));
                blue = (blue < 0 ? 0 : (blue > 255 ? 255 : blue));

            }

            zoomed.setPixel(column, row, QColor(red, green, blue).rgb());
        }
    }

    image = zoomed;
}

void convolution(QImage& image, double kernel[3][3])
{
    QImage newImage = QPixmap(image.width()-1, image.height()-1).toImage();

    for (int row = 1; row < image.height(); row++)
    {
        uchar* scan = image.scanLine(row);

        for (int column = 1; column < image.width(); column++)
        {
            int red = 0, green = 0, blue = 0;

            for(int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    QRgb* curPixel = reinterpret_cast<QRgb*>(scan + (i-1)*(4*sizeof(uchar)*image.width()) + (column+j-1)*4);
                    red += kernel[i][j]*qRed(*curPixel);
                    green += kernel[i][j]*qGreen(*curPixel);
                    blue += kernel[i][j]*qBlue(*curPixel);
                }
            }

            red = (red < 0 ? 0 : (red > 255 ? 255 : red));
            green = (green < 0 ? 0 : (green > 255 ? 255 : green));
            blue = (blue < 0 ? 0 : (blue > 255 ? 255 : blue));

            newImage.setPixel(column, row, QColor(red, green, blue).rgb());

        }
    }

    image = newImage;
}
