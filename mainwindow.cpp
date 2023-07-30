#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      imageForm(new ImageForm(this))
{
    resize(700, 500);
    setCentralWidget(imageForm);
}

MainWindow::~MainWindow()
{
}

