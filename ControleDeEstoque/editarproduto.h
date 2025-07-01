#ifndef EDITARPRODUTO_H
#define EDITARPRODUTO_H

#include <QDialog>

namespace Ui {
class EditarProduto;
}

// Janela para editar um produto existente
class EditarProduto : public QDialog
{
    Q_OBJECT

public:
    // Construtor recebe o ID do produto a ser editado
    explicit EditarProduto(int produtoId, QWidget *parent = nullptr);
    ~EditarProduto();

private slots:
    // Ação do botão "Salvar Alterações"
    void on_salvarButton_clicked();

private:
    Ui::EditarProduto *ui;
    int m_produtoId; // Guarda o ID do produto atual

    void carregarDados(); // Preenche os campos da interface com os dados do produto
};

#endif // EDITARPRODUTO_H
