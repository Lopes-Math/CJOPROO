#include "detalhesproduto.h"
#include "ui_detalhesproduto.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

// Construtor da janela de detalhes do produto
DetalhesProduto::DetalhesProduto(int produtoId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetalhesProduto)  // Inicializa a interface gráfica para detalhes do produto
{
    ui->setupUi(this);  // Configura a interface gráfica
    setWindowTitle("Detalhes do Produto");  // Define o título da janela
    carregarDetalhes(produtoId);  // Chama a função para carregar os detalhes do produto com o ID fornecido
}

// Destruidor da classe, libera a memória alocada para a interface
DetalhesProduto::~DetalhesProduto()
{
    delete ui;
}

// Função para carregar os detalhes de um produto com base no ID
void DetalhesProduto::carregarDetalhes(int produtoId)
{
    QFile file("produtos.json");  // Abre o arquivo de produtos
    if (!file.open(QIODevice::ReadOnly)) {
        // Se o arquivo não puder ser aberto, exibe uma mensagem de erro
        QMessageBox::critical(this, "Erro", "Não foi possível abrir o arquivo de produtos.");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());  // Lê o conteúdo do arquivo JSON
    file.close();  // Fecha o arquivo após ler os dados

    QJsonArray produtos = doc.array();  // Converte o documento JSON em um array de produtos
    QJsonObject produtoObj;  // Cria um objeto para armazenar os dados do produto encontrado
    bool encontrado = false;  // Flag para verificar se o produto foi encontrado

    // Procura o produto no array com o ID correspondente
    for (const QJsonValue &val : produtos) {
        QJsonObject obj = val.toObject();  // Converte o valor do array em um objeto JSON
        if (obj["id"].toInt() == produtoId) {  // Verifica se o ID do produto corresponde ao ID fornecido
            produtoObj = obj;  // Armazena os dados do produto encontrado
            encontrado = true;  // Marca que o produto foi encontrado
            break;  // Sai do loop após encontrar o produto
        }
    }

    if (encontrado) {
        // Preenche os campos da interface gráfica com os dados do produto
        ui->valor_id->setText(QString::number(produtoObj["id"].toInt()));  // Exibe o ID do produto
        ui->valor_nome->setText(produtoObj["nome"].toString());  // Exibe o nome do produto
        ui->valor_grupo->setText(produtoObj["grupo"].toString());  // Exibe o grupo do produto
        ui->valor_marca->setText(produtoObj["marca"].toString());  // Exibe a marca do produto
        ui->valor_codigo->setText(produtoObj["codigo_barras"].toString());  // Exibe o código de barras
        ui->valor_qtd->setText(QString::number(produtoObj["quantidade"].toInt()));  // Exibe a quantidade disponível
        ui->valor_preco->setText("R$ " + QString::number(produtoObj["preco"].toDouble(), 'f', 2));  // Exibe o preço formatado
        ui->valor_descricao->setText(produtoObj["descricao"].toString());  // Exibe a descrição do produto
    } else {
        // Se o produto não for encontrado, exibe uma mensagem de aviso
        QMessageBox::warning(this, "Não Encontrado", "O produto com o ID especificado não foi encontrado.");
        // Opcional: Você pode desabilitar campos ou fechar a janela se o produto não for encontrado
    }
}

// Função chamada quando o botão "Ok" é clicado para fechar a janela de detalhes
void DetalhesProduto::on_btnOk_clicked()
{
    this->accept();  // Fecha a janela de detalhes e sinaliza que a operação foi concluída com sucesso
}
