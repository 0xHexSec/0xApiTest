#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modules/request_io.h"

#include <QMessageBox>
#include <QFileDialog>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->methodBox->addItems({"GET", "POST", "PUT", "DELETE"});
    ui->headersTable->setColumnCount(2);
    ui->headersTable->setHorizontalHeaderLabels({"Key", "Value"});
    ui->headersTable->setRowCount(3);

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

HttpRequest MainWindow::buildRequestFromUI() const {
    HttpRequest req;
    req.url = ui->urlEdit->text().toStdString();
    req.body = ui->bodyEdit->toPlainText().toStdString();

    QString method = ui->methodBox->currentText();
    if (method == "GET") req.method = HttpMethod::GET;
    else if (method == "POST") req.method = HttpMethod::POST;
    else if (method == "PUT") req.method = HttpMethod::PUT;
    else req.method = HttpMethod::DELETE;

    for (int i = 0; i < ui->headersTable->rowCount(); ++i) {
        auto *keyItem = ui->headersTable->item(i, 0);
        auto *valItem = ui->headersTable->item(i, 1);
        if (keyItem && valItem && !keyItem->text().isEmpty())
            req.headers[keyItem->text().toStdString()] = valItem->text().toStdString();
    }

    return req;
}

void MainWindow::fillFromRequest(const HttpRequest &req) {
    ui->urlEdit->setText(QString::fromStdString(req.url));
    ui->bodyEdit->setText(QString::fromStdString(req.body));

    ui->methodBox->setCurrentText(
        req.method == HttpMethod::GET ? "GET" :
        req.method == HttpMethod::POST ? "POST" :
        req.method == HttpMethod::PUT ? "PUT" : "DELETE");

    ui->headersTable->setRowCount(0);
    int row = 0;
    for (const auto &[key, value] : req.headers) {
        ui->headersTable->insertRow(row);
        ui->headersTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(key)));
        ui->headersTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(value)));
        ++row;
    }
}

void MainWindow::onSendClicked() {
    auto req = buildRequestFromUI();
    std::stringstream ss;
    auto *oldBuf = std::cout.rdbuf(ss.rdbuf());

    send_request(req);

    std::cout.rdbuf(oldBuf);
    ui->responseView->setPlainText(QString::fromStdString(ss.str()));
}

void MainWindow::onSaveClicked() {
    QString filename = QFileDialog::getSaveFileName(this, "Request speichern", "", "*.json");
    if (!filename.isEmpty()) {
        save_request_to_file(buildRequestFromUI(), filename.toStdString());
    }
}

void MainWindow::onLoadClicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Request laden", "", "*.json");
    if (!filename.isEmpty()) {
        try {
            auto req = load_request_from_file(filename.toStdString());
            fillFromRequest(req);
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Fehler", e.what());
        }
    }
}