#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLabel>

namespace Ui {
class ImageEditorUI;
}

class ImageEditorUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageEditorUI(QWidget *parent = nullptr);
    ~ImageEditorUI();

private slots:
    void updateImage();

    void on_openButton_pressed();

    void on_saveButton_pressed();

    void on_copyButton_pressed();

    void on_convertButton_pressed();

    void on_horizontalButton_clicked();

    void on_verticalButton_clicked();

    void on_clockButton_clicked();

    void on_anticlockButton_clicked();

    void on_flipButton_pressed();

    void on_quantizeButton_pressed();

    void on_showHistButton_pressed();

    void on_brightnessButton_pressed();

    void on_contrastButton_pressed();

    void on_eqHistButton_pressed();

    void on_matchHistButton_pressed();

    void on_zoomOutButton_pressed();

    void on_zoomInButton_pressed();

    void on_filterButton_clicked();

private:
    Ui::ImageEditorUI *ui;
    QPixmap originalImage;
    QImage  *originalImageObj = nullptr;
    QPixmap newImage;
    QImage  *newImageObj = nullptr;
    QLabel *oLabel = nullptr;
    QLabel *nLabel = nullptr;
    bool isGrayscale = false;
    short int typeOfFlip = 0;
};

#endif // MAINWINDOW_H
