#ifndef LIBRARYWINDOW_H
#define LIBRARYWINDOW_H

#include <QMainWindow>
#include <QString>

class QLabel;
class QTableWidget;
class QPushButton;

class LibraryWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LibraryWindow(const QString& role, const QString& code, QWidget *parent = nullptr);

private slots:
    void onLoadSample();
    void onSelectionChanged();

private:
    QString m_role;
    QString m_code;

    QTableWidget* table;
    QLabel* labelStatus;
    QPushButton* btnLoadSample;

    void setupUi();
};

#endif // LIBRARYWINDOW_H
