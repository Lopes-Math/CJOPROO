#include "listagemprodutos.h"
#include "ui_listagemprodutos.h"
#include "cadastroproduto.h"
#include "detalhesproduto.h"
#include "editarproduto.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include <QHeaderView>

// Construtor da classe ListagemProdutos
ListagemProdutos::ListagemProdutos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListagemProdutos)  // Inicializa a interface gráfica para a listagem de produtos
{
    ui->setupUi(this);  // Configura a interface gráfica
    setWindowTitle("Listagem de Produtos");  // Define o título da janela

    // Configurações iniciais
    preencherGruposNoComboBox();  // Preenche o ComboBox de grupos com os dados dos produtos
    carregarDadosNaTabela();  // Carrega os produtos e os exibe na tabela

    // Conexões para os filtros
    connect(ui->buscaLineEdit, &QLineEdit::textChanged, this, &ListagemProdutos::filtrarTabela);
    connect(ui->grupoComboBox, &QComboBox::currentTextChanged, this, &ListagemProdutos::filtrarTabela);

    // Ajusta o tamanho das colunas da tabela
    ui->tabelaProdutos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Coluna "Nome"
    ui->tabelaProdutos->setColumnWidth(0, 40);  // ID
    ui->tabelaProdutos->setColumnWidth(4, 50);  // Qtd
    ui->tabelaProdutos->setColumnWidth(5, 80);  // Preço
    ui->tabelaProdutos->setColumnWidth(6, 90);  // Botão "Ver Mais"
    ui->tabelaProdutos->setColumnWidth(7, 90);  // Botão "Editar"
    ui->tabelaProdutos->setColumnWidth(8, 90);  // Botão "Excluir"
}

// Destruidor da classe, libera a memória da interface
ListagemProdutos::~ListagemProdutos()
{
    delete ui;
}

// Função para carregar os dados dos produtos a partir do arquivo JSON
QJsonArray ListagemProdutos::carregarProdutos()
{
    QFile file("produtos.json");  // Abre o arquivo de produtos
    QJsonArray produtos;
    if (file.open(QIODevice::ReadOnly)) {
        produtos = QJsonDocument::fromJson(file.readAll()).array();  // Lê os dados e os converte para um array JSON
        file.close();
    }
    return produtos;  // Retorna o array de produtos carregado
}

// Função para salvar os produtos de volta no arquivo JSON
bool ListagemProdutos::salvarProdutos(const QJsonArray &produtos)
{
    QFile file("produtos.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(produtos).toJson());  // Converte o array de produtos para JSON e escreve no arquivo
        file.close();
        return true;  // Retorna verdadeiro se o arquivo for salvo com sucesso
    }
    return false;  // Retorna falso se não conseguir salvar
}

// Função para preencher o ComboBox de grupos com os dados dos produtos
void ListagemProdutos::preencherGruposNoComboBox()
{
    QJsonArray produtos = carregarProdutos();  // Carrega os dados dos produtos
    QSet<QString> gruposSet;  // Usa um QSet para garantir que não haja grupos duplicados
    gruposSet.insert("Todos");  // Adiciona a opção "Todos" ao ComboBox

    // Adiciona os grupos dos produtos ao QSet
    for (auto val : produtos) {
        QJsonObject obj = val.toObject();
        QString grupo = obj["grupo"].toString();
        if (!grupo.isEmpty()) {
            gruposSet.insert(grupo);  // Adiciona o grupo se não for vazio
        }
    }

    // Preenche o ComboBox com os grupos encontrados
    ui->grupoComboBox->clear();
    for (const QString &grupo : std::as_const(gruposSet)) {
        ui->grupoComboBox->addItem(grupo);  // Adiciona o grupo ao ComboBox
    }
    ui->grupoComboBox->setCurrentText("Todos");  // Define "Todos" como o grupo selecionado por padrão
}

// Função para filtrar os produtos na tabela com base na busca e no grupo selecionado
void ListagemProdutos::filtrarTabela()
{
    carregarDadosNaTabela();  // Carrega os dados filtrados na tabela
}

// Função para carregar os dados dos produtos na tabela
void ListagemProdutos::carregarDadosNaTabela()
{
    QString filtro = ui->buscaLineEdit->text().trimmed();  // Obtém o texto de busca
    QString grupoSelecionado = ui->grupoComboBox->currentText();  // Obtém o grupo selecionado no ComboBox

    QJsonArray todosProdutos = carregarProdutos();  // Carrega todos os produtos do arquivo JSON
    ui->tabelaProdutos->setRowCount(0);  // Limpa a tabela antes de preencher com os dados filtrados

    // Configura os cabeçalhos da tabela
    QStringList headers = {"ID", "Nome", "Grupo", "Marca", "Qtd", "Preço", "", "", ""};
    ui->tabelaProdutos->setColumnCount(headers.size());
    ui->tabelaProdutos->setHorizontalHeaderLabels(headers);

    // Ajusta o tamanho das colunas após configurar os cabeçalhos
    ui->tabelaProdutos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tabelaProdutos->setColumnWidth(0, 40); ui->tabelaProdutos->setColumnWidth(4, 50);
    ui->tabelaProdutos->setColumnWidth(5, 80); ui->tabelaProdutos->setColumnWidth(6, 90);
    ui->tabelaProdutos->setColumnWidth(7, 90); ui->tabelaProdutos->setColumnWidth(8, 90);

    int linha = 0;
    // Loop para filtrar e carregar os dados na tabela
    for (const auto &val : todosProdutos) {
        auto p = val.toObject();
        QString nome = p["nome"].toString();
        QString marca = p["marca"].toString();
        QString grupo = p["grupo"].toString();
        QString codigo = p["codigo_barras"].toString();

        bool matchTexto = filtro.isEmpty() ||
                          nome.contains(filtro, Qt::CaseInsensitive) ||
                          marca.contains(filtro, Qt::CaseInsensitive) ||
                          codigo.contains(filtro, Qt::CaseInsensitive);

        bool matchGrupo = (grupoSelecionado == "Todos") || (grupo == grupoSelecionado);

        if (matchTexto && matchGrupo) {
            ui->tabelaProdutos->insertRow(linha);  // Adiciona uma nova linha na tabela
            int idProduto = p["id"].toInt();

            // Preenche as colunas com os dados do produto
            for (int col = 0; col < 6; ++col) {
                QString data;
                switch(col) {
                case 0: data = QString::number(idProduto); break;
                case 1: data = nome; break;
                case 2: data = grupo; break;
                case 3: data = marca; break;
                case 4: data = QString::number(p["quantidade"].toInt()); break;
                case 5: data = QString::number(p["preco"].toDouble(), 'f', 2); break;
                }
                QTableWidgetItem *item = new QTableWidgetItem(data);
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);  // Torna as células não editáveis
                ui->tabelaProdutos->setItem(linha, col, item);
            }

            // Adiciona os botões de ação ("Ver Mais", "Editar", "Excluir")
            adicionarBotoesAcao(linha, idProduto);

            linha++;  // Incrementa o número da linha
        }
    }
}

