#ifndef MAINWINDOW
#define MAINWINDOW

#include "renderer.hpp"
#include <QMainWindow>
#include <qimage.h>
#include <qmainwindow.h>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    
    ~MainWindow();

    void initializeSceneFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
private slots:
    void open();
    void loadScene(QAction* action);

private:
    Ui::MainWindow *ui;
    CRTRenderer* renderer;
    QImage* renderResult;

    QGraphicsScene *scene;
    QPixmap image;

    
};
#endif // MAINWINDOW