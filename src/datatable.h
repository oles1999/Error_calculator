#ifndef DATATABLE_H
#define DATATABLE_H

#include <QTableWidget>
#include <QKeyEvent>
#include "errorsEngine.h"


class DataTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit DataTable(QWidget* parent = nullptr);
    ~DataTable();
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

#endif // DATATABLE_H
