#include "adminwindow.h"
#include "mainwindow.h"
#include "ImageLibrary.h"
#include "ImageDescriptor.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QApplication>
#include <QMessageBox>
#include <QGroupBox>
#include <QSplitter>
#include <QImage>
#include <QPixmap>

#include <opencv2/imgcodecs.hpp> // opencv

// fonction trouver (recopié) qui sert pour afficher une image OpenCV (cv::Mat) dans qt car
//opencv et qt n'utilise pas le mem format d'image
static QImage matToQImage(const cv::Mat& mat)
{
    if (mat.empty()) return QImage();

    if (mat.type() == CV_8UC1) {
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return img.copy();
    }

    if (mat.type() == CV_8UC3) {
        QImage img(mat.cols, mat.rows, QImage::Format_RGB888);
        for (int y = 0; y < mat.rows; ++y) {
            const cv::Vec3b* src = mat.ptr<cv::Vec3b>(y);
            uchar* dst = img.scanLine(y);
            for (int x = 0; x < mat.cols; ++x) {
                dst[3*x]     = src[x][2]; // R
                dst[3*x + 1] = src[x][1]; // G
                dst[3*x + 2] = src[x][0]; // B
            }
        }
        return img;
    }

    return QImage();
}

AdminWindow::AdminWindow(const QString& code, ImageLibrary* lib, QWidget *parent)
    : QMainWindow(parent), library(lib)
{
    user = User(code.toStdString(), UserLevel::LEVEL2); // niveau 2

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // la Barre qui est en haut
    QHBoxLayout* topBar = new QHBoxLayout();
    QPushButton* btnSwitch = new QPushButton("Changer d'utilisateur", this);
    QPushButton* btnQuit   = new QPushButton("Quitter", this);

    topBar->addWidget(btnSwitch);
    topBar->addStretch();
    topBar->addWidget(btnQuit);
    mainLayout->addLayout(topBar);

    QLabel* title = new QLabel("Bibliothèque - Admin", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:18px;font-weight:bold;");
    mainLayout->addWidget(title);

    labelInfo = new QLabel("Niveau: 2\nCode: " + code + "\n(Tout est autorisé)", this);
    labelInfo->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(labelInfo);

    // Actions Box
    QGroupBox* actionsBox = new QGroupBox("Actions Admin", this);
    QVBoxLayout* actionsLayout = new QVBoxLayout(actionsBox);

    // Formulaire qui indique les cases a ajouté
    QGridLayout* form = new QGridLayout();

    editNumero = new QLineEdit(this);
    editTitre  = new QLineEdit(this);
    editSource = new QLineEdit(this);
    editMotCle = new QLineEdit(this);
    editCout   = new QLineEdit(this);
    editAccess = new QLineEdit(this);
    editAccess->setPlaceholderText("PUBLIC ou RESTRICTED");

    form->addWidget(new QLabel("Numéro"), 0, 0);
    form->addWidget(editNumero, 0, 1);
    form->addWidget(new QLabel("Titre"), 1, 0);
    form->addWidget(editTitre, 1, 1);
    form->addWidget(new QLabel("Source (chemin)"), 2, 0);
    form->addWidget(editSource, 2, 1);
    form->addWidget(new QLabel("Mot-clé"), 3, 0);
    form->addWidget(editMotCle, 3, 1);
    form->addWidget(new QLabel("Coût"), 4, 0);
    form->addWidget(editCout, 4, 1);
    form->addWidget(new QLabel("Accès"), 5, 0);
    form->addWidget(editAccess, 5, 1);

    actionsLayout->addLayout(form);

    btnAdd = new QPushButton("Ajouter image", this);
    actionsLayout->addWidget(btnAdd);

    // ===== Suppression =====
    QHBoxLayout* delLayout = new QHBoxLayout();
    editDeleteNumero = new QLineEdit(this);
    editDeleteNumero->setPlaceholderText("Numéro à supprimer");
    btnDelete = new QPushButton("Supprimer", this);
    delLayout->addWidget(editDeleteNumero);
    delLayout->addWidget(btnDelete);
    actionsLayout->addLayout(delLayout);

    // charger et sauvegarder
    QHBoxLayout* fileLayout = new QHBoxLayout();
    editFilename = new QLineEdit(this);
    editFilename->setPlaceholderText("Nom du fichier (ex: library.txt)");
    btnSave = new QPushButton("Sauvegarder", this);
    btnLoad = new QPushButton("Charger", this);

    fileLayout->addWidget(editFilename);
    fileLayout->addWidget(btnSave);
    fileLayout->addWidget(btnLoad);
    actionsLayout->addLayout(fileLayout);

    //tri par un coup avec une box
    QHBoxLayout* costBtns = new QHBoxLayout();
    btnFree = new QPushButton("Gratuit (≤0.99)", this);
    btnMid  = new QPushButton("1 à 9.99", this);
    btnHigh = new QPushButton("≥10", this);
    costBtns->addWidget(btnFree);
    costBtns->addWidget(btnMid);
    costBtns->addWidget(btnHigh);
    actionsLayout->addLayout(costBtns);

    QHBoxLayout* custom = new QHBoxLayout();
    editMin = new QLineEdit(this);
    editMax = new QLineEdit(this);
    editMin->setPlaceholderText("min");
    editMax->setPlaceholderText("max");
    btnCustom = new QPushButton("Afficher [min,max]", this);
    custom->addWidget(editMin);
    custom->addWidget(editMax);
    custom->addWidget(btnCustom);
    actionsLayout->addLayout(custom);

    btnAll = new QPushButton("Afficher tout", this);
    actionsLayout->addWidget(btnAll);

    // Affichage image
    QHBoxLayout* showLayout = new QHBoxLayout();
    editShowNumero = new QLineEdit(this);
    editShowNumero->setPlaceholderText("Numéro à afficher");
    btnShowImage = new QPushButton("Afficher image", this);
    showLayout->addWidget(editShowNumero);
    showLayout->addWidget(btnShowImage);
    actionsLayout->addLayout(showLayout);

    // Traitements Gris et Seuil
    QHBoxLayout* traitLayout = new QHBoxLayout();
    btnGris = new QPushButton("Convertir gris", this);
    editSeuil = new QLineEdit(this);
    editSeuil->setPlaceholderText("Seuil 0..255");
    btnSeuil = new QPushButton("Seuil", this);

    traitLayout->addWidget(btnGris);
    traitLayout->addWidget(editSeuil);
    traitLayout->addWidget(btnSeuil);
    actionsLayout->addLayout(traitLayout);

    // liste et image en bas
    textList = new QTextEdit(this);
    textList->setReadOnly(true);

    imageLabel = new QLabel("Aucune image", this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMinimumSize(320, 240);
    imageLabel->setStyleSheet("border:1px solid #bbb; background:#fafafa;");

    QSplitter* bottomSplit = new QSplitter(Qt::Horizontal, this);
    bottomSplit->addWidget(textList);
    bottomSplit->addWidget(imageLabel);
    bottomSplit->setStretchFactor(0, 1);
    bottomSplit->setStretchFactor(1, 1);

    QSplitter* mainSplit = new QSplitter(Qt::Vertical, this);
    mainSplit->addWidget(actionsBox);
    mainSplit->addWidget(bottomSplit);
    mainSplit->setStretchFactor(0, 0);
    mainSplit->setStretchFactor(1, 1);
    mainSplit->setSizes({330, 650});

    mainLayout->addWidget(mainSplit);

    //Connexions avec bouton
    connect(btnAdd, &QPushButton::clicked, this, &AdminWindow::onAddImageClicked);
    connect(btnDelete, &QPushButton::clicked, this, &AdminWindow::onDeleteImageClicked);
    connect(btnSave, &QPushButton::clicked, this, &AdminWindow::onSaveClicked);
    connect(btnLoad, &QPushButton::clicked, this, &AdminWindow::onLoadClicked);

    connect(btnFree, &QPushButton::clicked, this, &AdminWindow::onShowFree);
    connect(btnMid,  &QPushButton::clicked, this, &AdminWindow::onShowMid);
    connect(btnHigh, &QPushButton::clicked, this, &AdminWindow::onShowHigh);
    connect(btnCustom, &QPushButton::clicked, this, &AdminWindow::onShowCustom);
    connect(btnAll, &QPushButton::clicked, this, &AdminWindow::onShowAll);

    connect(btnShowImage, &QPushButton::clicked, this, &AdminWindow::onShowImageClicked);

    // Traitements les 2
    connect(btnGris, &QPushButton::clicked, this, &AdminWindow::onConvertirGris);
    connect(btnSeuil, &QPushButton::clicked, this, &AdminWindow::onSeuil);

    connect(btnQuit, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(btnSwitch, &QPushButton::clicked, this, [this]() {
        MainWindow* login = new MainWindow(library);
        login->show();
        close();
    });

    // Affichage initial
    onShowAll();

    resize(1200, 900);
}

// Add /effacé/ Sauvegarder /chargééé

void AdminWindow::onAddImageClicked()
{
    bool ok1, ok2;
    int numero = editNumero->text().toInt(&ok1);
    double cout = editCout->text().toDouble(&ok2);

    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "Erreur", "Numéro ou coût invalide");
        return;
    }

    QString acc = editAccess->text().trimmed().toUpper();
    AccessLevel access;

    if (acc == "PUBLIC")
        access = AccessLevel::PUBLIC;
    else if (acc == "RESTRICTED")
        access = AccessLevel::EDITOR;
    else {
        QMessageBox::warning(this, "Erreur", "Accès doit être PUBLIC ou RESTRICTED");
        return;
    }

    ImageDescriptor img(
        numero,
        editTitre->text().toStdString(),
        editSource->text().toStdString(),
        editMotCle->text().toStdString(),
        cout,
        access
    );

    library->addImage(img);
    onShowAll();

    editNumero->clear();
    editTitre->clear();
    editSource->clear();
    editMotCle->clear();
    editCout->clear();
    editAccess->clear();
}

void AdminWindow::onDeleteImageClicked()
{
    bool ok;
    int numero = editDeleteNumero->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Erreur", "Numéro invalide");
        return;
    }

    bool removed = library->removeImage(numero, user);
    if (!removed) {
        QMessageBox::information(this, "Info", "Aucune image avec ce numéro (ou accès refusé)");
        return;
    }

    onShowAll();
    editDeleteNumero->clear();
}

