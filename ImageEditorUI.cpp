#include <QFileDialog>
#include <cstring>
#include <cmath>
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
        nLabel->setPixmap(newImage);
        nLabel->resize(originalImageObj->width(),originalImageObj->height());
        nLabel->update();
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

        newImage = QPixmap::fromImage(*newImageObj);
        nLabel->setPixmap(newImage);
        nLabel->update();
    }
}

void ImageEditorUI::on_flipButton_pressed()
{

    if(newImageObj!=nullptr && (ui->horizontalButton->isChecked() || ui->verticalButton->isChecked()))
    {
        flip(*newImageObj, ui->horizontalButton->isChecked());
        newImage = QPixmap::fromImage(*newImageObj);
        nLabel->setPixmap(newImage);
        nLabel->update();
    }
}

void ImageEditorUI::on_quantizeButton_pressed()
{

    if(newImageObj!=nullptr && isGrayscale && (ui->shadesBox->value() > 0))
    {
        quantize(*newImageObj, ui->shadesBox->value());
        newImage = QPixmap::fromImage(*newImageObj);
        nLabel->setPixmap(newImage);
        nLabel->update();
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
        newImage = QPixmap::fromImage(*newImageObj);
        nLabel->setPixmap(newImage);
        nLabel->update();
    }
}

void ImageEditorUI::on_contrastButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        adjust(*newImageObj, ui->contrastBox->value(), false);
        newImage = QPixmap::fromImage(*newImageObj);
        nLabel->setPixmap(newImage);
        nLabel->update();
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
        newImage = QPixmap::fromImage(*newImageObj);
        nLabel->setPixmap(newImage);
        nLabel->update();
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
            newImage = QPixmap::fromImage(*newImageObj);
            nLabel->setPixmap(newImage);
            nLabel->update();
        }
    }
}

void ImageEditorUI::on_zoomOutButton_pressed()
{
    if(newImageObj!=nullptr)
    {
        zoomOut(*newImageObj, ui->out1->value(), ui->out2->value());
        newImage = QPixmap::fromImage(*newImageObj);
        nLabel->setPixmap(newImage);
        nLabel->resize(newImageObj->width(),newImageObj->height());
        nLabel->update();
    }
}
