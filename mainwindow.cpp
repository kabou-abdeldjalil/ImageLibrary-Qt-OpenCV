#include "mainwindow.h"
#include "userwindow.h"
#include "adminwindow.h"
#include "ImageLibrary.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QApplication>

MainWindow::MainWindow(ImageLibrary* lib, QWidget *parent)
    : QMainWindow(parent), library(lib)
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);

    QLabel* title = new QLabel("Connexion", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(title);

    lineEditCode = new QLineEdit(this);
    lineEditCode->setPlaceholderText("Ex: 99-ab-01");
    layout->addWidget(lineEditCode);

    btnLogin = new QPushButton("Se connecter", this);
    layout->addWidget(btnLogin);

    btnQuit = new QPushButton("Quitter", this);
    btnQuit->setStyleSheet("background:#c0392b; color:white; font-weight:bold; padding:6px;");
    layout->addWidget(btnQuit);

    labelMsg = new QLabel("", this);
    labelMsg->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelMsg);

    connect(btnLogin, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(btnQuit, &QPushButton::clicked, this, &MainWindow::onQuitClicked);

    // Optionnel: Entrée déclenche login
    connect(lineEditCode, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);

    setWindowTitle("ImageLibrary - Connexion");
    resize(300, 220);
}

void MainWindow::onQuitClicked()
{
    qApp->quit();
}

void MainWindow::onLoginClicked()
{
    QString code = lineEditCode->text().trimmed();

    QRegularExpression regex("^\\d{2}-[a-zA-Z]{2}-\\d{2}$");
    if (!regex.match(code).hasMatch())
    {
        labelMsg->setStyleSheet("color: red;");
        labelMsg->setText("Code invalide (ex: 99-ab-01)");
        return;
    }

    QString middle = code.mid(3, 2).toLower(); // us / ad / xx

    // Niveau 0
    if (middle != "us" && middle != "ad")
    {
        labelMsg->setStyleSheet("color: red;");
        labelMsg->setText("Niveau 0 : aucun accès autorisé");
        return;
    }

    // Niveau 1
    if (middle == "us")
    {
        auto* win = new UserWindow(code, library);
        win->show();
        close();
        return;
    }

    // Niveau 2
    if (middle == "ad")
    {
        auto* win = new AdminWindow(code, library);
        win->show();
        close();
        return;
    }
}
