#include "controleestoque.h"
#include "ui_controleestoque.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QMap>
#include <QSet>
#include <utility> // Necessário para std::as_const

ControleEstoque::ControleEstoque(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControleEstoque)
{
    ui->setupUi(this);
    setWindowTitle("Controle de Estoque");

    // Inicializa e preenche as tabelas e os ComboBoxes
    carregarTabela(); // Carrega a tabela de consulta (ui->tabelaProdutos)
    preencherTabelaMovimentacao(); // Carrega a tabela de movimentação (ui->movimentacaoTableWidget)
    preencherGruposNoComboBox(); // Preenche AMBOS os ComboBoxes de grupo e define "Todos" como padrão

    // Conexões para a Tabela de Consulta (ui->tabelaProdutos) - FILTROS INDEPENDENTES
    connect(ui->buscaLineEdit, &QLineEdit::textChanged, this, &ControleEstoque::filtrarTabelaConsulta);
    connect(ui->grupoComboBox, &QComboBox::currentTextChanged, this, &ControleEstoque::filtrarTabelaConsulta);
    //connect(ui->btnVoltarConsulta, &QPushButton::clicked, this, &ControleEstoque::on_btnVoltarConsulta_clicked); // Botão Voltar da consulta

    // Conexões para a Tabela de Movimentação (ui->movimentacaoTableWidget) - FILTROS INDEPENDENTES
    connect(ui->buscaMovimentacaoLineEdit, &QLineEdit::textChanged, this, &ControleEstoque::filtrarTabelaMovimentacao);
    connect(ui->grupoMovimentacaoComboBox, &QComboBox::currentTextChanged, this, &ControleEstoque::filtrarTabelaMovimentacao);
    connect(ui->confirmarMovimentacaoButton, &QPushButton::clicked, this, &ControleEstoque::on_confirmarMovimentacaoButton_clicked);
    connect(ui->movimentacaoTableWidget, &QTableWidget::cellChanged, this, &ControleEstoque::on_movimentacaoTableWidget_cellChanged);
    //connect(ui->btnVoltar, &QPushButton::clicked, this, &ControleEstoque::on_btnVoltar_clicked); // Botão Voltar da movimentação
}

ControleEstoque::~ControleEstoque()
{
    delete ui;
}

QJsonArray ControleEstoque::carregarProdutos()
{
    QFile file("produtos.json");
    QJsonArray produtos;
    if (file.open(QIODevice::ReadOnly)) {
        produtos = QJsonDocument::fromJson(file.readAll()).array();
        file.close();
    }
    return produtos;
}

bool ControleEstoque::salvarProdutos(const QJsonArray &produtos)
{
    QFile file("produtos.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(produtos).toJson());
        file.close();
        return true;
    }
    return false;
}

// Carrega e exibe todos os produtos na ui->tabelaProdutos (tabela de consulta)
void ControleEstoque::carregarTabela()
{
    QJsonArray produtos = carregarProdutos();
    ui->tabelaProdutos->setRowCount(0);
    QStringList headers = {"ID", "Nome", "Grupo", "Marca", "Qtd", "Preço"};
    ui->tabelaProdutos->setColumnCount(headers.size());
    ui->tabelaProdutos->setHorizontalHeaderLabels(headers);

    // Desativa a edição de todas as células na tabela de consulta por padrão
    ui->tabelaProdutos->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int linha = 0;
    for (auto val : produtos) {
        auto p = val.toObject();
        ui->tabelaProdutos->insertRow(linha);
        // Cria os QTableWidgetItem e define as flags para serem não editáveis
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(p["id"].toInt()));
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable); // Remove a flag de editável
        ui->tabelaProdutos->setItem(linha, 0, idItem);

        QTableWidgetItem *nomeItem = new QTableWidgetItem(p["nome"].toString());
        nomeItem->setFlags(nomeItem->flags() & ~Qt::ItemIsEditable);
        ui->tabelaProdutos->setItem(linha, 1, nomeItem);

        QTableWidgetItem *grupoItem = new QTableWidgetItem(p["grupo"].toString());
        grupoItem->setFlags(grupoItem->flags() & ~Qt::ItemIsEditable);
        ui->tabelaProdutos->setItem(linha, 2, grupoItem);

        QTableWidgetItem *marcaItem = new QTableWidgetItem(p["marca"].toString());
        marcaItem->setFlags(marcaItem->flags() & ~Qt::ItemIsEditable);
        ui->tabelaProdutos->setItem(linha, 3, marcaItem);

        QTableWidgetItem *qtdItem = new QTableWidgetItem(QString::number(p["quantidade"].toInt()));
        qtdItem->setFlags(qtdItem->flags() & ~Qt::ItemIsEditable);
        ui->tabelaProdutos->setItem(linha, 4, qtdItem);

        QTableWidgetItem *precoItem = new QTableWidgetItem(QString::number(p["preco"].toDouble(), 'f', 2));
        precoItem->setFlags(precoItem->flags() & ~Qt::ItemIsEditable);
        ui->tabelaProdutos->setItem(linha, 5, precoItem);

        linha++;
    }
}

