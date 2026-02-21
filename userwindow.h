#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include "User.h"

class QLabel;
class QTextEdit;
class QLineEdit;
class QPushButton;

class ImageLibrary;
class User;

class UserWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit UserWindow(const QString& code, ImageLibrary* lib, QWidget *parent = nullptr);

private slots:
    void onShowAll();

    // Sous-liste coût
    void onShowFree();
    void onShowMid();
    void onShowHigh();
    void onShowCustom();

    // Affichage image
    void onShowImageClicked();

private:
    void showImageByNumero(int numero);

private:
    ImageLibrary* library;
    User user;

    QTextEdit* textList;
    QLabel* imageLabel;

    // Plage coût
    QLineEdit* editMin;
    QLineEdit* editMax;
    QPushButton* btnFree;
    QPushButton* btnMid;
    QPushButton* btnHigh;
    QPushButton* btnCustom;
    QPushButton* btnAll;

    // Afficher image
    QLineEdit* editShowNumero;
    QPushButton* btnShowImage;
};

#endif