void AdminWindow::onSaveClicked()
{
    QString filename = editFilename->text().trimmed();
    if (filename.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Donne un nom de fichier");
        return;
    }

    bool ok = library->saveToFile(filename.toStdString());
    if (!ok) {
        QMessageBox::critical(this, "Erreur", "Impossible de sauvegarder");
        return;
    }

    QMessageBox::information(this, "OK", "Bibliothèque sauvegardée !");
}

void AdminWindow::onLoadClicked()
{
    QString filename = editFilename->text().trimmed();
    if (filename.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Donne un nom de fichier");
        return;
    }

    bool ok = library->loadFromFile(filename.toStdString());
    if (!ok) {
        QMessageBox::critical(this, "Erreur", "Impossible de charger (fichier introuvable ?)");
        return;
    }

    onShowAll();
    QMessageBox::information(this, "OK", "Bibliothèque chargée !");
}

// Sous-listes coût

void AdminWindow::onShowAll()
{
    textList->setPlainText(QString::fromStdString(library->dumpAllAccessible(user)));
}

void AdminWindow::onShowFree()
{
    textList->setPlainText(QString::fromStdString(library->dumpByCostRange(0.0, 0.99, user)));
}

void AdminWindow::onShowMid()
{
    textList->setPlainText(QString::fromStdString(library->dumpByCostRange(1.0, 9.99, user)));
}

