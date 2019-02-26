#include "export_excel.h"

#include <QSqlTableModel>
#include <QTableView>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QAxObject>

export_Excel::export_Excel()
{

}

void export_Excel::tableToExcel(QTableView *table, QString title){
    QString fileName = QFileDialog::getSaveFileName(table,"保存",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    "Excel 文件(*.xlsx *.xls *.xl)");
    if(fileName != ""){
        QAxObject *excel = new QAxObject;
        if(excel->setControl("Excel.Application")){//连接Excel控件
            excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
            excel->setProperty("DisplayAlerts", false);//不显示任何警告信息

            QAxObject *workBooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workBooks->dynamicCall("Add");//新建工作本
            QAxObject *workBook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
            QAxObject *workSheet = workBook->querySubObject("Worksheets(int)", 1);

            int i, j;
            //获取列
            int colCount = table->model()->columnCount();
            //获取行
            int rowCount = table->model()->rowCount();

            QAxObject *cell, *col;
            //标题行
            cell = workSheet->querySubObject("Cells(int,int)", 1, 1);
            cell->dynamicCall("SetValue(const QString&)", title);
            cell->querySubObject("Font")->setProperty("Size",18);
            //调整行高
            workSheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
            //合并标题行
            QString cellTitle;
            cellTitle.append("A1:");
            cellTitle.append(QChar(colCount - 1 + 'A'));
            cellTitle.append(QString::number(1));
            QAxObject *range = workSheet->querySubObject("Range(const QString&)", cellTitle);
            range->setProperty("WrapText", true);
            range->setProperty("MergeCells", true);
            range->setProperty("HorizontalAlignment", -4108); //xlCenter
            range->setProperty("VerticalAlignment", -4108); //xlCenter
            //列标题
            for(i = 0; i < colCount; i++){
                QString columnName;
                columnName.append(QChar(i + 'A'));
                columnName.append(":");
                columnName.append(QChar(i + 'A'));
                col = workSheet->querySubObject("Columns(const QString &)", columnName);
                col->setProperty("ColumnWidth", table->columnWidth(i)/6);
                cell = workSheet->querySubObject("Cells(int,int)", 2, i+1);
                //获取表格头部文字信息
                columnName = table->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
                cell->dynamicCall("SetValue(const QString&)", columnName);
                cell->querySubObject("Font")->setProperty("Bold", true);
                cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                cell->setProperty("HorizontalAlignment", -4108);//xlCenter
                cell->setProperty("VerticalAlignment", -4108);//xlCenter
            }

            //数据区
            //获取表格数据
            for(i = 0;i < rowCount; i++){
                for(j = 0;j < colCount; j++){
                    QModelIndex index = table->model()->index(i,j);
                    QString strdata = table->model()->data(index).toString();
                    workSheet->querySubObject("Cells(int,int)", i+3, j+1)->dynamicCall("SetValue(const QString&)", strdata);

                }
            }

            //画框线
            QString lrange;
            lrange.append("A2:");
            lrange.append(colCount - 1 + 'A');
            lrange.append(QString::number(table->model()->rowCount() + 2));
            range = workSheet->querySubObject("Range(const QString&)", lrange);
            range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
            range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
            //调整数据区行高
            QString rowsName;
            rowsName.append("2:");
            rowsName.append(QString::number(table->model()->rowCount() + 2));
            range = workSheet->querySubObject("Range(const QString&)", rowsName);
            range->setProperty("RowHeight", 20);
            workBook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存至filename
            workBook->dynamicCall("Close()");//关闭工作簿
            excel->dynamicCall("Quit()");//关闭excel
            delete excel;
            excel = NULL;
            if(QMessageBox::question(NULL,"完成",
                                     "文件已经导出，是否现在打开？",
                                     QMessageBox::Yes |
                                     QMessageBox::No) == QMessageBox::Yes){
                QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
            }
        }
        else {
            QMessageBox::warning(NULL,"错误",
                                 "未能创建 Excel 对象，请安装 Microsoft Excel。",
                                 QMessageBox::Apply);
        }
    }
}













