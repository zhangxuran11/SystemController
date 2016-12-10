#include "soft_keyboard.h"
#include<QVariant>
void Soft_Keyboard::show()
{
    senderWidget = sender();
    QTextEdit *currentTextEdit = dynamic_cast<QTextEdit*>(senderWidget);
    QLineEdit* currentLineEdit = dynamic_cast<QLineEdit*>(senderWidget);
    QPushButton *currentBtnEdit = dynamic_cast<QPushButton*>(senderWidget);
    if(currentTextEdit != NULL)
        lineEdit->setText(currentTextEdit->toPlainText());
    else if(currentLineEdit != NULL)
        lineEdit->setText(currentLineEdit->text());
    else if(currentBtnEdit != NULL)
        lineEdit->setText(currentBtnEdit->text());

    QDialog::show();
    raise();
    lineEdit->setSelection(lineEdit->text().length(),0);
}
void Soft_Keyboard::show(const QModelIndex& _modelIndex)
{
//    QTableView* tableView;
//    QListView* listView;
    senderWidget = sender();
    modelIndex = _modelIndex;
    QListView* listView = dynamic_cast<QListView*>(senderWidget);
    QTableView* tableView = dynamic_cast<QTableView*>(senderWidget);
    if(listView != NULL)
        lineEdit->setText(listView->model()->data(modelIndex).toString());
    else if(tableView != NULL)
        lineEdit->setText( tableView->model()->data(modelIndex).toString());

    QDialog::show();
    raise();
    lineEdit->setSelection(lineEdit->text().length(),0);
}
void Soft_Keyboard::slot_OK_PusnBtn_Clicked()
{
    QTextEdit *currentTextEdit = dynamic_cast<QTextEdit*>(senderWidget);
    QLineEdit* currentLineEdit = dynamic_cast<QLineEdit*>(senderWidget);
    QPushButton *currentBtnEdit = dynamic_cast<QPushButton*>(senderWidget);
    QListView* listView = dynamic_cast<QListView*>(senderWidget);
    QTableView* tableView = dynamic_cast<QTableView*>(senderWidget);
    if(currentTextEdit != NULL)
        currentTextEdit->setText(lineEdit->text());
    else if(currentLineEdit != NULL)
        currentLineEdit->setText(lineEdit->text());
    else if(currentBtnEdit != NULL)
        currentBtnEdit->setText(lineEdit->text());
    else if(listView != NULL)
        listView->model()->setData(modelIndex,lineEdit->text());
    else if(tableView != NULL)
        tableView->model()->setData(modelIndex,lineEdit->text());
    close();

}
Soft_Keyboard::Soft_Keyboard(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint|(windowFlags() & (~Qt::WindowCloseButtonHint)));
    currentEditWidget = NULL;
    this->resize(800, 600);
