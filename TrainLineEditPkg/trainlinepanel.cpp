#include "trainlinepanel.h"
#include "ui_trainlinepanel.h"
#include"soft_keyboard.h"
#include<QListWidget>
#include"dbservice.h"
#include<stdio.h>
#include<QTimer>
#include<QStringList>
#include"globalinfo_t.h"
#include<QColor>
#include<QTime>
#include<unistd.h>
//#include"ui_lineswin.h"
//#include"ui_detailinfowin.h"
TrainLinePanel::TrainLinePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainLinePanel)
{
    f_updataDB = true;
    setWindowFlags(Qt::FramelessWindowHint|(windowFlags() & (~Qt::WindowCloseButtonHint)));
    ui->setupUi(this);
//    keyboard = new Soft_Keyboard;

//    connect(ui->listWidget,SIGNAL(doubleClicked(QModelIndex)),keyboard,SLOT(show(QModelIndex)));
//    connect(ui->train_id_lineEdit,SIGNAL(clicked()),keyboard,SLOT(show()));
//    connect(ui->line_name_lineEdit,SIGNAL(clicked()),keyboard,SLOT(show()));


    int trainId = GlobalInfo_t::getCurrentTrainIdFromLocalFile();
    QString sql = QString("select line_name from tb_lines_info where train_id=%1").arg(trainId);
    ResultSet set = GlobalInfo_t::getInstance()->db->query(sql);
    if(set.count()!=0)
    {
        QString lineName = set[0].getPara("line_name");
        ui->currentLineLabel->setText(lineName);
    }
    initLineList();
    initTable();
    ui->stackedWidget->setCurrentIndex(1);
    ui->hideBtn1->setStyleSheet("border:none;background-color: rgba(255, 255, 255, 0);");
    ui->hideBtn2->setStyleSheet("border:none;background-color: rgba(255, 255, 255, 0);");
}
void TrainLinePanel::init()
{
}

void TrainLinePanel::initLineList()
{
    list_table_map.clear();
    for(int i = 2; i < ui->stackedWidget->count();i++)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
    ui->listWidget->clear();
    ResultSet set = GlobalInfo_t::getInstance()->db->query("select train_id,line_name  from tb_lines_info");
    for(int i = 0; i < set.count();i++)
    {
        char tmp[5];
        int i_train_id = set[i].getPara("train_id").toInt();
        sprintf(tmp,"%02d",i_train_id);
        QListWidgetItem* listItem = new QListWidgetItem(QString::fromAscii(tmp)+":"+set[i].getPara("line_name"),ui->listWidget);

        ui->listWidget->addItem(listItem);
    }

}

TrainLinePanel::~TrainLinePanel()
{
    delete ui;
}

void TrainLinePanel::on_add_line_btn_clicked()
{
    ui->line_name_lineEdit->setText("");
    ui->train_id_lineEdit->setText("");
    ui->stackedWidget->setCurrentIndex(0);

}

void TrainLinePanel::on_add_line_ok_btn_clicked()
{
    if(ui->train_id_lineEdit->text().trimmed() != "" && ui->line_name_lineEdit->text() != "")
    {
        QString sql = QString("insert into tb_lines_info (train_id,line_name) values(%1,'%2')").arg(ui->train_id_lineEdit->text()).arg(ui->line_name_lineEdit->text());
        GlobalInfo_t::getInstance()->db->exec(sql);
    }
    init();
    QTableWidget* tableWidget = list_table_map[ui->listWidget->item(ui->listWidget->count()-1)->text()];
    ui->stackedWidget->setCurrentWidget(tableWidget);
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);

}

void TrainLinePanel::on_delete_station_btn_clicked()
{
    QTableWidget* tableWidget =  dynamic_cast<QTableWidget*>(ui->stackedWidget->currentWidget());
    if(tableWidget == NULL)
        return;
    if(tableWidget->horizontalHeaderItem(0)->text() != "Station number")
        return ;
    QTableWidgetItem* currentItem = tableWidget->currentItem();
    if(currentItem == NULL)
        return;

    QString sql = QString("delete from tb_station_info where rowid=%1;").arg(tableWidget->item(currentItem->row(),9)->text());

    GlobalInfo_t::getInstance()->db->exec(sql);
    int index = ui->listWidget->currentRow();
    on_listWidget_currentRowChanged(index);

}

