#include "../headers/mainwindow.hpp"
#include "../ui/ui_mainwindow.h"
#include <qaction.h>
#include <qdebug.h>
#include <qdir.h>
#include <qframe.h>
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
#include <QResizeEvent>
#include "../headers/scene.hpp"
#include "../headers/renderer.hpp"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),ui(new Ui::MainWindow)
{   
    ui->setupUi(this);
    connect(ui->SelectSceneFile,&QMenu::triggered,this,&MainWindow::loadScene);

    renderer = new CRTRenderer();




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
void MainWindow::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);
    
    ui->renderView->setFixedWidth(event->size().height());
    
    if(ui->renderView->scene())
        ui->renderView->fitInView(ui->renderView->scene()->sceneRect(), Qt::KeepAspectRatio);
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

            image.load("../output.ppm");
            qDebug() <<"Image: height: " <<image.rect().height() << "; width: " << image.rect().width();
            scene2 = new QGraphicsScene(this);
            scene2->addPixmap(image);
            scene2->setSceneRect(image.rect());
            qDebug() <<"scene: height: " <<scene2->sceneRect().height() << "; width: " << scene2->sceneRect().width();
            ui->renderView->setSceneRect(scene2->sceneRect());
            qDebug() <<"render: height: " <<ui->renderView->sceneRect().height() << "; width: " << ui->renderView->sceneRect().width();
            ui->renderView->setScene(scene2);
            ui->renderView->fitInView(scene2->sceneRect());
        }
    }
    
}

void MainWindow::initializeSceneFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode) {
    //static bool firstDialog = true;

    dialog.setDirectory(QDir::currentPath());


    


}