//    this->resize(550, 500);
//    this->setMaximumSize(550,500);
//    this->setWindowTitle("Keyboard");

    hBoxLayout = new QHBoxLayout;
    vBoxLayout = new QVBoxLayout;

    tabWidget = new QTabWidget;
    tabWidget->setFixedSize(780, 400);

    soft_Keyboard_Numbers = new Soft_Keyboard_Numbers;
    soft_Keyboard_Complete = new Soft_Keyboard_Complete;
    soft_keyboard_Capital_Letter = new Soft_keyboard_Capital_Letter;
    soft_Keyboard_Lowercase_Letter = new Soft_Keyboard_Lowercase_Letter;
    soft_Keyboard_Symbol = new Soft_Keyboard_Symbol;

    connect_Letter_PushBtn_Click();

    //tabWidget->addTab(soft_Keyboard_Numbers,"Numbers");
    //tabWidget->addTab(soft_Keyboard_Complete,"Complete");
    tabWidget->addTab(soft_keyboard_Capital_Letter,"Capital Letter");
    tabWidget->addTab(soft_Keyboard_Lowercase_Letter,"Lowercase Letter");
    //tabWidget->addTab(soft_Keyboard_Symbol,"Symbol");

    tabWidget->setFocusPolicy(Qt::NoFocus);

    tabWidget->setStyleSheet("QTabBar::tab {min-width:100px;min-height:26px;color: black;border:1px solid; border-color:rgb(200, 200, 200); border-top-left-radius:4; border-top-right-radius:4;} \
                                  QTabBar::tab:!selected {margin-top: 5px;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(232, 232, 232, 255), stop:1 rgba(195, 196, 200, 255));} \
                                  QTabBar::tab:selected {background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(188, 222, 237, 255), stop:1 rgba(116, 173, 196, 255));} ");

    tabWidget->setAttribute(Qt::WA_AttributeCount, true);
    pushBtn_Clean = new QPushButton("Clean");
    pushBtn_Space = new QPushButton("Space");
    pushBtn_Backspace = new QPushButton("BackSpace");
    pushBtn_Quit = new QPushButton("OK");

    pushBtn_Clean->setStyleSheet("width:120px; height: 35px; color:rgb(39, 83, 102); border:none; border-radius:10px; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(188, 222, 237, 255), stop:1 rgba(116, 173, 196, 255)); font-size:20px;");
    pushBtn_Space->setStyleSheet("width:120px; height: 35px; color:rgb(39, 83, 102); border:none; border-radius:10px; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(188, 222, 237, 255), stop:1 rgba(116, 173, 196, 255)); font-size:20px;");
    pushBtn_Backspace->setStyleSheet("width:120px; height: 35px; color:rgb(39, 83, 102); border:none; border-radius:10px; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(188, 222, 237, 255), stop:1 rgba(116, 173, 196, 255)); font-size:20px;");
    pushBtn_Quit->setStyleSheet("width:120px; height: 35px; color:rgb(39, 83, 102); border:none; border-radius:10px; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 rgba(188, 222, 237, 255), stop:1 rgba(116, 173, 196, 255)); font-size:20px;");

    pushBtn_Clean->setFocusPolicy(Qt::NoFocus);
    pushBtn_Space->setFocusPolicy(Qt::NoFocus);
    pushBtn_Backspace->setFocusPolicy(Qt::NoFocus);
    pushBtn_Quit->setFocusPolicy(Qt::NoFocus);

    connect(pushBtn_Clean, SIGNAL(clicked()), this, SLOT(slot_Clean_PushBtn_Clicked()));
    connect(pushBtn_Space, SIGNAL(clicked()), this, SLOT(slot_Space_PushBtn_Clicked()));
    connect(pushBtn_Backspace, SIGNAL(clicked()), this, SLOT(slot_BackSpace_PusnBtn_Clicked()));
    connect(pushBtn_Quit, SIGNAL(clicked()), this, SLOT(slot_OK_PusnBtn_Clicked()));

    hBoxLayout->addStretch();
    hBoxLayout->addWidget(pushBtn_Clean);
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(pushBtn_Space);
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(pushBtn_Backspace);
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(pushBtn_Quit);
    hBoxLayout->addStretch();

    lineEdit = new QLineEdit;
    lineEdit->setFixedSize(780,40);

    vBoxLayout->addWidget(lineEdit);
    currentEditWidget = lineEdit;
    vBoxLayout->addWidget(tabWidget);
    vBoxLayout->addLayout(hBoxLayout);

    setLayout(vBoxLayout);
    this->setModal(false);

//    setAttribute(Qt::WA_X11DoNotAcceptFocus, true);
    setAttribute(Qt::WA_ShowWithoutActivating, true);
    this->setStyleSheet("background-color: rgb(227, 232, 234);");
}

Soft_Keyboard::~Soft_Keyboard()
{
}

void Soft_Keyboard::connect_Letter_PushBtn_Click(){
    for(int i = 0; i < 21; i++){
        connect(soft_Keyboard_Numbers->pushBtn_Numbers[i], SIGNAL(clicked()), this, SLOT(slot_Letter_PushBtn_Clicked()));
    }
    for(int i = 0; i < 87; i++){
        connect(soft_Keyboard_Complete->pushBtn_Complete[i], SIGNAL(clicked()), this, SLOT(slot_Letter_PushBtn_Clicked()));
    }
    for(int i = 0; i < 26; i++){
        connect(soft_keyboard_Capital_Letter->pushBtn_Capital_Letter[i], SIGNAL(clicked()), this, SLOT(slot_Letter_PushBtn_Clicked()));
    }
    for(int i = 0; i < 26; i++){
        connect(soft_Keyboard_Lowercase_Letter->pushBtn_Lowercase_Letter[i], SIGNAL(clicked()), this, SLOT(slot_Letter_PushBtn_Clicked()));
    }
    for(int i = 0; i < 32; i++){
        connect(soft_Keyboard_Symbol->pushBtn_Symbol[i], SIGNAL(clicked()), this, SLOT(slot_Letter_PushBtn_Clicked()));
    }
}

