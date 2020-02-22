#ifndef EXAMDIALOG_H
#define EXAMDIALOG_H
#include <QDialog>
#include <QTimer>
#include <QLayout>
#include <QTextEdit>
#include <QStringList>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>

class ExamDialog:public QDialog
{
public:
    ExamDialog(QWidget *parent=NULL);
    void refreshTime();
    void timeInit();
    bool textEditInit();
    void layoutInit();
    void buttonsInit();
    void getScores();
    bool hasNoSelect();

private:
    QTimer timer;
    QStringList answerList;   //答案保存
    QTextEdit *textEdit;
    QGridLayout *layout;
    QRadioButton *radiobtnsFront[32];
    QCheckBox *checkBoxs[4];
    QRadioButton *radiobtnsBack[2];
    QLabel *labels[10];
    QButtonGroup *buttonGroups[9];
    QPushButton *pushButton;
protected:

};

#endif // EXAMDIALOG_H
