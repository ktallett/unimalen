#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QProcess>

class UpdateDialog : public QDialog {
    Q_OBJECT

public:
    explicit UpdateDialog(QWidget *parent = nullptr);
    void checkForUpdates();

private slots:
    void onCheckFinished(int exitCode);
    void onUpdateFinished(int exitCode);
    void onUpdateProgress();

private:
    QLabel *statusLabel;
    QProgressBar *progressBar;
    QPushButton *updateButton;
    QPushButton *closeButton;
    QProcess *updateProcess;

    bool isRunningFromAppImage();
    QString getAppImagePath();
};

#endif // UPDATEDIALOG_H