void Soft_Keyboard::slot_Letter_PushBtn_Clicked(){
    QPushButton *aPushButton = dynamic_cast<QPushButton*>(sender());
    QTextEdit *currentTextEdit = dynamic_cast<QTextEdit*>(currentEditWidget);
    QLineEdit* currentLineEdit = dynamic_cast<QLineEdit*>(currentEditWidget);
    QPushButton *currentBtnEdit = dynamic_cast<QPushButton*>(currentEditWidget);
    QListView* listView = dynamic_cast<QListView*>(currentEditWidget);
    QTableView* tableView = dynamic_cast<QTableView*>(currentEditWidget);
    QString text;
    if(aPushButton->text() == "&&")
        text = "&";
    else
        text = aPushButton->text();
    if(currentLineEdit != NULL){
        currentLineEdit->insert(text);
        goto label_end;
    }
    if(currentTextEdit != NULL){
        currentTextEdit->append(text);
        goto label_end;
    }
    if(currentBtnEdit != NULL){
        currentBtnEdit->setText(currentBtnEdit->text()+text);
        goto label_end;
    }
    if(listView != NULL){
        QString str = listView->model()->data(modelIndex).toString()+text;
        listView->model()->setData(modelIndex,str);
        goto label_end;
    }
    if(tableView != NULL){

        QString str = tableView->model()->data(modelIndex).toString()+text;
        tableView->model()->setData(modelIndex,str);
        goto label_end;
    }
label_end:
    return;
}

void Soft_Keyboard::slot_Clean_PushBtn_Clicked(){
    QPushButton *currentBtnEdit = dynamic_cast<QPushButton*>(currentEditWidget);
    QTextEdit *currentTextEdit = dynamic_cast<QTextEdit*>(currentEditWidget);
    QLineEdit* currentLineEdit = dynamic_cast<QLineEdit*>(currentEditWidget);
    QListView* listView = dynamic_cast<QListView*>(currentEditWidget);
    QTableView* tableView = dynamic_cast<QTableView*>(currentEditWidget);

    if(currentBtnEdit != NULL){
        currentBtnEdit->setText("");
        goto label_end;
    }
    if(currentLineEdit != NULL){
        currentLineEdit->setText("");
        goto label_end;
    }
    if(currentTextEdit != NULL){
        currentTextEdit->setPlainText("");
        goto label_end;
    }
    if(listView != NULL){
        QString str = "";
        listView->model()->setData(modelIndex,str);
        goto label_end;
    }
    if(tableView != NULL){
        QString str = "";
        tableView->model()->setData(modelIndex,str);
        goto label_end;
    }
label_end:
    return;
}

void Soft_Keyboard::slot_Space_PushBtn_Clicked(){
    QPushButton *currentBtnEdit = dynamic_cast<QPushButton*>(currentEditWidget);
    QTextEdit *currentTextEdit = dynamic_cast<QTextEdit*>(currentEditWidget);
    QLineEdit* currentLineEdit = dynamic_cast<QLineEdit*>(currentEditWidget);
    QListView* listView = dynamic_cast<QListView*>(currentEditWidget);
    QTableView* tableView = dynamic_cast<QTableView*>(currentEditWidget);
    QString text = " ";
    if(currentBtnEdit != NULL){
        currentBtnEdit->setText(currentBtnEdit->text()+text);
        goto label_end;
    }
    if(currentLineEdit != NULL){
        currentLineEdit->insert(text);
        goto label_end;
    }
    if(currentTextEdit != NULL){
        currentTextEdit->append(text);
        goto label_end;
    }
    if(listView != NULL){
        QString str = listView->model()->data(modelIndex).toString() + " ";
        listView->model()->setData(modelIndex,str);
        goto label_end;
    }
    if(tableView != NULL){
        QString str = tableView->model()->data(modelIndex).toString() + " ";
        tableView->model()->setData(modelIndex,str);
        goto label_end;
    }
label_end:
    return;
}

void Soft_Keyboard::slot_BackSpace_PusnBtn_Clicked(){
    QPushButton *currentBtnEdit = dynamic_cast<QPushButton*>(currentEditWidget);
    QTextEdit *currentTextEdit = dynamic_cast<QTextEdit*>(currentEditWidget);
    QLineEdit* currentLineEdit = dynamic_cast<QLineEdit*>(currentEditWidget);
    QListView* listView = dynamic_cast<QListView*>(currentEditWidget);
    QTableView* tableView = dynamic_cast<QTableView*>(currentEditWidget);

    if(currentBtnEdit != NULL){
        currentBtnEdit->setText(currentBtnEdit->text().left(currentBtnEdit->text().length()-1));
        goto label_end;
    }
    if(currentLineEdit != NULL){
        currentLineEdit->backspace();
        goto label_end;
    }
    if(currentTextEdit != NULL){
        currentTextEdit->textCursor().deletePreviousChar();
        goto label_end;
    }
    if(listView != NULL){
        QString str = listView->model()->data(modelIndex).toString();
        str = str.remove(str.length()-1,1);
        listView->model()->setData(modelIndex,str);
        goto label_end;
    }
    if(tableView != NULL){
        QString str = tableView->model()->data(modelIndex).toString();
        str = str.remove(str.length()-1,1);
        tableView->model()->setData(modelIndex,str);
        goto label_end;
    }
label_end:
    return;
}

