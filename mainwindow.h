#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLineEdit;
class QLabel;
class QPushButton;

class ImageLibrary;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(ImageLibrary* lib, QWidget *parent = nullptr);

private slots:
    void onLoginClicked();
    void onQuitClicked();

private:
    ImageLibrary* library;

    QLineEdit* lineEditCode;
    QPushButton* btnLogin;
    QPushButton* btnQuit;
    QLabel* labelMsg;
};

#endif