// Preenche AMBOS os ComboBoxes de grupos com base nos produtos existentes e define "Todos" como padrão
void ControleEstoque::preencherGruposNoComboBox()
{
    QJsonArray produtos = carregarProdutos();
    QSet<QString> gruposSet;

    // Adiciona "Todos" como primeira opção no conjunto
    gruposSet.insert("Todos");

    // Adiciona os grupos existentes dos produtos ao conjunto
    for (auto val : produtos) {
        QJsonObject obj = val.toObject();
        QString grupo = obj["grupo"].toString();
        if (!grupo.isEmpty()) { // Garante que grupos vazios não sejam adicionados
            gruposSet.insert(grupo);
        }
    }

    // --- Preenche o ComboBox da TELA DE CONSULTA (ui->grupoComboBox) ---
    ui->grupoComboBox->clear();
    ui->grupoComboBox->addItem("Todos"); // Adiciona "Todos" explicitamente primeiro
    for (const QString &grupo : std::as_const(gruposSet)) {
        if (grupo != "Todos") { // Evita adicionar "Todos" duplicadamente se já foi adicionado
            ui->grupoComboBox->addItem(grupo);
        }
    }
    // Define "Todos" como o item selecionado
    int indexConsulta = ui->grupoComboBox->findText("Todos");
    if (indexConsulta != -1) {
        ui->grupoComboBox->setCurrentIndex(indexConsulta);
    } else {
        ui->grupoComboBox->setCurrentIndex(0); // Fallback para o primeiro item
    }

    // --- Preenche o ComboBox da TELA DE MOVIMENTAÇÃO (ui->grupoMovimentacaoComboBox) ---
    // Fazemos a mesma lógica para o novo ComboBox de movimentação
    ui->grupoMovimentacaoComboBox->clear();
    ui->grupoMovimentacaoComboBox->addItem("Todos"); // Adiciona "Todos" explicitamente primeiro
    for (const QString &grupo : std::as_const(gruposSet)) {
        if (grupo != "Todos") { // Evita adicionar "Todos" duplicadamente se já foi adicionado
            ui->grupoMovimentacaoComboBox->addItem(grupo);
        }
    }
    // Define "Todos" como o item selecionado
    int indexMovimentacao = ui->grupoMovimentacaoComboBox->findText("Todos");
    if (indexMovimentacao != -1) {
        ui->grupoMovimentacaoComboBox->setCurrentIndex(indexMovimentacao);
    } else {
        ui->grupoMovimentacaoComboBox->setCurrentIndex(0); // Fallback para o primeiro item
    }
}

