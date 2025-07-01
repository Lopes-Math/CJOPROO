#ifndef CADASTROPRODUTO_H
#define CADASTROPRODUTO_H

#include <QDialog>

namespace Ui {
class CadastroProduto;
}

// Classe da janela de cadastro de produto
class CadastroProduto : public QDialog
{
    Q_OBJECT

public:
    // Construtor e destrutor
    explicit CadastroProduto(QWidget *parent = nullptr);
    ~CadastroProduto();

private slots:
    // Ação do botão "Cadastrar Produto"
    void on_cadastrarButton_clicked();

private:
    Ui::CadastroProduto *ui;  // Interface gerada pelo Qt Designer
    int gerarProximoId();     // Gera automaticamente o próximo ID disponível
};

#endif // CADASTROPRODUTO_H
