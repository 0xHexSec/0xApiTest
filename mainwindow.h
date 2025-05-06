#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "modules/http_client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSendClicked();
    void onSaveClicked();
    void onLoadClicked();

private:
    Ui::MainWindow *ui;
    HttpRequest buildRequestFromUI() const;
    void fillFromRequest(const HttpRequest &req);
};

#endif // MAINWINDOW_H