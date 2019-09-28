#include <QMainWindow>
#include <QGraphicsScene>
#include <algorithm>
#include <math.h>

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

void flip(QImage& image, bool isHorizontal) // Função que realiza o espelhamento
{
    if(isHorizontal) // Se o espelhamento é horizontal
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
    else // Se o espelhamento é vertical
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
