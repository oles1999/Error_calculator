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
    void PasteStr(QString s);

public slots:
    void AddCell(int x, int y);
private:
    double GetNum(size_t row);
    QVector<QString> ParseString(QString s);

};

#endif // DATATABLE_H
