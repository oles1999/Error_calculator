#ifndef DATACOLUMN_H
#define DATACOLUMN_H

#include <QTableWidget>
#include <QKeyEvent>
#include "errorsEngine.h"


class DataColumn : public QTableWidget
{
    Q_OBJECT
public:
    explicit DataColumn(QWidget* parent = nullptr);
    ~DataColumn();
    void keyPressEvent(QKeyEvent* event) override;
    ErrCalcEngine::StatResults GetStatResults(double t_test, double inst_err = 0);
    std::vector<double> GetDataVector();
    QString CopyStr(QVector<QTableWidgetItem*> items);
    void PasteStr(QString s);
    void DeleteItems(QVector<QTableWidgetItem*> items);

public slots:
    void AddCell(int x, int y);
private:
    double GetNum(size_t row);
    QVector<QString> ParseString(QString s);
    void ShowContextMenu(const QPoint& pos);

};

#endif // DATACOLUMN_H
