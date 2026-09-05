#include "datatable.h"
#include <set>
#include <cmath>
#include <boost/math/statistics/univariate_statistics.hpp>
#include <boost/math/statistics/t_test.hpp>
#include <numeric>

//-------------------------PUBLIC----------------------------------------------

DataTable::DataTable(QWidget* parent):
    QTableWidget(parent)
{
    this->setEditTriggers(QAbstractItemView::AnyKeyPressed |
                          QAbstractItemView::EditKeyPressed);
    this->setFocusPolicy(Qt::StrongFocus);
    connect(this, &DataTable::cellChanged, this, &DataTable::AddCell);
}

DataTable::~DataTable()
{

}

void DataTable::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete){
        QVector<QTableWidgetItem*> items = this->selectedItems();
        std::set<int, std::greater<int>> rows;
        for (qsizetype i = 0; i < items.size(); i++){
            rows.insert(items.at(i)->row());
        }
        for (int row : rows){
            this->removeRow(row);
        }
    }

    QModelIndex index = currentIndex();
    if (index.isValid() && !item(index.row(), index.column())) {
        setItem(index.row(), index.column(), new QTableWidgetItem());
    }
    QTableWidget::keyPressEvent(event);
}

StatResults DataTable::GetStatResults(double confidence, double inst_err)
{
    StatResults res;
    res.count = this->rowCount() - 1;
    if (res.count < 2){
        return res;
    }
    std::vector<double> data(res.count);
    for (size_t i = 0; i < data.size(); i++){
        data.at(i) = this->GetNum(i);
    }
    res.sum = std::accumulate(data.begin(), data.end(), 0.0);
    res.count = data.size();
    res.mean = res.sum / res.count;
    res.dispersion = boost::math::statistics::sample_variance(data);
    res.standartError = std::sqrt(res.dispersion / res.count);
    boost::math::students_t dist(static_cast<double>(res.count - 1));
    double p = 1.0 - (1.0 - confidence) / 2;
    res.t_test = boost::math::quantile(dist, p);
    res.absoluteError = res.t_test * res.standartError;
    if (inst_err != 0)
        res.absoluteError = std::hypot(res.absoluteError, inst_err);
    return res;
}

//-------------------------PUBLIC SLOTS----------------------------------------------

void DataTable::AddCell(int x, int y)
{
    QTableWidgetItem* item = this->item(x, y);
    bool ok;
    item->text().toDouble(&ok);
    if (item->text() != ""){
        if (ok){
            if (x == this->rowCount() - 1){
                this->setRowCount(this->rowCount() + 1);
                this->setCurrentCell(this->rowCount() - 1, 0);
            }
        }
        else {
            item->setText("");
        }
    }
}

//----------------------PRIVATE--------------------------------------------------
double DataTable::GetNum(size_t row)
{
    return this->item(row, 0)->text().toDouble();
}

double DataTable::Sqr(double x)
{
    return x * x;
}