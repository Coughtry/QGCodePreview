#include "mainwin.h"
#include "ui_mainwin.h"

#include <QPushButton>
#include <QProcess>
#include <QDesktopServices>
#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_QuitOnClose);

    ui->setupUi(this);

    settings = new QSettings();

    view = new QGCodeViewer(this);
    view->gcode_editor = ui->gcode;

    setCentralWidget(view);

    connect(ui->gcode, SIGNAL(textChanged()), view, SLOT(changedGCode()));

    connect(ui->action_AutoZoom, SIGNAL(triggered()), view, SLOT(toggleAutoZoom()));

    connect(ui->action_Donate, SIGNAL(triggered(bool)), this, SLOT(helpDonate()));
    connect(ui->action_Issues, SIGNAL(triggered(bool)), this, SLOT(helpIssues()));
    connect(ui->action_Chat,   SIGNAL(triggered(bool)), this, SLOT(helpChat()));

    home_dir = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
    openFile = "";

    loadSettings();

}

MainWindow::~MainWindow()
{

}

void MainWindow::showFullScreen()
{
    if( ui->action_showFullScreen->isChecked() )
        showMaximized();
    else
        showNormal();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::loadSettings() 
{

    settings->beginGroup("gui");

    restoreGeometry(settings->value("geometry", saveGeometry() ).toByteArray());
    restoreState(settings->value("state", saveState() ).toByteArray());
    move(settings->value("pos", pos()).toPoint());
    resize(settings->value("size", size()).toSize());

    if (settings->value("maximized", isMaximized() ).toBool()) 
        showMaximized();
    ui->action_showFullScreen->setChecked(settings->value("maximized", isMaximized() ).toBool());
    
    view->setAutoZoom(settings->value("autoZoom", view->autoZoom()).toBool());
    ui->action_AutoZoom->setChecked(settings->value("autoZoom", view->autoZoom()).toBool());

    view->rs274 = settings->value("rs274", "").toString();
    view->tooltable = settings->value("tooltable", "").toString();
    view->gcodefile = settings->value("gcodefile", "").toString();


    settings->endGroup();
        // if any of these paths is not known, cannot work properly, so insist
    if(view->rs274.isEmpty() || view->tooltable.isEmpty() || view->gcodefile.isEmpty())        
        {
        int ret = 1;
        while(ret)
            ret = onSettings();
        }

}

void MainWindow::saveSettings() {

  settings->beginGroup("gui");

  settings->setValue("geometry", saveGeometry());
  settings->setValue("state", saveState());
  settings->setValue("maximized", isMaximized());

  if ( !isMaximized() ) {
    settings->setValue("pos", pos());
    settings->setValue("size", size());
  }

  settings->setValue("autoZoom", ui->action_AutoZoom->isChecked());

  settings->setValue("rs274", view->rs274);
  settings->setValue("tooltable", view->tooltable);
  settings->setValue("gcodefile", view->gcodefile);

  settings->endGroup();

}

/////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::closeEvent(QCloseEvent * event) 
{
  saveSettings();

  QMainWindow::closeEvent(event);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::onOpenFile()
{
QString filename;
QString path = home_dir + "machinekit";

    filename = QFileDialog::getOpenFileName(this, tr("Open GCode"), path, tr("GCode Files (*.ngc *.nc);; All files (*.*)"));
    if(filename.length())
        {
        disconnect(ui->gcode, SIGNAL(textChanged()), view, SLOT(changedGCode()));
        view->onOpenGCode(filename);
        openInBrowser(view->gcodefile);
        connect(ui->gcode, SIGNAL(textChanged()), view, SLOT(changedGCode()));
        }
}



void MainWindow::openInBrowser(QString filename)
{
QFile file(filename);
QString str;

    if (file.open(QFile::ReadOnly | QFile::Text))
        {
        str = "Loading file " + filename;
        ui->statusbar->showMessage(str, 5000);
        ui->gcode->clear();

        QTextStream ts(&file);

        while( !ts.atEnd())
            {
            str = ts.readLine();
            if(str.length()) // don't want blank lines
                {
                str = str + "\n";
                ui->gcode->appendNewPlainText(str);
                }
            }
        file.close();  

        str = "QGCodePreview :- " +  filename;
        setWindowTitle(str);

        ui->gcode->highlightLine(1);

        openFile = filename;
        }
    else
        {
        str = "Error loading file " + filename ;
        ui->statusbar->showMessage(str, 5000);
        }
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::onSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save G Code (As)"), openFile, tr("GCode Files (*.ngc *.nc);; All files (*.*)"));
    saveInBrowser(fileName);
}



int  MainWindow::saveInBrowser(QString& filename)
{
QFile file(filename);
QString str;

    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
        QTextStream out(&file);
        out << ui->gcode->toPlainText();
        file.close();

        str = "QGCodePreview:- " +  filename;
        setWindowTitle(str);
        return 0;
        }
    else
        {
        str = "Error saving file " + filename ;
        ui->statusbar->showMessage(str, 5000);
        return -1;
        }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

int MainWindow::onSettings()
{

    SettingsDialog *dlg = new SettingsDialog(this, home_dir);

    dlg->setValues(view->rs274, view->tooltable, view->gcodefile);

    dlg->exec();

    if(dlg->result())
        {
        view->rs274 = dlg->rs274;
        view->tooltable = dlg->tooltable;
        view->gcodefile = dlg->gcodefile;
        }

    if(view->rs274.isEmpty() || view->tooltable.isEmpty())	
        return 1;
    else
        return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::helpDonate() {
    QDesktopServices::openUrl(QUrl("https://koppi.github.com/paypal"));
}

void MainWindow::helpIssues() {
    QDesktopServices::openUrl(QUrl("https://github.com/koppi/gcoder/issues"));
}

void MainWindow::helpChat() {
    QDesktopServices::openUrl(QUrl("https://gitter.im/koppi/gcoder"));
}
