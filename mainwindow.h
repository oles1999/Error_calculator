#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void mousePressEvent(QMouseEvent* event) override;

public slots:
    void CheckTestLineEdit();
    void CalculateErrors();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
