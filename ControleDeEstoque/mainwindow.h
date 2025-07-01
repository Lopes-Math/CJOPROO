#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

// Tela principal do sistema (janela inicial com login/cadastro)
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginClicked();     // Ação ao clicar no botão de login
    void onCadastrarClicked(); // Ação ao clicar no botão de cadastro

private:
    Ui::MainWindow *ui; // Ponteiro para a interface gerada pelo Qt Designer
};

#endif // MAINWINDOW_H
