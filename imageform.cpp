#include "imageform.h"
#include "ui_imageform.h"
#include <QImage>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QFileDialog>

ImageForm::ImageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageForm), srcImage(nullptr), destImage(nullptr), destMat(nullptr)
{
    ui->setupUi(this);

    loadImage(":/Kakuro.png");
}

ImageForm::~ImageForm()
{
    delete ui;
    delete srcImage;
    delete destImage;
    delete destMat;
}

void ImageForm::showImage(const QImage* const image, QLabel* label)
{
    if (image != nullptr && label != nullptr)
    {
        QPixmap pixmap = QPixmap::fromImage(*image);
        label->setPixmap(pixmap.scaled(label->width(), label->height(), Qt::KeepAspectRatio));
    }
}

void ImageForm::showMessage(const QString& message)
{
    QMessageBox::information(this, QGuiApplication::applicationDisplayName(),message);
}

Mat ImageForm::createMat(const QImage& image) const
{
    switch (image.format())
    {
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
            return Mat(image.height(), image.width(), CV_8UC4,
                          const_cast<uchar*>(image.bits()), static_cast<size_t>(image.bytesPerLine()));
        default:
            return Mat();
    }
}


bool ImageForm::runOpenCVOps(Mat &src, Mat& dest)
{
    try
    {
        cvtColor(src, dest, COLOR_BGRA2GRAY);
        medianBlur(dest, dest, 5);
        adaptiveThreshold(dest, dest, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
    }
    catch(const cv::Exception& e)
    {
        showMessage(tr("OpenCV Image Processing can bot be done. Error is .").arg(e.what()));
        return false;
    }

    if (dest.empty())
    {
        showMessage(tr("OpenCV Image Processing can bot be done."));
        return false;
    }
    return true;
}

bool ImageForm::loadImage(const QString& path)
{
    if (srcImage != nullptr)
    {
        delete srcImage;
    }

    // Read Image from file
    srcImage = new QImage(path);
    if (srcImage->isNull())
    {
        showMessage(tr("Can not load image %1.").arg(QDir::toNativeSeparators(path)));
        return false;
    }

    // Display source image to the UI Label
    showImage(srcImage, ui->srcImgLbl);

    // Create OpenCV's Mat objects
    Mat srcMat = createMat(*srcImage);
    if (srcMat.empty())
    {
        showMessage(tr("Can not create OpenCV Matrix object for the image %1.").arg(QDir::toNativeSeparators(path)));
        return false;
    }
    delete destMat;
    destMat = new Mat;

    // Perform OpenCV operations
    if (!runOpenCVOps(srcMat, *destMat))
    {
        return false;
    }

    // Create Destination QImage
    delete destImage;
    destImage = new QImage(destMat->data, destMat->cols, destMat->rows, static_cast<int>(destMat->step), QImage::Format_Grayscale8);
    if (destImage->isNull())
    {
        showMessage(tr("Image can not be created from the openCV matrix."));
        return false;
    }

    // Display destination image to the UI Label
    showImage(destImage, ui->destImgLbl);

    return true;
}

void ImageForm::resizeEvent(QResizeEvent *event)
{
    showImage(srcImage, ui->srcImgLbl);
    showImage(destImage, ui->destImgLbl);
}

void ImageForm::on_openFileBtn_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if (!filePath.isNull())
    {
        ui->filePathLbl->setText(filePath);
        loadImage(filePath);
    }
}

