#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cadastrousuario.h"
#include "menumainwindow.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

// Construtor da classe MainWindow
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // Configura a interface gráfica

    // Conecta o botão de login ao slot onLoginClicked
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    // Conecta o botão de cadastro ao slot onCadastrarClicked
    connect(ui->cadastrarButton, &QPushButton::clicked, this, &MainWindow::onCadastrarClicked);
}

// Destruidor da classe MainWindow, libera a memória alocada para a interface
MainWindow::~MainWindow()
{
    delete ui;
}

// Função chamada quando o botão de login é clicado
void MainWindow::onLoginClicked()
{
    // Obtém o nome de usuário e a senha digitados
    QString usuario = ui->usernameLineEdit->text().trimmed();
    QString senha = ui->passwordLineEdit->text().trimmed();

    QFile file("usuarios.json");  // Abre o arquivo JSON contendo os dados dos usuários
    if (!file.open(QIODevice::ReadOnly)) {
        // Se o arquivo não puder ser aberto, exibe uma mensagem de erro
        ui->errorLabel->setText("Erro ao abrir o arquivo de usuários.");
        return;
    }

    QByteArray data = file.readAll();  // Lê todos os dados do arquivo
    file.close();  // Fecha o arquivo após a leitura

    QJsonDocument doc = QJsonDocument::fromJson(data);  // Converte os dados lidos em um documento JSON
    QJsonArray usuarios = doc.array();  // Converte o documento JSON em um array de usuários

    // Percorre todos os usuários no array
    for (const QJsonValue &val : usuarios) {
        QJsonObject obj = val.toObject();
        // Verifica se o nome de usuário e a senha coincidem com os dados cadastrados
        if (obj["usuario"].toString() == usuario && obj["senha"].toString() == senha) {
            // Login bem-sucedido → abre a janela MenuMainWindow
            MenuMainWindow *menu = new MenuMainWindow();
            menu->show();  // Exibe a janela do menu
            this->close();  // Fecha a janela de login
            return;  // Retorna após o login bem-sucedido
        }
    }

    // Caso o usuário ou a senha estejam incorretos, exibe uma mensagem de erro
    ui->errorLabel->setStyleSheet("color: red; font-weight: bold;");
    ui->errorLabel->setText("Usuário ou senha inválidos!");
}

// Função chamada quando o botão de cadastro é clicado
void MainWindow::onCadastrarClicked()
{
    // Cria uma nova instância da tela de cadastro de usuário
    CadastroUsuario cadastro;
    cadastro.exec();  // Exibe a tela de cadastro como um diálogo modal
}