void TrainLinePanel::on_add_station_btn_clicked()
{
    QTableWidget* tableWidget =  dynamic_cast<QTableWidget*>(ui->stackedWidget->currentWidget());
    if(tableWidget == NULL)
        return;
    if(tableWidget->horizontalHeaderItem(0)->text() != "Station number")
        return ;
    QListWidgetItem* item = ui->listWidget->currentItem();
    int index = ui->listWidget->currentRow();
    if(item == NULL)
        return;
    QStringList strList = item->text().split(QChar(':'));
    QString sql = QString("insert into tb_station_info (train_id,indexNu) values(%1,(select indexNu from tb_station_info where train_id=%1 order by indexNu DESC limit 1 )+1);").arg(strList[0]);
    GlobalInfo_t::getInstance()->db->exec(sql,false);
    on_listWidget_currentRowChanged(index);


}



void TrainLinePanel::show()
{

    //ui->stackedWidget->setCurrentIndex(1);
    //init();
    QWidget::show();
}
void TrainLinePanel::hide()
{
    QWidget::hide();
}
void TrainLinePanel::on_ok_btn_clicked()
{
    hide();
}


void TrainLinePanel::on_setCurLineBtn_clicked()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(item == NULL)
        return;
    QStringList strList = item->text().split(QChar(':'));
    ui->currentLineLabel->setText(strList[1]);
    int newTrainId = strList[0].toInt();
    if(GlobalInfo_t::getCurrentTrainIdFromLocalFile() == newTrainId)
        return;
    GlobalInfo_t::saveCurrentTrainIdToLocalFile(newTrainId);
    emit changeCurrentLine(newTrainId);
}




void TrainLinePanel::on_runStateBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void TrainLinePanel::updateDatabase(QTableWidgetItem* item)
{
    if(f_updataDB == false)
        return;
    if(item->tableWidget()->horizontalHeaderItem(0)->text() == "Station number")
    {

        QString val = item->text();
        QString colName;
        switch (item->column())
        {
        case 0:colName="indexNu";break;
        case 1:colName="station_id";break;
        case 2:colName="station_name_en";val = "'"+val+"'";break;
        case 3:colName="station_name_th";val = "'"+val+"'";break;
        case 4:colName="start_time";val = "'"+val+"'";break;
        case 5:colName="arrive_time";val = "'"+val+"'";break;
        case 6:colName="lat";break;
        case 7:colName="lng";break;
        case 8:colName="sea_level";break;
        }
        QString sql = QString("update tb_station_info set %1=%2 where rowid=%3;")
                .arg(colName).arg(val).arg(item->tableWidget()->item(item->row(),9)->text());
        GlobalInfo_t::getInstance()->db->exec(sql,false);

    }
    else if(item->tableWidget()->horizontalHeaderItem(0)->text() == "sample id")
    {
        QString sample_id = item->tableWidget()->item(item->row(),0)->text();
        QString val = item->text();
        QString colName;
        switch (item->column())
        {
        case 0:colName="sample_id";break;
        case 1:colName="train_id";break;
        case 2:colName="lng";break;
        case 3:colName="lat";break;
        case 4:colName="sea_level";break;
        }
        QString sql = QString("update tb_railway_info set %1=%2 where sample_id=%3").arg(colName).arg(val).arg(sample_id);
        GlobalInfo_t::getInstance()->db->exec(sql);
    }
}
static bool disableHideBtn = false;
static int hideBtnF = 0;
static QTimer* timer = NULL;
static void func_jiaozheng()
{
    system("xinput_calibrator");
    disableHideBtn = false;
    hideBtnF = 0;
}
void TrainLinePanel::clsHideBtnF()
{
    hideBtnF = 0;
}
void TrainLinePanel::on_hideBtn1_clicked()
{
    if(disableHideBtn)
        return;
    if(timer == NULL)
    {
        timer = new QTimer;
        timer->setInterval(5000);
        timer->setSingleShot(true);
        connect(timer,SIGNAL(timeout()),this,SLOT(clsHideBtnF()));
        timer->start(5000);
    }
    if(hideBtnF == 0)
        hideBtnF++;
    else if(hideBtnF == 2)
        hideBtnF++;
}

void TrainLinePanel::on_hideBtn2_clicked()
{
    if(disableHideBtn)
        return;
    if(timer)
        timer->start(5000);
    if(hideBtnF == 1)
        hideBtnF++;
    else if(hideBtnF == 3)
    {
        disableHideBtn = true;
        QtConcurrent::run(func_jiaozheng);
    }
}

