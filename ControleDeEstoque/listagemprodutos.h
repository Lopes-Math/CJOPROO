#ifndef LISTAGEMPRODUTOS_H
#define LISTAGEMPRODUTOS_H

#include <QDialog>
#include <QJsonArray>
#include <QSet>
#include <utility> // Usado pelo std::as_const

namespace Ui {
class ListagemProdutos;
}

// Tela com a listagem geral dos produtos
class ListagemProdutos : public QDialog
{
    Q_OBJECT

public:
    explicit ListagemProdutos(QWidget *parent = nullptr);
    ~ListagemProdutos();

private slots:
    // Filtros e botões principais
    void filtrarTabela();
    void on_btnCadastrarProduto_clicked();
    void on_btnVoltar_clicked();

    // Ações específicas de cada item da tabela
    void onVerMaisClicked(int produtoId);
    void onEditarClicked(int produtoId);
    void onExcluirClicked(int produtoId);

private:
    Ui::ListagemProdutos *ui;

    // Métodos de apoio pra carregar e manipular os dados
    QJsonArray carregarProdutos();
    bool salvarProdutos(const QJsonArray &produtos);
    void carregarDadosNaTabela();               // Preenche a tabela com os produtos
    void preencherGruposNoComboBox();           // Grupos usados no filtro
    void adicionarBotoesAcao(int linha, int produtoId); // Coloca os botões "Ver", "Editar", "Excluir" na tabela
};

#endif // LISTAGEMPRODUTOS_H
