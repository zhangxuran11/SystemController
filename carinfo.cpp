#include "carinfo.h"
#include "globalinfo.h"
#include"mainwindow.h"
QVector<CarInfo*> CarInfo::v_carInfos = QVector<CarInfo*>(20,0);
CarInfo::CarInfo()
{
	connect(&timer,SIGNAL(timeout()),this,SLOT(slot_timeout()));
	timer.setSingleShot(true);
    timer.start(30000);
}
void CarInfo::slot_timeout()
{
//	qDebug("slot_timeout");
	setSlm("--",1);
	setSlm("--",2);
}
void CarInfo::init()
{
    for(int i = 1;i <= 13;i++)
        v_carInfos[i] = new CarInfo;
    MainWindow *mainWin = GlobalInfo::getInstance()->mainWin;
    v_carInfos[1]->slm1 = mainWin->ui->slm1_1;
    v_carInfos[1]->slm2 = mainWin->ui->slm1_2;

    v_carInfos[2]->slm1 = mainWin->ui->slm2_1;
    v_carInfos[2]->slm2 = mainWin->ui->slm2_2;

    v_carInfos[3]->slm1 = mainWin->ui->slm3_1;
    v_carInfos[3]->slm2 = mainWin->ui->slm3_2;

    v_carInfos[4]->slm1 = mainWin->ui->slm4_1;
    v_carInfos[4]->slm2 = mainWin->ui->slm4_2;
    v_carInfos[5]->slm1 = mainWin->ui->slm5_1;
    v_carInfos[5]->slm2 = mainWin->ui->slm5_2;

    v_carInfos[6]->slm1 = mainWin->ui->slm6_1;
    v_carInfos[6]->slm2 = mainWin->ui->slm6_2;

    v_carInfos[7]->slm1 = mainWin->ui->slm7_1;
    v_carInfos[7]->slm2 = mainWin->ui->slm7_2;

    v_carInfos[8]->slm1 = mainWin->ui->slm8_1;
    v_carInfos[8]->slm2 = mainWin->ui->slm8_2;

    v_carInfos[9]->slm1 = mainWin->ui->slm9_1;
    v_carInfos[9]->slm2 = mainWin->ui->slm9_2;

    v_carInfos[10]->slm1 = mainWin->ui->slm10_1;
    v_carInfos[10]->slm2 = mainWin->ui->slm10_2;

    v_carInfos[11]->slm1 = mainWin->ui->slm11_1;
    v_carInfos[11]->slm2 = mainWin->ui->slm11_2;

    v_carInfos[12]->slm1 = mainWin->ui->slm12_1;
    v_carInfos[12]->slm2 = mainWin->ui->slm12_2;

    v_carInfos[13]->slm1 = mainWin->ui->slm13_1;
    v_carInfos[13]->slm2 = mainWin->ui->slm13_2;

    for(int i = 1;i <= 13;i++)
    {
        v_carInfos[i]->setSlm("--",1);
        v_carInfos[i]->setSlm("--",2);
    }
}

void CarInfo::setSlm(const QString &state, int slmNo)
{
    QString styleSheet;
    if(state == "OPEN")
    {
        styleSheet = "color: rgb(255, 255, 255);"
                "font: 900 14pt \"Ubuntu\";"
                "border: 1px solid black;"
                "background-color: rgb(85, 255, 0);";
        timer.start(30000);
    }
    else if(state == "CLOSE")
    {
        styleSheet = "background-color: rgb(255, 85, 0);"
                "color: rgb(255, 255, 255);"
                "font: 900 14pt \"Ubuntu\";"
                "border: 1px solid black;";
        timer.start(30000);
    }
    else if(state == "--")
    {
        styleSheet = "background-color: rgb(255, 255, 255);"
                "color: rgb(255, 255, 255);"
                "font: 900 14pt \"Ubuntu\";"
                "border: 1px solid black;";
    }
    if(slmNo == 1&&slm1->text() != state)
    {
        slm1->setText(state);
        slm1->setStyleSheet(styleSheet);
    }
    else if(slmNo == 2&&slm2->text()!=state)
    {
        slm2->setText(state);
        slm2->setStyleSheet(styleSheet);
    }
}

