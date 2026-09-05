#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->lineEditUnits, &QLineEdit::editingFinished, this, &MainWindow::CheckTestLineEdit);
    connect(ui->lineEditTtest, &QLineEdit::editingFinished, this, &MainWindow::CheckTestLineEdit);
    connect(ui->lineEditInstErr, &QLineEdit::editingFinished, this, &MainWindow::CheckTestLineEdit);
    connect(ui->pushButtonRun, &QPushButton::clicked, this, &MainWindow::CalculateErrors);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && focusWidget()){
        focusWidget()->clearFocus();
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::CheckTestLineEdit()
{
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
    bool isNum;
    QString s = lineEdit->text();
    double val = s.toDouble(&isNum);
    if (lineEdit == ui->lineEditUnits){
        if (isNum){
            lineEdit->setText("");
        }
    }
    if (lineEdit == ui->lineEditTtest){
        if (!isNum || val < 0 || val > 1){
            lineEdit->setText("0.95");
        }
    }
    if (lineEdit == ui->lineEditInstErr){
        if (!isNum || val < 0){
            lineEdit->setText("0");
        }
    }
}


void MainWindow::CalculateErrors()
{
    double confidence = ui->lineEditTtest->text().toDouble();
    double instErr = ui->lineEditInstErr->text().toDouble();
    StatResults results = ui->tableWidgetData->GetStatResults(confidence, instErr);
    QString log = QString(
                      "Сума: %1 %8\n"
                      "Дисперсія: %2 %8\n"
                      "Стандартна похибка середнього: %3 %8\n"
                      "Результат: %4 ± %5 %8\n"
                      "Коефіцієнт Стюдента: %6\n"
                      "Кількість вимірів: %7\n\n"
                      ).arg(results.sum)
                      .arg(results.dispersion)
                      .arg(results.standartError)
                      .arg(results.mean)
                      .arg(results.absoluteError)
                      .arg(results.t_test)
                      .arg(results.count)
                      .arg(ui->lineEditUnits->text());
    ui->textBrowserResults->setText(log);
}