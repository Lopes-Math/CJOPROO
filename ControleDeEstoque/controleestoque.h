#ifndef CONTROLEESTOQUE_H
#define CONTROLEESTOQUE_H

#include <QDialog>
#include <QJsonArray>
#include <QMap>
#include <QPair>
#include <QSet>
#include <utility> // Usado por std::as_const

namespace Ui {
class ControleEstoque;
}

// Tela de controle de estoque
class ControleEstoque : public QDialog
{
    Q_OBJECT

public:
    explicit ControleEstoque(QWidget *parent = nullptr);
    ~ControleEstoque();

private slots:
    // Lógica da aba de consulta
    void on_buscaLineEdit_textChanged(const QString &text);
    void filtrarTabelaConsulta();
    void on_btnVoltarConsulta_clicked(); // Precisa estar aqui pra funcionar corretamente

    // Lógica da aba de movimentação
    void on_buscaMovimentacaoLineEdit_textChanged(const QString &text);
    void on_grupoMovimentacaoComboBox_currentTextChanged(const QString &text);
    void filtrarTabelaMovimentacao();
    void on_confirmarMovimentacaoButton_clicked();
    void on_movimentacaoTableWidget_cellChanged(int row, int column);
    void on_btnVoltar_clicked();

private:
    Ui::ControleEstoque *ui;

    QJsonArray produtosMemoria; // Produtos carregados na memória
    QMap<int, QPair<int, int>> m_movimentacaoData; // idProduto -> (quantidadeAtual, novaQuantidade)

    // Funções auxiliares
    QJsonArray carregarProdutos();                    // Lê os produtos do arquivo JSON
    bool salvarProdutos(const QJsonArray &produtos);  // Salva as alterações
    void carregarTabela();                            // Preenche a tabela de consulta
    void preencherGruposNoComboBox();                 // Alimenta os grupos no ComboBox
    void preencherTabelaMovimentacao();               // Tabela da aba de movimentação
    void atualizarQuantidadesNaTabela();              // Aplica as alterações de estoque
};

#endif // CONTROLEESTOQUE_H