// --- Funções de Filtro para a Tabela de Consulta (ui->tabelaProdutos) ---
void ControleEstoque::filtrarTabelaConsulta()
{
    QString filtro = ui->buscaLineEdit->text().trimmed(); // Termo de busca da tela de consulta
    QString grupoSelecionado = ui->grupoComboBox->currentText(); // Grupo selecionado da tela de consulta

    QJsonArray todosProdutos = carregarProdutos(); // Carrega todos os produtos novamente
    ui->tabelaProdutos->setRowCount(0); // Limpa a tabela de consulta

    // Desativa a edição de todas as células na tabela de consulta (garante consistência)
    ui->tabelaProdutos->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList headers = {"ID", "Nome", "Grupo", "Marca", "Qtd", "Preço"};
    ui->tabelaProdutos->setColumnCount(headers.size());
    ui->tabelaProdutos->setHorizontalHeaderLabels(headers);

    int linha = 0;
    for (auto val : todosProdutos) {
        auto p = val.toObject();
        QString nome = p["nome"].toString();
        QString marca = p["marca"].toString();
        QString grupo = p["grupo"].toString();
        QString codigo = p["codigo_barras"].toString();

        // Lógica de filtro combinada: busca por texto E filtro por grupo
        bool matchTexto = filtro.isEmpty() ||
                          nome.contains(filtro, Qt::CaseInsensitive) ||
                          marca.contains(filtro, Qt::CaseInsensitive) ||
                          codigo.contains(filtro, Qt::CaseInsensitive);

        bool matchGrupo = (grupoSelecionado == "Todos") || (grupo == grupoSelecionado);

        if (matchTexto && matchGrupo) {
            ui->tabelaProdutos->insertRow(linha);
            // Cria os QTableWidgetItem e define as flags para serem não editáveis
            QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(p["id"].toInt()));
            idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
            ui->tabelaProdutos->setItem(linha, 0, idItem);

            QTableWidgetItem *nomeItem = new QTableWidgetItem(nome);
            nomeItem->setFlags(nomeItem->flags() & ~Qt::ItemIsEditable);
            ui->tabelaProdutos->setItem(linha, 1, nomeItem);

            QTableWidgetItem *grupoItem = new QTableWidgetItem(grupo);
            grupoItem->setFlags(grupoItem->flags() & ~Qt::ItemIsEditable);
            ui->tabelaProdutos->setItem(linha, 2, grupoItem);

            QTableWidgetItem *marcaItem = new QTableWidgetItem(marca);
            marcaItem->setFlags(marcaItem->flags() & ~Qt::ItemIsEditable);
            ui->tabelaProdutos->setItem(linha, 3, marcaItem);

            QTableWidgetItem *qtdItem = new QTableWidgetItem(QString::number(p["quantidade"].toInt()));
            qtdItem->setFlags(qtdItem->flags() & ~Qt::ItemIsEditable);
            ui->tabelaProdutos->setItem(linha, 4, qtdItem);

            QTableWidgetItem *precoItem = new QTableWidgetItem(QString::number(p["preco"].toDouble(), 'f', 2));
            precoItem->setFlags(precoItem->flags() & ~Qt::ItemIsEditable);
            ui->tabelaProdutos->setItem(linha, 5, precoItem);

            linha++;
        }
    }
}

// Slot que é chamado quando o texto na barra de busca da tela de consulta muda
void ControleEstoque::on_buscaLineEdit_textChanged(const QString &text)
{
    Q_UNUSED(text); // Para evitar warning de variável não usada
    filtrarTabelaConsulta();
}

// Slot para o botão "Voltar" na aba de Consulta
void ControleEstoque::on_btnVoltarConsulta_clicked()
{
    this->reject(); // ALTERADO: Usando reject() para encerrar o diálogo modal corretamente
}

// --- Funções para a Tabela de Movimentação (ui->movimentacaoTableWidget) ---

