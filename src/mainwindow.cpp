#include "../headers/mainwindow.hpp"
#include "../ui/ui_mainwindow.h"
#include <qaction.h>
#include <qdir.h>
#include <qgraphicsscene.h>
#include <qimage.h>
#include <qmenu.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <iostream>
#include <QLabel>
#include <qrgb.h>
#include "../headers/scene.hpp"
#include "../headers/renderer.hpp"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),ui(new Ui::MainWindow)
{   
    ui->setupUi(this);
    connect(ui->openAct,&QMenu::aboutToShow,this,&MainWindow::open);
    connect(ui->SelectSceneFile,&QMenu::triggered,this,&MainWindow::loadScene);

    renderer = new CRTRenderer();
    renderResult = new QImage();
    renderResult->fill(0);

    QLabel label;
    image.fill(Qt::black);
    image.load("../output.ppm");
    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->renderView->setScene(scene);
    //label.setPixmap(QPixmap::fromImage()); 
    //connect(ui->openAct,&QAction::triggered,this,&MainWindow::open); 
    /*
    QAction* openAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                          tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    ui->menubar->addAction(openAct);
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::open() {
    std::printf("wowowo");
}


void MainWindow::loadScene(QAction* action) {

    
    if(!action->text().compare("Load CRTScene")) {

        //initialize dialog window to select scene file
        QFileDialog dialog(this, tr("Open File"));
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setNameFilter(tr("CRTScenes (*.crtscene)"));
        dialog.setViewMode(QFileDialog::Detail);
        QString fileName;
        if(dialog.exec()) {
            fileName = dialog.selectedFiles()[0];
            std::cout <<fileName.toStdString() << std::endl;
            //std::printf(dialog.selectedFiles()[0])


            CRTScene scene(fileName.toStdString());
            std::printf("Begin importing scene.\n");
            scene.parseSceneFile(fileName.toStdString());
            //renderer = new CRTRenderer(&scene);
            renderer->loadScene(&scene);
            //renderer->setupTriangleAccessStructure();
            //CRTRenderer renderer(&scene);
            //renderer.setupTriangleAccessStructure();
            std::printf("finished importing scene.\n");
            std::printf("Begin rendering scene.\n");
            auto start = std::chrono::high_resolution_clock::now();

            renderer->render();
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::microseconds dur = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
            const double seconds = dur.count()/1'000'000.0;
            std::cout<< "Execution time: "<<seconds <<" seconds." <<std::endl;
            std::printf("finished rendering scene.\n");
            renderer->storeImage("../output.ppm");
            std::printf("finished storing output.\n");
        }
    }
    
}

void MainWindow::initializeSceneFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode) {
    //static bool firstDialog = true;

    dialog.setDirectory(QDir::currentPath());


    


}