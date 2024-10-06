#ifndef WINNING_H
#define WINNING_H

#include <QDialog>

namespace Ui {
class winning;
}

class winning : public QDialog
{
    Q_OBJECT

public:
    explicit winning(QWidget *parent = nullptr);
    ~winning();
    void getscore(int x, int k, int g);
    void getleveldata(QString m, QString d, int dif, bool aim);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::winning *ui;
    QString map;
    QString defence;
    bool AimAssist;

    int difficulty;
};

#endif // WINNING_H