// Preenche a tabela de movimentação com os produtos e seus dados de movimentação persistentes
void ControleEstoque::preencherTabelaMovimentacao()
{
    produtosMemoria = carregarProdutos(); // Carrega os produtos em memória para a tabela de movimentação

    ui->movimentacaoTableWidget->setRowCount(0);
    QStringList headers = {"ID", "Nome", "Grupo", "Marca", "Qtd Atual", "Preço", "Qtd Entrada", "Qtd Saída"};
    ui->movimentacaoTableWidget->setColumnCount(headers.size());
    ui->movimentacaoTableWidget->setHorizontalHeaderLabels(headers);

    // Desconecte temporariamente o sinal para evitar que cellChanged seja disparado durante o preenchimento
    disconnect(ui->movimentacaoTableWidget, &QTableWidget::cellChanged, this, &ControleEstoque::on_movimentacaoTableWidget_cellChanged);

    int linha = 0;
    for (int i = 0; i < produtosMemoria.size(); ++i) {
        auto obj = produtosMemoria[i].toObject();
        int id = obj["id"].toInt();
        QString nome = obj["nome"].toString();
        QString grupo = obj["grupo"].toString();
        QString marca = obj["marca"].toString();
        int quantidade = obj["quantidade"].toInt();
        double preco = obj["preco"].toDouble();

        ui->movimentacaoTableWidget->insertRow(linha);

        // ID (Coluna 0) - Não editável
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(id));
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        ui->movimentacaoTableWidget->setItem(linha, 0, idItem);

        // Nome (Coluna 1) - Não editável
        QTableWidgetItem *nomeItem = new QTableWidgetItem(nome);
        nomeItem->setFlags(nomeItem->flags() & ~Qt::ItemIsEditable);
        ui->movimentacaoTableWidget->setItem(linha, 1, nomeItem);

        // Grupo (Coluna 2) - Não editável
        QTableWidgetItem *grupoItem = new QTableWidgetItem(grupo);
        grupoItem->setFlags(grupoItem->flags() & ~Qt::ItemIsEditable);
        ui->movimentacaoTableWidget->setItem(linha, 2, grupoItem);

        // Marca (Coluna 3) - Não editável
        QTableWidgetItem *marcaItem = new QTableWidgetItem(marca);
        marcaItem->setFlags(marcaItem->flags() & ~Qt::ItemIsEditable);
        ui->movimentacaoTableWidget->setItem(linha, 3, marcaItem);

        // Qtd Atual (Coluna 4) - Não editável
        QTableWidgetItem *qtdAtualItem = new QTableWidgetItem(QString::number(quantidade));
        qtdAtualItem->setFlags(qtdAtualItem->flags() & ~Qt::ItemIsEditable);
        ui->movimentacaoTableWidget->setItem(linha, 4, qtdAtualItem);

        // Preço (Coluna 5) - Não editável
        QTableWidgetItem *precoItem = new QTableWidgetItem(QString::number(preco, 'f', 2));
        precoItem->setFlags(precoItem->flags() & ~Qt::ItemIsEditable);
        ui->movimentacaoTableWidget->setItem(linha, 5, precoItem);

        // Recupera os valores de entrada/saída salvos do mapa persistente
        QPair<int, int> savedValues = m_movimentacaoData.value(id, qMakePair(0, 0));

        // Qtd Entrada (Coluna 6) - EDITÁVEL
        QTableWidgetItem *qtdEntradaItem = new QTableWidgetItem(savedValues.first ? QString::number(savedValues.first) : "");
        qtdEntradaItem->setFlags(qtdEntradaItem->flags() | Qt::ItemIsEditable); // Garante que é editável
        ui->movimentacaoTableWidget->setItem(linha, 6, qtdEntradaItem);

        // Qtd Saída (Coluna 7) - EDITÁVEL
        QTableWidgetItem *qtdSaidaItem = new QTableWidgetItem(savedValues.second ? QString::number(savedValues.second) : "");
        qtdSaidaItem->setFlags(qtdSaidaItem->flags() | Qt::ItemIsEditable); // Garante que é editável
        ui->movimentacaoTableWidget->setItem(linha, 7, qtdSaidaItem);

        linha++;
    }
    // Reconecte o sinal após o preenchimento
    connect(ui->movimentacaoTableWidget, &QTableWidget::cellChanged, this, &ControleEstoque::on_movimentacaoTableWidget_cellChanged);
}

