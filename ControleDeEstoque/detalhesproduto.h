#ifndef DETALHESPRODUTO_H
#define DETALHESPRODUTO_H

#include <QDialog>

namespace Ui {
class DetalhesProduto;
}

// Tela de detalhes de um produto específico
class DetalhesProduto : public QDialog
{
    Q_OBJECT

public:
    // Construtor recebe o ID do produto que será exibido
    explicit DetalhesProduto(int produtoId, QWidget *parent = nullptr);
    ~DetalhesProduto();

private slots:
    void on_btnOk_clicked(); // Fecha a janela ao clicar em OK

private:
    Ui::DetalhesProduto *ui;

    // Carrega os dados do produto na interface
    void carregarDetalhes(int produtoId);
};

#endif // DETALHESPRODUTO_H
