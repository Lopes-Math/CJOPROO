#include "cadastroproduto.h"
#include "ui_cadastroproduto.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>
#include <QDir>

// Construtor da janela de cadastro de produto
CadastroProduto::CadastroProduto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CadastroProduto)  // Inicializa a interface gráfica do cadastro
{
    ui->setupUi(this);  // Configura a UI
    setWindowTitle("Cadastro de Produto");  // Define o título da janela
}

// Destruidor da classe, limpa a memória alocada para a interface
CadastroProduto::~CadastroProduto()
{
    delete ui;
}

// Função chamada quando o usuário clica no botão "Cadastrar"
void CadastroProduto::on_cadastrarButton_clicked()
{
    QFile file("produtos.json");  // Abre o arquivo JSON onde os produtos são armazenados

    // Exibe o caminho atual do diretório de trabalho no console (para fins de debug)
    qDebug() << "Salvando em:" << QDir::currentPath();

    // Obtém os dados preenchidos pelo usuário na interface
    QString nome = ui->nomeLineEdit->text();
    QString grupo = ui->grupoComboBox->currentText();
    QString codigoBarras = ui->codigoBarrasLineEdit->text();
    QString marca = ui->marcaLineEdit->text();
    QString descricao = ui->descricaoTextEdit->toPlainText();
    int quantidade = ui->quantidadeSpinBox->value();
    double preco = ui->precoDoubleSpinBox->value();

    // Valida se os campos obrigatórios estão preenchidos corretamente
    if (nome.isEmpty() || grupo == "Selecione o grupo" || codigoBarras.isEmpty() || marca.isEmpty()) {
        // Se algum campo obrigatório não foi preenchido, exibe um aviso e interrompe a execução
        QMessageBox::warning(this, "Campos Obrigatórios", "Preencha todos os campos obrigatórios.");
        return;
    }

    // Lê o arquivo JSON existente e carrega os produtos
    QJsonArray produtos;
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());  // Converte o conteúdo do arquivo para um documento JSON
        produtos = doc.array();  // Converte o documento JSON para um array de produtos
        file.close();
    }

    // Cria um novo objeto JSON para o produto a ser adicionado
    QJsonObject novoProduto;
    novoProduto["id"] = gerarProximoId();  // Gera um ID único para o produto
    novoProduto["nome"] = nome;
    novoProduto["grupo"] = grupo;
    novoProduto["codigo_barras"] = codigoBarras;
    novoProduto["marca"] = marca;
    novoProduto["descricao"] = descricao;
    novoProduto["quantidade"] = quantidade;
    novoProduto["preco"] = preco;

    // Adiciona o novo produto ao array de produtos
    produtos.append(novoProduto);

    // Abre o arquivo JSON para escrita e salva os dados atualizados
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(produtos);  // Cria um novo documento JSON com os produtos atualizados
        file.write(doc.toJson());  // Escreve o documento no arquivo
        file.close();

        // Exibe uma mensagem de sucesso para o usuário
        QMessageBox::information(this, "Sucesso", "Produto cadastrado com sucesso!");

        // Fecha a janela de cadastro e sinaliza sucesso
        accept();
    } else {
        // Caso haja algum erro ao salvar o produto no arquivo, exibe uma mensagem de erro
        QMessageBox::critical(this, "Erro", "Erro ao salvar o produto no arquivo.");
    }
}

// Função para gerar o próximo ID único para o produto a ser cadastrado
int CadastroProduto::gerarProximoId()
{
    QFile file("produtos.json");  // Abre o arquivo de produtos para leitura
    int maxId = 0;  // Inicializa o ID máximo encontrado como 0

    // Abre o arquivo para leitura
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());  // Lê o conteúdo do arquivo JSON
        QJsonArray produtos = doc.array();  // Converte o documento em um array de produtos
        file.close();

        // Percorre todos os produtos para encontrar o maior ID
        for (const QJsonValue &val : produtos) {
            QJsonObject obj = val.toObject();
            if (obj.contains("id") && obj["id"].isDouble()) {  // Verifica se o produto tem o campo "id" e se ele é um número
                int id = obj["id"].toInt();  // Obtém o ID do produto
                if (id > maxId)  // Atualiza o máximo ID encontrado
                    maxId = id;
            }
        }
    }

    // Retorna o próximo ID disponível (maior ID encontrado + 1)
    return maxId + 1;
}
