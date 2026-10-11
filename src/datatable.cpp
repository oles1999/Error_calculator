#include "datatable.h"
#include <qapplication.h>
#include <qmenu.h>
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
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DataTable::customContextMenuRequested, this, &DataTable::ShowContextMenu);
}

DataTable::~DataTable()
{

}

void DataTable::keyPressEvent(QKeyEvent* event)
{
    if (event->matches(QKeySequence::Copy)){
        QVector<QTableWidgetItem*> items = this->selectedItems();
        QApplication::clipboard()->setText(this->CopyStr(items));
        return;
    }

    if (event->matches(QKeySequence::Paste)){
        this->PasteStr(QGuiApplication::clipboard()->text());
        return;
    }

    if (event->matches(QKeySequence::Cut)){
        QVector<QTableWidgetItem*> items = this->selectedItems();
        QApplication::clipboard()->setText(this->CopyStr(items));
        this->DeleteItems(items);
        return;
    }

    if (event->key() == Qt::Key_Delete){
        QVector<QTableWidgetItem*> items = this->selectedItems();
        this->DeleteItems(items);
        return;
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

QString DataTable::CopyStr(QVector<QTableWidgetItem*> items)
{
    QString buf, itemText;
    QTableWidgetItem* item;
    bool isNum;
    for (qsizetype i = 0; i < items.size(); i++){
        item = items.at(i);
        itemText = item->text();
        itemText.toDouble(&isNum);
        if (isNum){
            buf = buf + itemText + "\n";
        }
    }
    return buf;
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

void DataTable::DeleteItems(QVector<QTableWidgetItem*> items)
{
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

void DataTable::ShowContextMenu(const QPoint& pos)
{
    QMenu contextMenu(this);
    QAction* actionCopy = contextMenu.addAction("Copy\tCtrl+C");
    QAction* actionPaste = contextMenu.addAction("Paste\tCtrl+V");
    QAction* actionCut = contextMenu.addAction("Cut\tCtrl+X");
    QAction* actionClear = contextMenu.addAction("Clear\tDelete");
    QAction* selectedAction = contextMenu.exec(viewport()->mapToGlobal(pos));

    if (selectedAction == actionCopy){
        QVector<QTableWidgetItem*> items = this->selectedItems();
        QApplication::clipboard()->setText(this->CopyStr(items));
    }

    else if (selectedAction == actionPaste){
        this->PasteStr(QApplication::clipboard()->text());
    }

    else if (selectedAction == actionCut){
        QVector<QTableWidgetItem*> items = this->selectedItems();
        QApplication::clipboard()->setText(this->CopyStr(items));
        this->DeleteItems(items);
    }

    else if (selectedAction == actionClear){
        QVector<QTableWidgetItem*> items = this->selectedItems();
        this->DeleteItems(items);
    }

    else {
        return;
    }
}
