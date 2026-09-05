#ifndef DATATABLE_H
#define DATATABLE_H

#include <QTableWidget>
#include <QKeyEvent>
#include <limits>

struct StatResults
{
    double sum = std::numeric_limits<double>::quiet_NaN();
    double mean = std::numeric_limits<double>::quiet_NaN();;
    double dispersion = std::numeric_limits<double>::quiet_NaN();;
    double standartError = std::numeric_limits<double>::quiet_NaN();;
    double t_test = std::numeric_limits<double>::quiet_NaN();;
    double absoluteError = std::numeric_limits<double>::quiet_NaN();;
    size_t count = 0;
};

class DataTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit DataTable(QWidget* parent = nullptr);
    ~DataTable();
    void keyPressEvent(QKeyEvent* event) override;
    StatResults GetStatResults(double t_test, double inst_err = 0);
public slots:
    void AddCell(int x, int y);
private:
    double GetNum(size_t row);
    static double Sqr(double x);
};

#endif // DATATABLE_H
