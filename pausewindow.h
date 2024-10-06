#ifndef PAUSEWINDOW_H
#define PAUSEWINDOW_H

#include <QDialog>

namespace Ui {
class pauseWindow;
}

class pauseWindow : public QDialog
{
    Q_OBJECT

public:
    explicit pauseWindow(QWidget *parent = nullptr);
    ~pauseWindow();

private slots:
    void on_pushButton_resume_clicked();

    void on_pushButton_exit_clicked();
signals:
    void resumeGame();
    void endGame();

private:
    Ui::pauseWindow *ui;
};

#endif // PAUSEWINDOW_H