// Filtra a tabela de MOVIMENTAÇÃO (ui->movimentacaoTableWidget)
void ControleEstoque::filtrarTabelaMovimentacao()
{
    // Desconecte temporariamente o sinal para evitar que cellChanged seja disparado
    disconnect(ui->movimentacaoTableWidget, &QTableWidget::cellChanged, this, &ControleEstoque::on_movimentacaoTableWidget_cellChanged);

    // 1. Salvar os valores atuais das colunas de Entrada e Saída no mapa m_movimentacaoData
    for (int i = 0; i < ui->movimentacaoTableWidget->rowCount(); ++i) {
        QTableWidgetItem *idItem = ui->movimentacaoTableWidget->item(i, 0);
        QTableWidgetItem *entradaItem = ui->movimentacaoTableWidget->item(i, 6);
        QTableWidgetItem *saidaItem = ui->movimentacaoTableWidget->item(i, 7);

        if (idItem) {
            int id = idItem->text().toInt();
            int entrada = entradaItem ? entradaItem->text().toInt() : 0;
            int saida = saidaItem ? saidaItem->text().toInt() : 0;
            m_movimentacaoData[id] = qMakePair(entrada, saida);
        }
    }

    QString filtro = ui->buscaMovimentacaoLineEdit->text().trimmed(); // Termo de busca da tela de movimentação
    QString grupoSelecionado = ui->grupoMovimentacaoComboBox->currentText(); // Grupo selecionado da tela de movimentação

    QJsonArray todosProdutos = produtosMemoria; // Use a lista de produtos em memória
    ui->movimentacaoTableWidget->setRowCount(0); // Limpa a tabela de movimentação

    int linha = 0;
    for (auto val : todosProdutos) {
        QJsonObject obj = val.toObject();
        QString nome = obj["nome"].toString();
        QString marca = obj["marca"].toString();
        QString grupo = obj["grupo"].toString();
        QString codigo = obj["codigo_barras"].toString();

        // Lógica de filtro combinada: busca por texto E filtro por grupo
        bool matchTexto = filtro.isEmpty() ||
                          nome.contains(filtro, Qt::CaseInsensitive) ||
                          marca.contains(filtro, Qt::CaseInsensitive) ||
                          codigo.contains(filtro, Qt::CaseInsensitive);

        bool matchGrupo = (grupoSelecionado == "Todos") || (grupo == grupoSelecionado);

        if (matchTexto && matchGrupo) {
            ui->movimentacaoTableWidget->insertRow(linha);
            int id = obj["id"].toInt();

            // ID (Coluna 0) - Não editável
            QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(id));
            idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
            ui->movimentacaoTableWidget->setItem(linha, 0, idItem);

            // Nome (Coluna 1) - Não editável
            QTableWidgetItem *nomeItem = new QTableWidgetItem(nome);
            nomeItem->setFlags(nomeItem->flags() & ~Qt::ItemIsEditable);
            ui->movimentacaoTableWidget->setItem(linha, 1, nomeItem);

            // Grupo (Coluna 2) - Não editável
            QTableWidgetItem *grupoItem = new QTableWidgetItem(grupo);
            grupoItem->setFlags(grupoItem->flags() & ~Qt::ItemIsEditable);
            ui->movimentacaoTableWidget->setItem(linha, 2, grupoItem);

            // Marca (Coluna 3) - Não editável
            QTableWidgetItem *marcaItem = new QTableWidgetItem(marca);
            marcaItem->setFlags(marcaItem->flags() & ~Qt::ItemIsEditable);
            ui->movimentacaoTableWidget->setItem(linha, 3, marcaItem);

            // Qtd Atual (Coluna 4) - Não editável
            QTableWidgetItem *qtdAtualItem = new QTableWidgetItem(QString::number(obj["quantidade"].toInt()));
            qtdAtualItem->setFlags(qtdAtualItem->flags() & ~Qt::ItemIsEditable);
            ui->movimentacaoTableWidget->setItem(linha, 4, qtdAtualItem);

            // Preço (Coluna 5) - Não editável
            QTableWidgetItem *precoItem = new QTableWidgetItem(QString::number(obj["preco"].toDouble(), 'f', 2));
            precoItem->setFlags(precoItem->flags() & ~Qt::ItemIsEditable);
            ui->movimentacaoTableWidget->setItem(linha, 5, precoItem);

            // Restaure os valores de entrada e saída do mapa persistente
            QPair<int, int> valores = m_movimentacaoData.value(id, qMakePair(0, 0));

            // Qtd Entrada (Coluna 6) - EDITÁVEL
            QTableWidgetItem *qtdEntradaItem = new QTableWidgetItem(valores.first ? QString::number(valores.first) : "");
            qtdEntradaItem->setFlags(qtdEntradaItem->flags() | Qt::ItemIsEditable);
            ui->movimentacaoTableWidget->setItem(linha, 6, qtdEntradaItem);

            // Qtd Saída (Coluna 7) - EDITÁVEL
            QTableWidgetItem *qtdSaidaItem = new QTableWidgetItem(valores.second ? QString::number(valores.second) : "");
            qtdSaidaItem->setFlags(qtdSaidaItem->flags() | Qt::ItemIsEditable);
            ui->movimentacaoTableWidget->setItem(linha, 7, qtdSaidaItem);

            linha++;
        }
    }
    // Reconecte o sinal após o preenchimento
    connect(ui->movimentacaoTableWidget, &QTableWidget::cellChanged, this, &ControleEstoque::on_movimentacaoTableWidget_cellChanged);
}

