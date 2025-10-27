#include "UpdateDialog.h"
#include <QStandardPaths>
#include <QFile>
#include <QMessageBox>
#include <QCoreApplication>

UpdateDialog::UpdateDialog(QWidget *parent)
    : QDialog(parent), updateProcess(nullptr)
{
    setWindowTitle("Check for Updates");
    setMinimumWidth(400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    statusLabel = new QLabel("Checking for updates...", this);
    layout->addWidget(statusLabel);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 0); // Indeterminate progress
    progressBar->setVisible(false);
    layout->addWidget(progressBar);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    updateButton = new QPushButton("Update", this);
    updateButton->setVisible(false);
    connect(updateButton, &QPushButton::clicked, this, [this]() {
        updateButton->setEnabled(false);
        closeButton->setEnabled(false);
        progressBar->setVisible(true);
        statusLabel->setText("Downloading update...");

        if (updateProcess) {
            updateProcess->deleteLater();
        }

        updateProcess = new QProcess(this);
        connect(updateProcess, &QProcess::finished,
                this, &UpdateDialog::onUpdateFinished);

        updateProcess->start("appimageupdatetool", QStringList() << getAppImagePath());
    });
    buttonLayout->addWidget(updateButton);

    closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);
}

void UpdateDialog::checkForUpdates() {
    if (!isRunningFromAppImage()) {
        statusLabel->setText("Not running from AppImage. Auto-update is only available for AppImage builds.");
        return;
    }

    statusLabel->setText("Checking for updates...");
    progressBar->setVisible(true);

    if (updateProcess) {
        updateProcess->deleteLater();
    }

    updateProcess = new QProcess(this);
    connect(updateProcess, &QProcess::finished,
            this, &UpdateDialog::onCheckFinished);

    // Use appimageupdatetool to check for updates
    updateProcess->start("appimageupdatetool",
                        QStringList() << "-j" << getAppImagePath());
}

void UpdateDialog::onCheckFinished(int exitCode) {
    progressBar->setVisible(false);

    if (exitCode == 0) {
        // Update available
        statusLabel->setText("An update is available!");
        updateButton->setVisible(true);
    } else if (exitCode == 1) {
        // Already up to date
        statusLabel->setText("You are already running the latest version.");
    } else {
        // Error
        QString error = updateProcess->readAllStandardError();
        statusLabel->setText("Failed to check for updates.\n" + error);
    }
}

void UpdateDialog::onUpdateFinished(int exitCode) {
    progressBar->setVisible(false);
    updateButton->setEnabled(true);
    closeButton->setEnabled(true);

    if (exitCode == 0) {
        statusLabel->setText("Update completed successfully!\nPlease restart the application.");
        QMessageBox::information(this, "Update Complete",
            "The update has been downloaded successfully.\n"
            "Please restart the application to use the new version.");
    } else {
        QString error = updateProcess->readAllStandardError();
        statusLabel->setText("Update failed.\n" + error);
        QMessageBox::warning(this, "Update Failed",
            "Failed to download the update.\n\n" + error);
    }
}

void UpdateDialog::onUpdateProgress() {
    // Could parse progress from appimageupdatetool output here
}

bool UpdateDialog::isRunningFromAppImage() {
    return !qEnvironmentVariable("APPIMAGE").isEmpty();
}

QString UpdateDialog::getAppImagePath() {
    return qEnvironmentVariable("APPIMAGE");
}
