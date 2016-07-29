#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSettings>

#include "QGCodeEditor.h"
#include "QGCodeViewer.h"
#include "settings_dlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    void parseGcode();

public slots:

    virtual void onOpenFile();
    virtual void onSaveAs();
    virtual int onSettings();

    virtual void showFullScreen();

    virtual void helpDonate();
    virtual void helpIssues();
    virtual void helpChat();

protected:
    void loadSettings();
    void saveSettings();

private:  // functions
//    int openInViewer(QString filename);
    void openInBrowser(QString filename);
    int saveInBrowser(QString& filename);

    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private: // data
    QString home_dir, openFile;

    Ui::MainWindow *ui;

    QGCodeViewer *view;

    QSettings *settings;
};

#endif // MAINWINDOW_H