void AdminWindow::onShowHigh()
{
    textList->setPlainText(QString::fromStdString(library->dumpByCostRange(10.0, 1e18, user)));
}

void AdminWindow::onShowCustom()
{
    bool ok1, ok2;
    double minC = editMin->text().toDouble(&ok1);
    double maxC = editMax->text().toDouble(&ok2);

    if (!ok1 || !ok2 || minC > maxC) {
        QMessageBox::warning(this, "Erreur", "Plage invalide (min/max)");
        return;
    }

    textList->setPlainText(QString::fromStdString(library->dumpByCostRange(minC, maxC, user)));
}

// Affichage image

void AdminWindow::onShowImageClicked()
{
    bool ok;
    int numero = editShowNumero->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "Numéro invalide");
        return;
    }
    showImageByNumero(numero);
}

void AdminWindow::showImageByNumero(int numero)
{
    ImageDescriptor* desc = library->findByNumero(numero);
    if (!desc) {
        QMessageBox::information(this, "Info", "Aucune image avec ce numéro");
        return;
    }

    currentMat = cv::imread(desc->getSource(), cv::IMREAD_COLOR);
    if (currentMat.empty()) {
        QMessageBox::critical(this, "Erreur", "Impossible de charger l'image");
        return;
    }

    afficher(currentMat);
}

void AdminWindow::afficher(const cv::Mat& image)
{
    QImage qimg = matToQImage(image);
    if (qimg.isNull()) return;

    QPixmap pix = QPixmap::fromImage(qimg);
    imageLabel->setPixmap(pix.scaled(imageLabel->size(),
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation));
    imageLabel->setText("");
}

// Traitements

void AdminWindow::convertirGris(cv::Mat& image)
{
    if (image.empty()) return;

    cv::Mat gris(image.rows, image.cols, CV_8UC1);

    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);

            unsigned char valeurGris =
                static_cast<unsigned char>(
                    0.299 * pixel[2] +
                    0.587 * pixel[1] +
                    0.114 * pixel[0]
                );

            gris.at<unsigned char>(y, x) = valeurGris;
        }
    }

    image = gris;
}

void AdminWindow::seuiller(cv::Mat& image, unsigned char T)
{
    if (image.empty()) return;

    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            unsigned char pixel = image.at<unsigned char>(y, x);

            if (pixel >= T)
                image.at<unsigned char>(y, x) = 255;
            else
                image.at<unsigned char>(y, x) = 0;
        }
    }
}

void AdminWindow::onConvertirGris()
{
    if (currentMat.empty()) {
        QMessageBox::warning(this, "Erreur", "Charge une image d'abord (Afficher image)");
        return;
    }

    convertirGris(currentMat);
    afficher(currentMat);
}

void AdminWindow::onSeuil()
{
    if (currentMat.empty()) {
        QMessageBox::warning(this, "Erreur", "Charge une image d'abord (Afficher image)");
        return;
    }

    bool ok;
    int T = editSeuil->text().toInt(&ok);
    if (!ok || T < 0 || T > 255) {
        QMessageBox::warning(this, "Erreur", "Seuil invalide (0..255)");
        return;
    }

    //  seuil doit être appliqué sur image en niveaux de gris
    if (currentMat.channels() == 3)
        convertirGris(currentMat);

    seuiller(currentMat, static_cast<unsigned char>(T));
    afficher(currentMat);
}

