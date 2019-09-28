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
    void on_openButton_pressed();

    void on_saveButton_pressed();

    void on_copyButton_pressed();

    void on_convertButton_pressed();

    void on_flipButton_pressed();

    void on_quantizeButton_pressed();

private:
    Ui::ImageEditorUI *ui;
    QPixmap originalImage;
    QImage  *originalImageObj = nullptr;
    QPixmap newImage;
    QImage  *newImageObj = nullptr;
    QLabel *oLabel = nullptr;
    QLabel *nLabel = nullptr;
    bool isGrayscale = false;
};

#endif // MAINWINDOW_H