// Slot que é chamado quando o texto na barra de busca da tela de movimentação muda
void ControleEstoque::on_buscaMovimentacaoLineEdit_textChanged(const QString &text)
{
    Q_UNUSED(text); // Para evitar warning de variável não usada
    filtrarTabelaMovimentacao();
}

// Slot que é chamado quando o grupo no ComboBox da tela de movimentação muda
void ControleEstoque::on_grupoMovimentacaoComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text); // Para evitar warning de variável não usada
    filtrarTabelaMovimentacao();
}

// Atualiza as quantidades na coluna "Qtd Atual" da tabela de movimentação
// após as movimentações serem confirmadas e salvas.
void ControleEstoque::atualizarQuantidadesNaTabela()
{
    QJsonArray produtos = carregarProdutos();

    for (int i = 0; i < ui->movimentacaoTableWidget->rowCount(); ++i) {
        int id = ui->movimentacaoTableWidget->item(i, 0)->text().toInt();

        for (const QJsonValue &val : produtos) {
            QJsonObject obj = val.toObject();
            if (obj["id"].toInt() == id) {
                ui->movimentacaoTableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(obj["quantidade"].toInt())));
                break;
            }
        }
    }
}

// Slot chamado quando uma célula na tabela de movimentação é alterada.
// Captura os valores de entrada/saída e os armazena no mapa persistente.
void ControleEstoque::on_movimentacaoTableWidget_cellChanged(int row, int column)
{
    // Verifique se a mudança ocorreu nas colunas de Entrada (6) ou Saída (7)
    if (column == 6 || column == 7) {
        // Desconecte temporariamente o sinal para evitar recursão infinita ao atualizar a célula
        disconnect(ui->movimentacaoTableWidget, &QTableWidget::cellChanged, this, &ControleEstoque::on_movimentacaoTableWidget_cellChanged);

        int id = ui->movimentacaoTableWidget->item(row, 0)->text().toInt();
        int entrada = ui->movimentacaoTableWidget->item(row, 6) ? ui->movimentacaoTableWidget->item(row, 6)->text().toInt() : 0;
        int saida = ui->movimentacaoTableWidget->item(row, 7) ? ui->movimentacaoTableWidget->item(row, 7)->text().toInt() : 0;

        // Armazena os valores no mapa persistente
        m_movimentacaoData[id] = qMakePair(entrada, saida);

        // Reconecte o sinal
        connect(ui->movimentacaoTableWidget, &QTableWidget::cellChanged, this, &ControleEstoque::on_movimentacaoTableWidget_cellChanged);
    }
}

