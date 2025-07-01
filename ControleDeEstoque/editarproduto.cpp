#include "editarproduto.h"
#include "ui_editarproduto.h"  // Certifique-se que o nome do UI corresponde

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

// Construtor da classe EditarProduto
EditarProduto::EditarProduto(int produtoId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditarProduto),  // Usa a UI de EditarProduto
    m_produtoId(produtoId)  // Recebe o ID do produto a ser editado
{
    ui->setupUi(this);  // Configura a interface gráfica
    setWindowTitle("Editar Produto");  // Define o título da janela

    // Carrega os dados do produto existente nos campos da interface
    carregarDados();
}

// Destruidor da classe, libera a memória alocada para a interface
EditarProduto::~EditarProduto()
{
    delete ui;
}

// Função para carregar os dados do produto no momento da edição
void EditarProduto::carregarDados()
{
    QFile file("produtos.json");  // Abre o arquivo de produtos
    if (!file.open(QIODevice::ReadOnly)) return;  // Se não conseguir abrir o arquivo, retorna

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());  // Lê o conteúdo do arquivo e converte para JSON
    file.close();  // Fecha o arquivo após a leitura

    QJsonArray produtos = doc.array();  // Converte o documento JSON para um array de produtos
    // Percorre todos os produtos para encontrar o produto com o ID correspondente
    for (const QJsonValue &val : produtos) {
        QJsonObject obj = val.toObject();
        if (obj["id"].toInt() == m_produtoId) {  // Se o ID do produto corresponder ao ID fornecido
            // Preenche os campos da interface com os dados do produto
            ui->nomeLineEdit->setText(obj["nome"].toString());
            ui->marcaLineEdit->setText(obj["marca"].toString());
            ui->codigoBarrasLineEdit->setText(obj["codigo_barras"].toString());
            ui->descricaoTextEdit->setPlainText(obj["descricao"].toString());
            ui->precoDoubleSpinBox->setValue(obj["preco"].toDouble());
            ui->quantidadeSpinBox->setValue(obj["quantidade"].toInt());

            // Encontra e seleciona o item no ComboBox de grupos
            int index = ui->grupoComboBox->findText(obj["grupo"].toString());
            if (index != -1) {
                ui->grupoComboBox->setCurrentIndex(index);  // Define o grupo selecionado no ComboBox
            }
            break;  // Sai do loop após encontrar o produto
        }
    }
}

// Função chamada quando o botão "Salvar" é clicado
void EditarProduto::on_salvarButton_clicked()
{
    // 1. Obtém os novos dados da interface
    QString nome = ui->nomeLineEdit->text();
    QString grupo = ui->grupoComboBox->currentText();
    QString codigoBarras = ui->codigoBarrasLineEdit->text();
    QString marca = ui->marcaLineEdit->text();
    QString descricao = ui->descricaoTextEdit->toPlainText();
    double preco = ui->precoDoubleSpinBox->value();

    // Valida se os campos obrigatórios estão preenchidos corretamente
    if (nome.isEmpty() || grupo == "Selecione o grupo" || codigoBarras.isEmpty() || marca.isEmpty()) {
        QMessageBox::warning(this, "Campos Obrigatórios", "Preencha todos os campos obrigatórios.");
        return;  // Se algum campo obrigatório estiver vazio, exibe um aviso e interrompe o processo
    }

    // 2. Abre o arquivo JSON para leitura
    QFile file("produtos.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Erro", "Não foi possível ler o arquivo de produtos.");
        return;  // Se não conseguir abrir o arquivo, exibe uma mensagem de erro e retorna
    }
    QJsonArray produtos = QJsonDocument::fromJson(file.readAll()).array();  // Lê e converte os dados em um array de produtos
    file.close();  // Fecha o arquivo após a leitura

    // 3. Procura o produto com o ID fornecido e atualiza seus dados
    for (int i = 0; i < produtos.size(); ++i) {
        QJsonObject obj = produtos[i].toObject();
        if (obj["id"].toInt() == m_produtoId) {  // Se o ID do produto corresponder ao ID fornecido
            // Atualiza os dados do produto com os novos valores fornecidos pelo usuário
            obj["nome"] = nome;
            obj["grupo"] = grupo;
            obj["codigo_barras"] = codigoBarras;
            obj["marca"] = marca;
            obj["descricao"] = descricao;
            obj["preco"] = preco;
            // A quantidade não é alterada aqui, ela permanece a mesma

            // Substitui o objeto antigo pelo novo no array
            produtos[i] = obj;
            break;  // Sai do loop após encontrar o produto e atualizar seus dados
        }
    }

    // 4. Salva o arquivo JSON modificado
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Erro", "Não foi possível salvar as alterações.");
        return;  // Se não conseguir abrir o arquivo para escrita, exibe uma mensagem de erro e retorna
    }
    file.write(QJsonDocument(produtos).toJson());  // Escreve o array atualizado no arquivo
    file.close();  // Fecha o arquivo após a escrita

    // Exibe uma mensagem de sucesso após salvar as alterações
    QMessageBox::information(this, "Sucesso", "Produto atualizado com sucesso!");
    accept();  // Fecha a janela e sinaliza sucesso
}