void TrainLinePanel::__initTableHeader()
{
    int listLineCount = ui->listWidget->count();
    for(int i = 0; i < listLineCount;i++)
    {
        QTableWidget *tableWidget = new QTableWidget;
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    //    connect(tableWidget,SIGNAL(doubleClicked(QModelIndex)),keyboard,SLOT(show(QModelIndex)));

        tableWidget->verticalHeader()->setHidden(true);


        tableWidget->setColumnCount(10);
        tableWidget->setColumnHidden(9,true);
        tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        tableWidget->horizontalHeader()->setMinimumHeight(30);
        tableWidget->horizontalHeader()->setMinimumSectionSize(140);
        QTableWidgetItem* item = new QTableWidgetItem("Station number");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(0,item);
        item = new QTableWidgetItem("Station id");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(1,item);
        item = new QTableWidgetItem("Name(EN)");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(2,item);
        item = new QTableWidgetItem("Name(TH)");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(3,item);
        item = new QTableWidgetItem("Start time");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(4,item);
        item = new QTableWidgetItem("Arrive time");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(5,item);

        item = new QTableWidgetItem("Lng");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(6,item);

        item = new QTableWidgetItem("Lat");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(7,item);

        item = new QTableWidgetItem("Sea level");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(8,item);

        item = new QTableWidgetItem("Rowid");
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setHorizontalHeaderItem(9,item);

        ui->stackedWidget->addWidget(tableWidget);
    }
//    connect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(updateDatabase(QTableWidgetItem*)));
}
void TrainLinePanel::initTable()
{
    __initTableHeader();
    int listLineCount = ui->listWidget->count();
    f_updataDB = false;
    if(listLineCount + 2 < ui->stackedWidget->count())
    {
        qDebug("listLineCount is not match stackedWidget count!!");
        return;
    }
    for(int i = 0 ;i < listLineCount;i++)
    {
        int trainId = ui->listWidget->item(i)->text().split(QChar(':'))[0].toInt();
        QString sql = QString("select rowid,indexNu,station_id,station_name_en,station_name_th,start_time,arrive_time,lat,lng,sea_level  from tb_station_info where train_id=%1 order by indexNu").arg(trainId);
        ResultSet rowSet = GlobalInfo_t::getInstance()->db->query(sql);
        QTableWidget *tableWidget = dynamic_cast<QTableWidget*>( ui->stackedWidget->widget(i+2));
        if(tableWidget == NULL)
        {
            qDebug("tableWidget %d is null !",i);
            return;
        }
        tableWidget->setRowCount(rowSet.count());

        for(int i = 0;i < rowSet.count();i++)
        {
            QTableWidgetItem* item = NULL;
            item = tableWidget->item(i,0);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,0,item);
            }
            item->setText(rowSet[i].getPara("indexNu"));

            item = tableWidget->item(i,1);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,1,item);
            }
            item->setText(rowSet[i].getPara("station_id"));

            item = tableWidget->item(i,2);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,2,item);
            }
            item->setText(rowSet[i].getPara("station_name_en"));

            item = tableWidget->item(i,3);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,3,item);
            }
            item->setText(rowSet[i].getPara("station_name_th"));

            item = tableWidget->item(i,4);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,4,item);
            }
            item->setText(rowSet[i].getPara("start_time"));

            item = tableWidget->item(i,5);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,5,item);
            }
            item->setText(rowSet[i].getPara("arrive_time"));

            item = tableWidget->item(i,6);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,6,item);
            }
            item->setText(rowSet[i].getPara("lng"));

            item = tableWidget->item(i,7);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,7,item);
            }
            item->setText(rowSet[i].getPara("lat"));

            item = tableWidget->item(i,8);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,8,item);
            }
            item->setText(rowSet[i].getPara("sea_level"));

            item = tableWidget->item(i,9);
            if(item == NULL)
            {
                item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignHCenter);
                tableWidget->setItem(i,9,item);
            }
            item->setText(rowSet[i].getPara("rowid"));
        }
    }

    f_updataDB = true;
    if(ui->stackedWidget->currentIndex() != 1)
        ui->stackedWidget->setCurrentIndex(1);
}
void TrainLinePanel::on_listWidget_currentRowChanged(int row)
{
    static bool init_f = true;
    if(init_f)
    {
        init_f = false;
//        __initTableHeader();
        return;
    }
    if(row == -1)
        return;
    if(ui->stackedWidget->currentIndex() != row +2)
    {
        qDebug("%d",row);
        ui->stackedWidget->setCurrentIndex(row+2);
    }
}

void TrainLinePanel::on_listWidget_itemClicked(QListWidgetItem *)
{
    return;
    static bool init_f = true;
    if(init_f)
    {
        init_f = false;
        if(ui->listWidget->currentRow() != 1)
        {
            on_listWidget_currentRowChanged(1);
        }
    }/*
    if(ui->stackedWidget->currentIndex() != 1)
        ui->stackedWidget->setCurrentIndex(1);*/
}
