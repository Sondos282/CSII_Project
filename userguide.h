#ifndef USERGUIDE_H
#define USERGUIDE_H

#include <QDialog>

namespace Ui {
class userguide;
}

class userguide : public QDialog
{
    Q_OBJECT

public:
    explicit userguide(QWidget *parent = nullptr);
    void set(QString selcmap, QString d, bool m,bool a);
    ~userguide();

private slots:
    void on_pushButton_clicked();

private:
    Ui::userguide *ui;
    QString map;
    QString def;
    bool music;
    bool AimAssist;

};

#endif // USERGUIDE_H
