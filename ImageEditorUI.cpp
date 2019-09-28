#include "ImageEditorUI.h"
#include "ui_ImageEditorUI.h"
#include <QFileDialog>
#include "ImageManager.h"
#include <cstring>


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
        }
        oLabel->setPixmap(originalImage);
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
        }
        nLabel->setPixmap(newImage);
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

            break;

        case 3: //contrast

            break;

        case 4: //brightness

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
