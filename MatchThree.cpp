#include "DisplayFrame.cpp"

int main(int argc, char** argv) {
    const int sideNum = 10;
    QApplication app(argc, argv);
    DisplayFrame window(sideNum);
    QGraphicsScene scene(&window);
    QGraphicsView view(&scene, &window);
    window.ready(scene);
    window.show();
    return app.exec();
}
