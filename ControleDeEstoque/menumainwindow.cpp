#include "menumainwindow.h"
#include "ui_menumainwindow.h"
#include "listagemprodutos.h"
#include "controleestoque.h"
#include "mainwindow.h" // Inclui o cabeçalho da MainWindow

// Construtor da classe MenuMainWindow
MenuMainWindow::MenuMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuMainWindow)  // Inicializa a interface gráfica para o menu principal
{
    ui->setupUi(this);  // Configura a interface gráfica
    setWindowTitle("Menu Principal");  // Define o título da janela

    // Conecta o botão "Cadastros" ao slot on_btnCadastros_clicked
    connect(ui->btnCadastros, &QPushButton::clicked, this, &MenuMainWindow::on_btnCadastros_clicked);
    // Conecta o botão "Estoque" ao slot on_btnEstoque_clicked
    connect(ui->btnEstoque, &QPushButton::clicked, this, &MenuMainWindow::on_btnEstoque_clicked);
    // Conecta o botão "Logout" ao slot on_btnLogout_clicked
    connect(ui->btnLogout, &QPushButton::clicked, this, &MenuMainWindow::on_btnLogout_clicked);
}

// Destruidor da classe MenuMainWindow, libera a memória da interface
MenuMainWindow::~MenuMainWindow()
{
    delete ui;
}

// Função chamada quando o botão "Cadastros" é clicado
void MenuMainWindow::on_btnCadastros_clicked()
{
    this->hide();  // Esconde a janela do Menu Principal

    // Cria a janela de Listagem de Produtos (onde o usuário pode ver todos os produtos)
    ListagemProdutos *telaListagem = new ListagemProdutos(this);

    telaListagem->setModal(true);  // Define o diálogo como modal, ou seja, o usuário deve fechá-lo antes de interagir com outras janelas
    telaListagem->exec();  // Executa o diálogo de listagem de produtos, bloqueando até que ele seja fechado

    delete telaListagem;  // Libera a memória do diálogo após ser fechado

    this->show();  // Mostra a janela do Menu Principal novamente
}

// Função chamada quando o botão "Estoque" é clicado
void MenuMainWindow::on_btnEstoque_clicked()
{
    this->hide();  // Esconde a janela do Menu Principal

    // Cria a janela de Controle de Estoque, onde o usuário pode gerenciar o estoque de produtos
    ControleEstoque *estoque = new ControleEstoque(this);

    estoque->setModal(true);  // Define o diálogo como modal, ou seja, o usuário deve fechá-lo antes de interagir com outras janelas
    estoque->exec();  // Executa o diálogo de controle de estoque, bloqueando até que ele seja fechado

    delete estoque;  // Libera a memória do diálogo após ser fechado

    this->show();  // Mostra a janela do Menu Principal novamente
}

// Função chamada quando o botão "Logout" é clicado
void MenuMainWindow::on_btnLogout_clicked()
{
    this->close();  // Fecha a janela atual (MenuMainWindow) e efetua o logout do usuário
}
