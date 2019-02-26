#ifndef EXPORT_EXCEL_H
#define EXPORT_EXCEL_H

#include <QSqlTableModel>
#include <QString>
#include <QTableView>


class export_Excel
{
public:
    export_Excel();
public:
    static void tableToExcel(QTableView *table,QString title);

};

#endif // EXPORT_EXCEL_H
