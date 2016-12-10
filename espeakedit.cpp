#include "espeakedit.h"
#include "ui_espeakedit.h"
#include <QFile>
#include<QDebug>
#include "soft_keyboard.h"
#include"globalinfo.h"
ESpeakEdit::ESpeakEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ESpeakEdit)
{
    ui->setupUi(this);
    currentESpeakIndex = -1;
    QFile::copy(":/push_wav.sh","/tmp/push_wav.sh");
    system("chmod +x /tmp/push_wav.sh");
    Soft_Keyboard* keyboard = new Soft_Keyboard;

    connect(ui->tableWidget,SIGNAL(doubleClicked(QModelIndex)),keyboard,SLOT(show(QModelIndex)));
    setWindowFlags(Qt::FramelessWindowHint|(windowFlags() & (~Qt::WindowCloseButtonHint)));
    updateData();
}


void ESpeakEdit::updateData()
{
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setMinimumHeight(30);
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(140);
     ui->tableWidget->setColumnWidth(0,50);
     ui->tableWidget->setColumnWidth(1,545);
     ui->tableWidget->setColumnWidth(2,100);
     ui->tableWidget->setColumnWidth(3,100);
    QTableWidgetItem* item = new QTableWidgetItem("ID");
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setHorizontalHeaderItem(0,item);
    item = new QTableWidgetItem("Content");
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setHorizontalHeaderItem(1,item);
    item = new QTableWidgetItem("Delete");
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setHorizontalHeaderItem(2,item);
    item = new QTableWidgetItem("Play");
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setHorizontalHeaderItem(3,item);
    QStringList strList;
    QFile f("/etc/espeak.txt");
    f.open(QIODevice::ReadWrite);
    while(!f.atEnd())
    {
        strList.append( f.readLine().trimmed());
    }
    f.close();
    ui->tableWidget->setRowCount(strList.count());
    for(int i = 0;i < strList.count();i++)
    {
        item = new QTableWidgetItem(QString::number(i+1));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->setItem(i,0,item);
        item = new QTableWidgetItem(strList[i]);
//        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->setItem(i,1,item);
        QPushButton* btn = new QPushButton("Delete");
        connect(btn,SIGNAL(clicked()),this,SLOT(slot_delete_btn()));
        ui->tableWidget->setCellWidget(i,2,btn);
        btn = new QPushButton("Play");
        connect(btn,SIGNAL(clicked()),this,SLOT(slot_play_btn()));
        ui->tableWidget->setCellWidget(i,3,btn);
    }
}
void ESpeakEdit::slot_delete_btn()
{
    QPushButton* senderBtn = dynamic_cast<QPushButton*>(sender());
    QModelIndex index = ui->tableWidget->indexAt(QPoint(senderBtn->x(),senderBtn->y()));
    for(int i = index.row();i<ui->tableWidget->rowCount()-1;i++)
    {
        QString str = ui->tableWidget->item(i+1,1)->text();
        ui->tableWidget->item(i,1)->setText(str);
    }
    delete ui->tableWidget->cellWidget(ui->tableWidget->rowCount()-1,2);
    delete ui->tableWidget->cellWidget(ui->tableWidget->rowCount()-1,2);
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()-1);
}
ESpeakEdit::~ESpeakEdit()
{
    delete ui;
}


void ESpeakEdit::on_add_btn_clicked()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    QTableWidgetItem* item = new QTableWidgetItem(QString::number(ui->tableWidget->rowCount()));
    item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,item);
    item = new QTableWidgetItem("");
//        item->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,item);
    QPushButton* btn = new QPushButton("Delete");
    connect(btn,SIGNAL(clicked()),this,SLOT(slot_delete_btn()));
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,2,btn);
    btn = new QPushButton("Play");
    connect(btn,SIGNAL(clicked()),this,SLOT(slot_play_btn()));
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,3,btn);
}

void ESpeakEdit::on_ok_btn_clicked()
{
    QFile f("/etc/espeak.txt");
    f.open(QIODevice::WriteOnly);
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        QString str = ui->tableWidget->item(i,1)->text()+"\n";
        f.write(str.toUtf8());
    }
    f.close();
    hide();
}
void ESpeakEdit::slot_play_btn()
{
    QPushButton* senderBtn = dynamic_cast<QPushButton*>(sender());
    QModelIndex index = ui->tableWidget->indexAt(QPoint(senderBtn->x(),senderBtn->y()));
    if(currentESpeakIndex != -1 && index.row() != currentESpeakIndex)
        return;
    QPushButton* btn = dynamic_cast<QPushButton*>(ui->tableWidget->cellWidget(index.row(),3));
    if(btn->text()=="Play")
    {
        currentESpeakIndex = index.row();
        system("echo play > /tmp/espeak_state.txt");
        QString str = ui->tableWidget->item(index.row(),1)->text();
        QString cmd = QString("/tmp/push_wav.sh \"")+str+"\" &";
        qDebug()<<cmd;
        system("ps | grep [p]ush_wav.sh | awk '{print $1}'|  xargs kill -9");
        system(cmd.toAscii().data());
        btn->setText("Stop");
        GlobalInfo::getInstance()->vServerCtrl->pausedServer("ESPEAK");
        for(int i = 0;i < ui->tableWidget->rowCount();i++)
        {
            if(i == currentESpeakIndex)
                continue;
            QPushButton* btnItem = dynamic_cast<QPushButton*>(ui->tableWidget->cellWidget(i,3));
            btnItem->setDisabled(true);
        }
    }
    else if(btn->text()=="Stop")
    {
        currentESpeakIndex = -1;
        for(int i = 0;i < ui->tableWidget->rowCount();i++)
        {
            QPushButton* btnItem = dynamic_cast<QPushButton*>(ui->tableWidget->cellWidget(i,3));
            btnItem->setDisabled(false);
        }
        system("echo stop > /tmp/espeak_state.txt");
        btn->setText("Play");
        GlobalInfo::getInstance()->vServerCtrl->playServer();
    }
}
