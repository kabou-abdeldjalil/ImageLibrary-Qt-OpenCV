#include "userwindow.h"
#include "mainwindow.h"
#include "ImageLibrary.h"
#include "ImageDescriptor.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QSplitter>
#include <QMessageBox>
#include <QApplication>
#include <QImage>
#include <QPixmap>

#include <opencv2/opencv.hpp>  // OpenCV

// QImage meme chose que pour admin transforme dans un format adapter pour opencv
static QImage matToQImage(const cv::Mat& mat)
{
    if (mat.empty()) return QImage();

    if (mat.type() == CV_8UC1) {
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return img.copy();
    }
    if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        return img.copy();
    }

    cv::Mat converted;
    mat.convertTo(converted, CV_8U);
    cv::Mat rgb;
    cv::cvtColor(converted, rgb, cv::COLOR_BGR2RGB);
    QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    return img.copy();
}

UserWindow::UserWindow(const QString& code, ImageLibrary* lib, QWidget *parent)
    : QMainWindow(parent), library(lib)
{
    user = User(code.toStdString(),UserLevel::LEVEL1); // niveau 1

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Barre haute
    QHBoxLayout* topBar = new QHBoxLayout();
    QPushButton* btnSwitch = new QPushButton("Changer d'utilisateur", this);
    QPushButton* btnQuit   = new QPushButton("Quitter", this);

    topBar->addWidget(btnSwitch);
    topBar->addStretch();
    topBar->addWidget(btnQuit);
    mainLayout->addLayout(topBar);

    connect(btnQuit, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(btnSwitch, &QPushButton::clicked, this, [this]() {
        MainWindow* login = new MainWindow(library);
        login->show();
        close();
    });

    // Titre
    QLabel* title = new QLabel("Bibliothèque - Consultation", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:18px;font-weight:bold;");
    mainLayout->addWidget(title);

    QLabel* info = new QLabel("Niveau: 1\n(Consultation seulement)", this);
    info->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(info);


    //  Sous-liste par coût

    QHBoxLayout* costBtns = new QHBoxLayout();
    btnFree = new QPushButton("Gratuit (≤0.99)", this);
    btnMid  = new QPushButton("1 à 9.99", this);
    btnHigh = new QPushButton("≥10", this);

    costBtns->addWidget(btnFree);
    costBtns->addWidget(btnMid);
    costBtns->addWidget(btnHigh);
    mainLayout->addLayout(costBtns);

    QHBoxLayout* custom = new QHBoxLayout();
    editMin = new QLineEdit(this);
    editMax = new QLineEdit(this);
    editMin->setPlaceholderText("min");
    editMax->setPlaceholderText("max");
    btnCustom = new QPushButton("Afficher [min,max]", this);

    custom->addWidget(editMin);
    custom->addWidget(editMax);
    custom->addWidget(btnCustom);
    mainLayout->addLayout(custom);

    btnAll = new QPushButton("Afficher tout", this);
    mainLayout->addWidget(btnAll);

    // Affichage image

    QHBoxLayout* showLayout = new QHBoxLayout();
    editShowNumero = new QLineEdit(this);
    editShowNumero->setPlaceholderText("Numéro à afficher");
    btnShowImage = new QPushButton("Afficher image", this);

    showLayout->addWidget(editShowNumero);
    showLayout->addWidget(btnShowImage);
    mainLayout->addLayout(showLayout);

    //  en Bas :liste + image

    textList = new QTextEdit(this);
    textList->setReadOnly(true);

    imageLabel = new QLabel("Aucune image", this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMinimumSize(320, 240);
    imageLabel->setStyleSheet("border:1px solid #bbb; background:#fafafa;");

    QSplitter* split = new QSplitter(Qt::Horizontal, this);
    split->addWidget(textList);
    split->addWidget(imageLabel);
    split->setStretchFactor(0, 1);
    split->setStretchFactor(1, 1);

    mainLayout->addWidget(split);

    // connexions avec les boutons
    connect(btnAll, &QPushButton::clicked, this, &UserWindow::onShowAll);
    connect(btnFree, &QPushButton::clicked, this, &UserWindow::onShowFree);
    connect(btnMid,  &QPushButton::clicked, this, &UserWindow::onShowMid);
    connect(btnHigh, &QPushButton::clicked, this, &UserWindow::onShowHigh);
    connect(btnCustom, &QPushButton::clicked, this, &UserWindow::onShowCustom);

    connect(btnShowImage, &QPushButton::clicked, this, &UserWindow::onShowImageClicked);

    onShowAll();
    resize(1100, 800);
}

void UserWindow::onShowAll()
{
    textList->setPlainText(QString::fromStdString(library->dumpAllAccessible(user)));
}

void UserWindow::onShowFree()
{
    textList->setPlainText(QString::fromStdString(library->dumpByCostRange(0.0, 0.99, user)));
}

void UserWindow::onShowMid()
{
    textList->setPlainText(QString::fromStdString(library->dumpByCostRange(1.0, 9.99, user)));
}

void UserWindow::onShowHigh()
{
    textList->setPlainText(QString::fromStdString(library->dumpByCostRange(10.0, 1e18, user)));
}

void UserWindow::onShowCustom()
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

void UserWindow::onShowImageClicked()
{
    bool ok;
    int numero = editShowNumero->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "Numéro invalide");
        return;
    }
    showImageByNumero(numero);
}

void UserWindow::showImageByNumero(int numero)
{
    ImageDescriptor* desc = library->findByNumero(numero);
    if (!desc) {
        QMessageBox::information(this, "Info", "Aucune image avec ce numéro");
        return;
    }

    //  il faut respecter les droits user
    if (!library->canAccess(*desc, user)) {
        QMessageBox::warning(this, "Accès refusé", "Tu n'as pas le droit d'accéder à cette image.");
        return;
    }

    std::string path = desc->getSource(); // source = chemin
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);

    if (img.empty()) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de charger l'image :\n" + QString::fromStdString(path));
        return;
    }

    QImage qimg = matToQImage(img);
    if (qimg.isNull()) {
        QMessageBox::critical(this, "Erreur", "Conversion OpenCV -> QImage échouée");
        return;
    }

    QPixmap pix = QPixmap::fromImage(qimg);
    imageLabel->setPixmap(pix.scaled(imageLabel->size(),
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation));
    imageLabel->setText("");
}
