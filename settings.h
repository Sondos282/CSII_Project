#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();
    QGraphicsView view;
    QGraphicsScene* scene;
    QWidget* parent;
    void loadMap(const QString &location);
    void previewMap(const QString file,QGraphicsScene *scene);
    bool b1();
    bool b2();
    bool b3();
    bool b4();
    bool cannon();
    bool wizard();
    bool archer();
    void setbuttons(QString a, QString b, bool c, bool d);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::settings *ui;
};

#endif // SETTINGS_H
