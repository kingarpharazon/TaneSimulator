#include <QFileDialog>
#include <QProgressDialog>
#include <QtConcurrentMap>
#include <QFutureWatcher>
#include <QProcess>

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    this->setupUi(this);
}

void Dialog::on_pushButtonBrowse_clicked()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, QObject::tr("Select a file"),
                                                "", tr("csv (*.csv)"), nullptr, nullptr);

    if(!filename.isEmpty())
    {
        QFileInfo fileInfo(filename);

        _filename = filename;
        lineEditDataFile->setText(filename);
        lineEditOutputFile->setText(fileInfo.baseName());
    }
}

void Dialog::on_pushButtonAdd_clicked()
{
    if(!lineEditDataFile->text().isEmpty() &&
       !lineEditOutputFile->text().isEmpty())
    {
        // Get the filename, parameters and output file from the dialog
        QString datafile = lineEditDataFile->text();
        QString parameters = lineEditParameters->text();
        QString outputFile = lineEditOutputFile->text();

        SimItem item;
        item.setDataFile(datafile);
        item.setParameters(parameters);
        item.setOutputFile(outputFile);

        _simItems << item;

        listWidgetItems->addItem(lineEditDataFile->text());

        pushButtonStart->setEnabled(true);
    }
}

void Dialog::on_pushButtonRemove_clicked()
{
    // Delete the corresponding item from the list
    _simItems.removeAt(listWidgetItems->row(listWidgetItems->currentItem()));

    // Delete the currently selected item, since Qt does not delete the item
    // when it is removed, it must be manually deleted
    QListWidgetItem* item;
    item = listWidgetItems->takeItem(listWidgetItems->row(listWidgetItems->currentItem()));
    delete item;

    // If the QListWidget is empty, dissable pushButtonStart
    if(listWidgetItems->count() == 0)
    {
        pushButtonStart->setEnabled(false);
    }
}

void Dialog::on_pushButtonClear_clicked()
{
    _simItems.clear();
    listWidgetItems->clear();
    pushButtonStart->setEnabled(false);
}

void Dialog::on_pushButtonStart_clicked()
{
    QProgressDialog progressDialog;
    progressDialog.setLabelText("Processing items. Please wait.");

    QFutureWatcher<void> futureWatcher;
    connect(&futureWatcher, SIGNAL(finished()),
            &progressDialog, SLOT(reset()));
    connect(&futureWatcher, SIGNAL(progressRangeChanged(int, int)),
            &progressDialog, SLOT(setRange(int, int)));
    connect(&futureWatcher, SIGNAL(progressValueChanged(int)),
            &progressDialog, SLOT(setValue(int)));
    connect(&progressDialog, SIGNAL(canceled()),
            &futureWatcher, SLOT(cancel()));

    // Start the computation. To be able to use a member function in the call to
    // map(), an instance to the containing class is needed, i.e., the 'this'
    // pointer. A lambda is used here to provide such pointer.
    futureWatcher.setFuture(QtConcurrent::map(_simItems, [this](SimItem& item)
    {
        QString command = "../TaneSimulator/DependencyMiner -f " + item.dataFile() +
            " " + item.parameters() + " -o " + item.outputFile();
        QProcess::execute(command);
    }));

    // Display the dialog and start the event loop
    progressDialog.exec();

    futureWatcher.waitForFinished();

    // Query the future to check if it was cancelled
    qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
}

void Dialog::on_lineEditParameters_textChanged()
{
    if(!lineEditDataFile->text().isEmpty())
    {
        QFileInfo fileInfo(lineEditDataFile->text());
        QString outputfileName = fileInfo.baseName() + '_' + lineEditParameters->text();
        outputfileName.replace(' ', '_');

        lineEditOutputFile->setText(outputfileName);
    }
}