// Função para adicionar os botões de ação ("Ver Mais", "Editar", "Excluir") na tabela
void ListagemProdutos::adicionarBotoesAcao(int linha, int produtoId)
{
    // Botão "Ver Mais"
    QPushButton *btnVerMais = new QPushButton("Ver Mais", this);
    connect(btnVerMais, &QPushButton::clicked, this, [this, produtoId](){
        onVerMaisClicked(produtoId);
    });
    ui->tabelaProdutos->setCellWidget(linha, 6, btnVerMais);

    // Botão "Editar"
    QPushButton *btnEditar = new QPushButton("Editar", this);
    connect(btnEditar, &QPushButton::clicked, this, [this, produtoId](){
        onEditarClicked(produtoId);
    });
    ui->tabelaProdutos->setCellWidget(linha, 7, btnEditar);

    // Botão "Excluir"
    QPushButton *btnExcluir = new QPushButton("Excluir", this);
    btnExcluir->setStyleSheet("color: red;");
    connect(btnExcluir, &QPushButton::clicked, this, [this, produtoId](){
        onExcluirClicked(produtoId);
    });
    ui->tabelaProdutos->setCellWidget(linha, 8, btnExcluir);
}

// Função chamada quando o botão "Cadastrar Produto" é clicado
void ListagemProdutos::on_btnCadastrarProduto_clicked()
{
    this->hide();  // Esconde a janela de listagem

    // Cria e executa a tela de cadastro de produto como um diálogo modal
    CadastroProduto *formCadastro = new CadastroProduto(this);
    int resultado = formCadastro->exec();  // Bloqueia a execução até o diálogo ser fechado

    delete formCadastro;  // Libera a memória do diálogo de cadastro

    this->show();  // Mostra novamente a janela de listagem

    // Se o cadastro for aceito, atualiza a tabela e os filtros
    if (resultado == QDialog::Accepted) {
        carregarDadosNaTabela();
        preencherGruposNoComboBox();  // Atualiza os filtros de grupo
    }
}

// Função chamada quando o botão "Ver Mais" é clicado
void ListagemProdutos::onVerMaisClicked(int produtoId)
{
    DetalhesProduto *telaDetalhes = new DetalhesProduto(produtoId, this);
    telaDetalhes->exec();  // Exibe a tela de detalhes do produto como um diálogo modal
    delete telaDetalhes;  // Libera a memória após o fechamento
}

// Função chamada quando o botão "Editar" é clicado
void ListagemProdutos::onEditarClicked(int produtoId)
{
    EditarProduto *telaEditar = new EditarProduto(produtoId, this);
    if (telaEditar->exec() == QDialog::Accepted) {  // Se as alterações foram salvas
        carregarDadosNaTabela();  // Atualiza a tabela com os dados modificados
    }
    delete telaEditar;  // Libera a memória da tela de edição
}

// Função chamada quando o botão "Excluir" é clicado
void ListagemProdutos::onExcluirClicked(int produtoId)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Excluir Produto",
                                  QString("Tem certeza que deseja excluir o produto com ID %1? Esta ação não pode ser desfeita.").arg(produtoId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QJsonArray produtos = carregarProdutos();  // Carrega os produtos
        QJsonArray novosProdutos;
        bool encontrado = false;

        // Percorre os produtos e remove o produto selecionado
        for (const QJsonValue &value : produtos) {
            if (value.toObject()["id"].toInt() == produtoId) {
                encontrado = true;
            } else {
                novosProdutos.append(value);  // Adiciona os produtos não removidos
            }
        }

        // Se o produto foi encontrado e removido, salva os dados atualizados no arquivo
        if (encontrado && salvarProdutos(novosProdutos)) {
            QMessageBox::information(this, "Sucesso", "Produto excluído com sucesso.");
            carregarDadosNaTabela();  // Atualiza a tabela para refletir a exclusão
        } else {
            QMessageBox::warning(this, "Erro", "Não foi possível excluir o produto.");
        }
    }
}

// Função chamada quando o botão "Voltar" é clicado
void ListagemProdutos::on_btnVoltar_clicked()
{
    this->reject();  // Fecha a janela de listagem
}
