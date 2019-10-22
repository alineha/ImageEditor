#include <QFileDialog>
#include <cstring>
#include <cmath>
#include <numeric>
#include "ImageEditorUI.h"
#include "ui_ImageEditorUI.h"
#include "ImageManager.h"


ImageEditorUI::ImageEditorUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageEditorUI)
{
    ui->setupUi(this);
}

ImageEditorUI::~ImageEditorUI()
{
    delete ui;
}

void ImageEditorUI::updateImage()
{
    newImage = QPixmap::fromImage(*newImageObj);
    nLabel->setPixmap(newImage);
    nLabel->resize(newImageObj->width(),newImageObj->height());
    nLabel->update();
}

void ImageEditorUI::on_openButton_pressed()
{
    QString imagePath = QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("JPEG (*.jpg *.jpeg)"));

    if (imagePath != nullptr)
    {
        originalImageObj = new QImage();
        originalImageObj->load(imagePath);

        originalImage = QPixmap::fromImage(*originalImageObj);

        if (oLabel == nullptr)
        {
            oLabel = new QLabel();
            oLabel->show();
            oLabel->setWindowTitle("Original Image");
        }
        oLabel->setPixmap(originalImage);
        oLabel->resize(originalImageObj->width(),originalImageObj->height());
        oLabel->update();
    }
}

void ImageEditorUI::on_saveButton_pressed()
{
    if (newImageObj != nullptr)
    {
        QString newImagePath = QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("JPEG (*.jpg *.jpeg)" ));

        if (newImagePath != nullptr)
        {
            *newImageObj = newImage.toImage();
            newImageObj->save(newImagePath, "JPG");
            newImageObj->save("imagem.jpg");
        }
    }
}

void ImageEditorUI::on_copyButton_pressed()
{
    if(originalImageObj!=nullptr)
    {
        newImageObj = new QImage();
        *newImageObj = (*originalImageObj).copy();
        newImage = QPixmap::fromImage(*newImageObj);
        if (nLabel == nullptr)
        {
            nLabel = new QLabel();
            nLabel->show();
            nLabel->setWindowTitle("Edited Image");
        }
        updateImage();
        isGrayscale = false;
    }

}

void ImageEditorUI::on_convertButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        switch(ui->comboBox->currentIndex())
        {
        case 1: //greyscale
            greyscale(*newImageObj);
            isGrayscale = true;
            break;

        case 2: //negative
            negative(*newImageObj);
            break;

        default:

            break;

        }

        updateImage();
    }
}

void ImageEditorUI::on_horizontalButton_clicked()
{
    typeOfFlip = 1;
}

void ImageEditorUI::on_verticalButton_clicked()
{
    typeOfFlip = 2;
}

void ImageEditorUI::on_clockButton_clicked()
{
    typeOfFlip = 3;
}

void ImageEditorUI::on_anticlockButton_clicked()
{
    typeOfFlip = 4;
}

void ImageEditorUI::on_flipButton_pressed()
{
    if(newImageObj!=nullptr && typeOfFlip != 0)
    {  
        flip(*newImageObj, typeOfFlip);
        updateImage();
    }
}

void ImageEditorUI::on_quantizeButton_pressed()
{

    if(newImageObj!=nullptr && isGrayscale && (ui->shadesBox->value() > 0))
    {
        quantize(*newImageObj, ui->shadesBox->value());
        updateImage();
    }
}

void ImageEditorUI::on_showHistButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        makeHistogram(*newImageObj, isGrayscale);
    }
}

void ImageEditorUI::on_brightnessButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        adjust(*newImageObj, ui->brightBox->value(), true);
        updateImage();
    }
}

void ImageEditorUI::on_contrastButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        adjust(*newImageObj, ui->contrastBox->value(), false);
        updateImage();
    }
}

void ImageEditorUI::on_eqHistButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        if (isGrayscale)
            equalizeHistogram(*newImageObj);
        else
            equalizeColorHistogram(*newImageObj);
        updateImage();
    }
}

void ImageEditorUI::on_matchHistButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        QString imagePath = QFileDialog::getOpenFileName(this,tr("Select target image"),"",tr("JPEG (*.jpg *.jpeg)"));

        if (imagePath != nullptr)
        {
            QImage* target = new QImage();
            target->load(imagePath);

            histogramMatching(*newImageObj, *target, isGrayscale);
            updateImage();
            isGrayscale = true;
        }
    }
}

void ImageEditorUI::on_zoomOutButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        zoomOut(*newImageObj, ui->out1->value(), ui->out2->value());
        updateImage();
    }
}

void ImageEditorUI::on_zoomInButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        zoomIn(*newImageObj);
        updateImage();
    }
}

void ImageEditorUI::on_filterButton_clicked()
{
    if(newImageObj!=nullptr)
    {
        double kernel[3][3];

        kernel[2][0] = ui->filter1->value();
        kernel[1][0] = ui->filter2->value();
        kernel[0][0] = ui->filter3->value();
        kernel[2][1] = ui->filter4->value();
        kernel[1][1] = ui->filter5->value();
        kernel[0][1] = ui->filter6->value();
        kernel[2][2] = ui->filter7->value();
        kernel[1][2] = ui->filter8->value();
        kernel[2][2] = ui->filter9->value();

        convolution(*newImageObj, kernel);

        updateImage();
    }
}



