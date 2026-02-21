#include "librarywindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>

LibraryWindow::LibraryWindow(const QString& role, const QString& code, QWidget *parent)
    : QMainWindow(parent), m_role(role), m_code(code)
{
    setupUi();
    setWindowTitle("Bibliotheque - " + m_role);
    resize(900, 500);
}

void LibraryWindow::setupUi()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Titre
    QLabel* title = new QLabel("Bibliothèque d'images", this);
    title->setStyleSheet("font-size: 18px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Infos
    QLabel* info = new QLabel("Role: " + m_role + " | Code: " + m_code, this);
    info->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(info);

    // Boutons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLoadSample = new QPushButton("Charger 3 images test", this);
    btnLayout->addWidget(btnLoadSample);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    // Table
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Numero", "Titre", "Mot-cle", "Cout", "Acces"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(table);

    // Status
    labelStatus = new QLabel("Sélectionne une image…", this);
    labelStatus->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(labelStatus);

    // Connexions
    connect(btnLoadSample, &QPushButton::clicked, this, &LibraryWindow::onLoadSample);
    connect(table, &QTableWidget::itemSelectionChanged, this, &LibraryWindow::onSelectionChanged);
}

void LibraryWindow::onLoadSample()
{
    table->setRowCount(0);

    struct Row { int num; QString titre; QString kw; double cout; QString acces; };
    Row rows[] = {
        {1, "Fleur", "nature", 0.0,  "PUBLIC"},
        {2, "Lena",  "portrait", 5.0, "EDITOR"},
        {3, "Mars",  "espace", 20.0, "ADMIN"}
    };

    for (const auto& r : rows) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::number(r.num)));
        table->setItem(row, 1, new QTableWidgetItem(r.titre));
        table->setItem(row, 2, new QTableWidgetItem(r.kw));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(r.cout)));
        table->setItem(row, 4, new QTableWidgetItem(r.acces));
    }

    labelStatus->setText("✅ 3 images test chargées. Clique une ligne.");
}

void LibraryWindow::onSelectionChanged()
{
    auto items = table->selectedItems();
    if (items.isEmpty()) return;

    QString num = table->item(table->currentRow(), 0)->text();
    QString titre = table->item(table->currentRow(), 1)->text();
    labelStatus->setText("Sélection: #" + num + " - " + titre);
}
