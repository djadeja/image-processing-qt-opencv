#ifndef IMAGEFORM_H
#define IMAGEFORM_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

namespace Ui {
class ImageForm;
}

class ImageForm : public QWidget
{
    Q_OBJECT

public:
    explicit ImageForm(QWidget *parent = nullptr);
    ~ImageForm();

private:
    Ui::ImageForm *ui;
    QImage *srcImage, *destImage;
    Mat *destMat;

private:
    bool loadImage(const QString& path);
    void showImage(const QImage* const image, QLabel* label);
    void showMessage(const QString& message);
    Mat createMat(const QImage& image) const;
    bool runOpenCVOps(Mat &src, Mat& dest);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
private slots:
    void on_openFileBtn_clicked();
};

#endif // IMAGEFORM_H
