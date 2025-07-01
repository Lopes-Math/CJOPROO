#ifndef MENUMAINWINDOW_H
#define MENUMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MenuMainWindow;
}

// Janela principal do menu depois do login
class MenuMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuMainWindow(QWidget *parent = nullptr);
    ~MenuMainWindow();

private slots:
    void on_btnCadastros_clicked(); // Vai para a tela de cadastros
    void on_btnEstoque_clicked();   // Abre a tela de controle de estoque
    void on_btnLogout_clicked();    // Faz logout do sistema

private:
    Ui::MenuMainWindow *ui; // Interface gerada pelo Qt Designer
};

#endif // MENUMAINWINDOW_H
