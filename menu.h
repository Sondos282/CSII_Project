#ifndef MENU_H
#define MENU_H
#include<QGraphicsView>
#include <QDialog>
#include"settings.h"
namespace Ui {
class Menu;
}

class Menu : public QDialog
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();
    QGraphicsView view;
    QGraphicsScene* scene;
    void setmapanddefence(QString m, QString d, bool music,bool inAim);
    void loadMap(const QString &location);
    void previewMap(const QString file, QGraphicsScene *scene);
    settings *settingview;

private slots:

    void on_gamemanual_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Menu *ui;
    QString map;
    QString defence;
    bool music;
    bool AimAssist;

};

#endif // MENU_H
