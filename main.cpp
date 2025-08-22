#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QProcess>
#include <QTextEdit>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Expression Validator");
    window.resize(700, 450);

    QVBoxLayout *layout = new QVBoxLayout(&window);

    QTextEdit *output = new QTextEdit();
    output->setReadOnly(true);

    QPushButton *selectBtn = new QPushButton("Select CSV File");
    QPushButton *runBtn = new QPushButton("Run Model");

    QString selectedFile;

    layout->addWidget(selectBtn);
    layout->addWidget(runBtn);
    layout->addWidget(output);
    QObject::connect(selectBtn, &QPushButton::clicked, [&]() {
        selectedFile = QFileDialog::getOpenFileName(&window, "Open CSV", "", "CSV Files (*.csv)");
        if (!selectedFile.isEmpty()) {
            output->append("Selected file: " + selectedFile);
            std::cout << "Selected file: " << selectedFile.toStdString() << std::endl;
        }
    });
    QObject::connect(runBtn, &QPushButton::clicked, [&]() {
        if (selectedFile.isEmpty()) {
            output->append("Please select a CSV first!");
            std::cout << "Please select a CSV first!" << std::endl;
            return;
        }

        output->append(" Running Python script...");
        std::cout << "Running Python script..." << std::endl;

        QProcess *process = new QProcess(&window);

        QString pythonExe = "C:/Users/aarch/AppData/Local/Programs/Python/Python312/python.exe";
        QString scriptPath = "C:/Users/aarch/OneDrive/Desktop/gui c++/evaluate_csv.py";
        process->setWorkingDirectory("C:/Users/aarch/OneDrive/Desktop/gui c++");
        process->start(pythonExe, QStringList() << scriptPath << selectedFile);

        if(!process->waitForStarted()) {
            output->append("Failed to start Python script!");
            std::cout << "Failed to start Python script!" << std::endl;
            return;
        }

        process->waitForFinished(-1); 
        QString stdOutput = QString::fromUtf8(process->readAllStandardOutput());
        if(!stdOutput.isEmpty()) {
            output->append("Output:\n" + stdOutput);
            std::cout << stdOutput.toStdString() << std::endl;   
        }

        QString stdError = QString::fromUtf8(process->readAllStandardError());
        if(!stdError.isEmpty()) {
            output->append(" Errors:\n" + stdError);
            std::cerr << stdError.toStdString() << std::endl;   
        }
    });

    window.setLayout(layout);
    window.show();

    return app.exec();
}