// Slot chamado quando o botão de confirmar movimentação é clicado.
// Aplica as movimentações ao estoque e salva no arquivo JSON.
void ControleEstoque::on_confirmarMovimentacaoButton_clicked()
{
    QJsonArray produtos = carregarProdutos();
    bool alterou = false;
    bool erro = false;

    // Primeiro, salve todos os dados preenchidos na tabela para o mapa antes de processar
    // Isso garante que os dados de produtos não visíveis mas preenchidos sejam considerados.
    for (int i = 0; i < ui->movimentacaoTableWidget->rowCount(); ++i) {
        QTableWidgetItem *idItem = ui->movimentacaoTableWidget->item(i, 0);
        QTableWidgetItem *entradaItem = ui->movimentacaoTableWidget->item(i, 6);
        QTableWidgetItem *saidaItem = ui->movimentacaoTableWidget->item(i, 7);

        if (idItem) {
            int id = idItem->text().toInt();
            int entrada = entradaItem ? entradaItem->text().toInt() : 0;
            int saida = saidaItem ? saidaItem->text().toInt() : 0;
            m_movimentacaoData[id] = qMakePair(entrada, saida);
        }
    }

    // Agora, itere sobre o mapa m_movimentacaoData para aplicar as mudanças aos produtos
    for (auto it = m_movimentacaoData.begin(); it != m_movimentacaoData.end(); ++it) {
        int id_produto = it.key();
        int entrada = it.value().first;
        int saida = it.value().second;

        if (entrada > 0 && saida > 0) {
            erro = true; // Sinaliza erro: entrada e saída simultâneas
            continue; // Pula este item para não aplicar a movimentação inválida
        }

        if (entrada <= 0 && saida <= 0)
            continue; // Pula se não houver movimentação para este produto

        for (int j = 0; j < produtos.size(); ++j) {
            QJsonObject obj = produtos[j].toObject();
            if (obj["id"].toInt() == id_produto) {
                int qtdAtual = obj["quantidade"].toInt();
                if (entrada > 0) {
                    obj["quantidade"] = qtdAtual + entrada;
                    produtos[j] = obj;
                    alterou = true;
                } else if (saida > 0) {
                    if (qtdAtual >= saida) { // Verifica se há estoque suficiente para a saída
                        obj["quantidade"] = qtdAtual - saida;
                        produtos[j] = obj;
                        alterou = true;
                    } else {
                        // Aviso de estoque insuficiente
                        QMessageBox::warning(this, "Erro de Estoque",
                                             QString("Saída do produto '%1' (ID: %2) é maior que a quantidade atual em estoque. Operação não realizada para este item.").arg(obj["nome"].toString()).arg(id_produto));
                        erro = true; // Sinaliza que houve um erro para este item
                    }
                }
                break; // Produto encontrado e processado, vai para o próximo do mapa
            }
        }
    }

    if (alterou && salvarProdutos(produtos)) {
        ui->movimentacaoMsgLabel->setStyleSheet("color: green;");
        ui->movimentacaoMsgLabel->setText("Movimentações realizadas com sucesso.");
        carregarTabela(); // Atualiza a tabela de consulta para refletir as novas quantidades
        atualizarQuantidadesNaTabela(); // Atualiza as quantidades na tabela de movimentação

        // Limpa os campos de entrada/saída na tabela de movimentação (apenas os visíveis)
        // Isso é feito para resetar a UI após a confirmação.
        for (int i = 0; i < ui->movimentacaoTableWidget->rowCount(); ++i) {
            if (ui->movimentacaoTableWidget->item(i, 6))
                ui->movimentacaoTableWidget->item(i, 6)->setText("");
            if (ui->movimentacaoTableWidget->item(i, 7))
                ui->movimentacaoTableWidget->item(i, 7)->setText("");
        }
        m_movimentacaoData.clear(); // Limpa TODOS os dados de movimentação armazenados para uma nova rodada
    } else if (!alterou) {
        ui->movimentacaoMsgLabel->setStyleSheet("color: red;");
        ui->movimentacaoMsgLabel->setText("Nenhuma movimentação realizada.");
    }

    if (erro) {
        // Mensagem de aviso mais geral, pois erros específicos já foram mostrados acima
        QMessageBox::warning(this, "Aviso", "Alguns problemas ocorreram durante as movimentações (ex: entrada e saída preenchidas no mesmo item, ou saída maior que o estoque). Por favor, verifique os detalhes nos avisos anteriores.");
    }
}

// Slot para o botão "Voltar" na aba de Movimentação
void ControleEstoque::on_btnVoltar_clicked()
{
    this->reject(); // ALTERADO: Usando reject() para encerrar o diálogo modal corretamente
}
