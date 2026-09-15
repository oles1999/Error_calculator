#include "datatable.h"
#include <set>
#include <QClipboard>
#include <QGuiApplication>

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
        size_t nrows = this->rowCount();
        for (int row : rows){
            this->removeRow(row);
        }
        if (rows.size() == nrows){
            this->setRowCount(1);
        }
    }

    if (event->matches(QKeySequence::Paste)){
        this->PasteStr(QGuiApplication::clipboard()->text());
    }
    QModelIndex index = currentIndex();
    if (index.isValid() && !item(index.row(), index.column())) {
        setItem(index.row(), index.column(), new QTableWidgetItem());
    }
    QTableWidget::keyPressEvent(event);
}

ErrCalcEngine::StatResults DataTable::GetStatResults(double confidence, double inst_err)
{
    ErrCalcEngine::InputData inputData;
    ErrCalcEngine::StatResults res;
    inputData.data = this->GetDataVector();
    inputData.confidence = confidence;
    inputData.instErr = inst_err;
    res = ErrCalcEngine::CalculateStatResults(inputData);
    return res;
}

std::vector<double> DataTable::GetDataVector()
{
    qsizetype n = this->rowCount() - 1;
    std::vector<double> res(n);
    for (qsizetype i = 0; i < n; i++){
        res.at(i) = this->GetNum(i);
    }
    return res;
}

void DataTable::PasteStr(QString s)
{
    QVector<QString> data = this->ParseString(s);
    if (data.empty()){
        return;
    }
    qsizetype startIndex = this->currentRow();
    qsizetype reqRows = startIndex + data.size();
    if (this->rowCount() < reqRows){
        this->setRowCount(reqRows);
    }
    QTableWidgetItem* cell;
    for (qsizetype i = 0; i < data.size(); i++){
        reqRows = i + startIndex;
        cell = this->item(reqRows, 0);
        if (!cell){
            cell = new QTableWidgetItem();
            this->setItem(reqRows, 0, cell);
        }
        this->item(reqRows, 0)->setText(data.at(i));
    }

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


QVector<QString> DataTable::ParseString(QString s)
{
    s.replace(',', '.');
    QVector<QString> data = s.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    bool ok;
    for (qsizetype i = 0; i < data.size(); i++){
        data.at(i).toDouble(&ok);
        if (ok == false){
            data.removeAt(i);
        }
    }
    return data;
}
