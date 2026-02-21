#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H
#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include "User.h"
#include <opencv2/core.hpp> // opencv

class QLabel;
class QLineEdit;
class QPushButton;
class ImageLibrary;

class AdminWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AdminWindow(const QString& code, ImageLibrary* lib, QWidget *parent = nullptr);

private slots:
    // ajouté  et supp
    void onAddImageClicked();
    void onDeleteImageClicked();

    // chargé et enregis
    void onSaveClicked();
    void onLoadClicked();

    // cout filtre
    void onShowFree();
    void onShowMid();
    void onShowHigh();
    void onShowCustom();
    void onShowAll();

    // affff mon image
    void onShowImageClicked();

    // ✅ Traitements d'image niv gris et seuil
    void onConvertirGris();
    void onSeuil();

private:
    void showImageByNumero(int numero);

    // ✅ Traitements
    void convertirGris(cv::Mat& image);
    void seuiller(cv::Mat& image, unsigned char T);

    // Affichage image
    void afficher(const cv::Mat& image);

private:
    ImageLibrary* library;
    User user;

    QLabel* labelInfo;

    // Widgets
    QTextEdit* textList;
    QLabel* imageLabel;

    // Ajout
    QLineEdit* editNumero;
    QLineEdit* editTitre;
    QLineEdit* editSource;
    QLineEdit* editMotCle;
    QLineEdit* editCout;
    QLineEdit* editAccess;
    QPushButton* btnAdd;

    // Supprimer
    QLineEdit* editDeleteNumero;
    QPushButton* btnDelete;

    // enregistré et chargé
    QLineEdit* editFilename;
    QPushButton* btnSave;
    QPushButton* btnLoad;

    // cout des filtre
    QLineEdit* editMin;
    QLineEdit* editMax;
    QPushButton* btnFree;
    QPushButton* btnMid;
    QPushButton* btnHigh;
    QPushButton* btnCustom;
    QPushButton* btnAll;

    // Affichage image
    QLineEdit* editShowNumero;
    QPushButton* btnShowImage;

    // ✅ Traitements
    QPushButton* btnGris;
    QLineEdit* editSeuil;
    QPushButton* btnSeuil;

    // ✅ Image courante chargée
    cv::Mat currentMat;
};

#endif
