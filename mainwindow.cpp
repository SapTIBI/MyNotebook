#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     ui->textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     this->setCentralWidget(ui->textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_exit_triggered() // закрытие приложения
{
    close(); //событие closeEvent
}

void MainWindow::closeEvent(QCloseEvent *event) //обработка сигнала закрытия формы - нажатие кнопки крестик(выход)
{
    if (QFile::exists(path_file)){
        QFile file;
        file.setFileName(path_file);
        file.open(QIODevice::ReadOnly);
        QByteArray ba;//массив для передачи данных
        ba.clear();
        long long int size;//размер файла
        size=file.size();
        ba=file.read(size);//чтение из файла
        //проверка на то, что файл сохранить не нужно(то есть он не изменен)
        //спрашиваем подтверждение на "выход" у пользователя
        if ( QString::fromUtf8(ba) == ui->textEdit->toPlainText()) {
            if (QMessageBox::question(this,"Выходим?","Точно выходим?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                event->accept(); // закрываем приложение
            else
                event->ignore(); // отмена выхода
        }
        //если файл был изменен, то предлагаем пользователю его сохранить перед дальнейшим действием
        else {
            QMessageBox msgBox;
            msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед выходом?");
            msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setIcon(QMessageBox::Information);
            int res = msgBox.exec();
            switch (res) {
            //пользователь выбрал сохранить данные
            case QMessageBox::Yes:
                if (on_action_savefile_triggered()){
                    msgBox.close();
                    event->accept();
                }

                break;
            //пользователь выбрал сохранить данные
            case QMessageBox::No:
                msgBox.close();
                event->accept();
                break;
            //пользователь выбрал отменить операцию
            case QMessageBox::Cancel:
                msgBox.close();
                event->ignore();
                break;
            }

        }

    }
    // случай если файл не существует в системе(таково пути нет)
    else if (!ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед выходом?");
        msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Information);
        int res = msgBox.exec();
        switch (res)
        {
        case QMessageBox::Yes:
            if (on_action_savefile_triggered()) {
                event->accept();
                msgBox.close();
                return;
            }
            event->ignore();
            msgBox.close();
            return;
           // break;
        case QMessageBox::No:
            msgBox.close();
            event->accept();
            return;
           // break;
        case QMessageBox::Cancel:
            msgBox.close();
            event->ignore();
            return;
           // break;


        }
    }

}


void MainWindow::on_action_newfile_triggered() // создание нового файла
{
    // случай если файл существует в системе(такой путь есть), сохраняем его без помощи окна
    if (QFile::exists(path_file)){
        QFile file;
        file.setFileName(path_file);
        file.open(QIODevice::ReadOnly);
        QByteArray ba;//массив для передачи данных
        ba.clear();
        long long int size;//размер файла
        size=file.size();
        ba=file.read(size);//чтение из файла
        if (QString::fromUtf8(ba) == ui->textEdit->toPlainText()) {
            no_change = false;
            //создаем новый пустой файл
            MainWindow::setWindowTitle("Безымянный - Блокнот");
            path_file = "";
            ui->textEdit->clear();
            no_change = true;
            return;
            //
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед созданием нового документа?");
            msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setIcon(QMessageBox::Information);
            // получаем id нажатой кнопки
            int res = msgBox.exec();
            switch (res)
            {
            case QMessageBox::Yes:
                no_change = false;
                if (on_action_savefile_triggered()){
                    path_file = "";
                    MainWindow::setWindowTitle("Безымянный - Блокнот");
                    ui->textEdit->clear();
                }
                no_change = true;
                msgBox.close();
                break;
            case QMessageBox::No:
                no_change = false;
                path_file = "";
                msgBox.close();
                MainWindow::setWindowTitle("Безымянный - Блокнот");
                ui->textEdit->clear();
                no_change = true;

                break;
            case QMessageBox::Cancel:
                msgBox.close();
                break;


            }

        }

    }
    else if (!ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед созданием нового документа?");
        msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Information);
        int res = msgBox.exec();
        switch (res)
        {
        case QMessageBox::Yes:
            no_change = false;
            if (on_action_savefile_triggered()){
                MainWindow::setWindowTitle("Безымянный - Блокнот");
                ui->textEdit->clear();
            }
            no_change = true;
            msgBox.close();

            break;
        case QMessageBox::No:
            no_change = false;
            msgBox.close();
            MainWindow::setWindowTitle("Безымянный - Блокнот");
            path_file = "";
            ui->textEdit->clear();
            no_change = true;

            break;
        case QMessageBox::Cancel:
            msgBox.close();
            break;


        }
    }

}

bool  MainWindow::on_action_savefile_triggered() // сохранение данных в уже существующий файл
{
   // случай если файл существует в системе(такой путь есть), сохраняем его без помощи окна
   if (QFile::exists(path_file)) {
       QFile file;
       file.setFileName(path_file);
       file.open(QIODevice::WriteOnly);
       file.write(ui->textEdit->toPlainText().toUtf8());//запись в файл
       file.close();//закрываем
       QFileInfo fileInfo(file.fileName());
       QString file_short_name = fileInfo.fileName();
       MainWindow::setWindowTitle(file_short_name);
       return true;
   }
   // случай если файл не существует в системе(таково пути нет), сохраняем его при помощи окна(впервые)
   else {
      return  on_action_savefileas_triggered();
   }
}

bool MainWindow::on_action_savefileas_triggered() // сохранение файла впервые с выбраным именем
{
    //имя файла
    QString file_name;
    //получаем имя файла(путь к нему)
    file_name = QFileDialog::getSaveFileName(this,tr("Сохранить файл"),directory_path, tr("Text files (*.txt)"));
    if (!file_name.isEmpty()) {
        QFile file;
        file.setFileName(file_name);
        file.open(QIODevice::WriteOnly);
        file.write(ui->textEdit->toPlainText().toUtf8() );//запись в файл
        file.close();//закрываем
        path_file = file_name;
        QFileInfo fileInfo(file.fileName());
        QString file_short_name = fileInfo.fileName();
        MainWindow::setWindowTitle(file_short_name);
        return true;
       // no_change = true;
    }
    return false;


}



void MainWindow::on_action_openfile_triggered() // открытие файла
{
    no_change = false;
    QString file_dir;//имя файла
    //имя файла
    // случай если файл существует в системе
    if (QFile::exists(path_file)) {
        QFile file;
        file.setFileName(path_file);
        file.open(QIODevice::ReadOnly);
        QByteArray ba;//массив для передачи данных
        ba.clear();
        long long int size;//размер файла
        size=file.size();
        ba=file.read(size);//чтение из файла
        file.close();
        //проверка на то, что файл сохранить не нужно(то есть он не изменен)
        qDebug() << QString::fromUtf8(ba);
        if (QString::fromUtf8(ba) == ui->textEdit->toPlainText()) {
            QString file_dir;//имя файла
            file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.txt)"));
            //проверяем выбран ли фаил
            if (!file_dir.isEmpty())
            {//фаил выбран
                no_change = false;
                ui->textEdit->clear();
                QFile file;
                file.setFileName(file_dir);
                file.open(QIODevice::ReadOnly);
                QByteArray ba;//массив для передачи данных
                ba.clear();
                long long int size;//размер файла
                size=file.size();
                ba=file.read(size);//чтение из файла
                ui->textEdit->append(QString::fromUtf8(ba));//копирование в поле редактора
                file.close();//закрываем
                path_file = file_dir;
                QFileInfo fileInfo(file.fileName());
                QString file_short_name = fileInfo.fileName();
                MainWindow::setWindowTitle(file_short_name);
                no_change = true;
            }
            return;
        }
        //если файл был изменен, то предлагаем пользователю его сохранить перед дальнейшим дейсвием
        //показываем окно выбора
        else{
            QMessageBox msgBox;
            msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед открытием нового документа?");
            msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setIcon(QMessageBox::Information);
            int res = msgBox.exec();
            switch (res)
            {
            //пользователь выбрал сохранить данные
            case QMessageBox::Yes:
                on_action_savefile_triggered();//сохраняем текущие данные
                on_action_openfile_triggered();//открываем файл
                msgBox.close(); // скрываем окно с предупреждением(выбором)
                break;
            //пользователь выбрал не сохранять данные
            case QMessageBox::No:
                file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.txt)"));
                //проверяем выбран ли фаил
                if (!file_dir.isEmpty())
                {//фаил выбран
                    no_change = false;
                    QFile file;
                    file.setFileName(file_dir);
                    file.open(QIODevice::ReadOnly);
                    QByteArray ba;//массив для передачи данных
                    ba.clear();
                    long long int size;//размер файла
                    size=file.size();
                    ba=file.read(size);//чтение из файла
                    ui->textEdit->clear();
                    ui->textEdit->append(QString::fromUtf8(ba));//копирование в поле редактора
                    file.close();//закрываем
                    path_file = file_dir;
                    QFileInfo fileInfo(file.fileName());
                    QString file_short_name = fileInfo.fileName();
                    MainWindow::setWindowTitle(file_short_name);
                    no_change = true;
                }
                msgBox.close();// скрываем окно с предупреждением(выбором)
                break;
            //пользователь выбрал отменить  операцию
            case QMessageBox::Cancel:
                msgBox.close();// скрываем окно с предупреждением(выбором)
                break;
            }
        }
    }
    // случай, когда файла еще нет в системе, пользователю предлагают сохранить изменнные данные(окно выбора)
    else if (!ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед открытием нового документа?");
        msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Information);
        int res = msgBox.exec(); // получаем id нажатой кнопки
        switch (res)
        {
        //пользователь выбрал сохранить данные
        case QMessageBox::Yes:
            if (on_action_savefile_triggered()){
                //получаем имя файла( путь к файлу)
                file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.txt)"));
                //проверяем выбран ли фаил
                if (!file_dir.isEmpty())
                {//фаил выбран
                    no_change = false;
                    ui->textEdit->clear();
                    QFile file;
                    file.setFileName(file_dir);
                    file.open(QIODevice::ReadOnly);
                    QByteArray ba;//массив для передачи данных
                    ba.clear();
                    long long int size;//размер файла
                    size=file.size();
                    ba=file.read(size);//чтение из файла
                    ui->textEdit->append(QString::fromUtf8(ba));//копирование в поле редактора
                    file.close();//закрываем
                    path_file = file_dir;
                    QFileInfo fileInfo(file.fileName());
                    QString file_short_name = fileInfo.fileName();
                    MainWindow::setWindowTitle(file_short_name);
                    no_change = true;
                }
            }
            msgBox.close();
            break;
        //пользователь выбрал не сохранять данные

        case QMessageBox::No:

                file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.txt)"));
                //проверяем выбран ли фаил
                if (!file_dir.isEmpty())
                {//фаил выбран
                    no_change = false;
                    QFile file;
                    file.setFileName(file_dir);
                    file.open(QIODevice::ReadOnly);
                    QByteArray ba;//массив для передачи данных
                    ba.clear();
                    long long int size;//размер файла
                    size=file.size();
                    ba=file.read(size);//чтение из файла
                    ui->textEdit->clear();
                    ui->textEdit->append(QString::fromUtf8(ba));//копирование в поле редактора
                    file.close();//закрываем
                    path_file = file_dir;
                    QFileInfo fileInfo(file.fileName());
                    QString file_short_name = fileInfo.fileName();
                    MainWindow::setWindowTitle(file_short_name);
                    no_change = true;
                }
                msgBox.close();// скрываем окно с предупреждением(выбором)
                break;

        //пользователь выбрал отменить операцию
        case QMessageBox::Cancel:
            msgBox.close();// скрываем окно с предупреждением(выбором)
            break;


        }
    }
    else {
        //имя файла
        QString file_dir;
        //получаем имя файла( путь к файлу)
        file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.txt)"));
        //проверяем выбран ли фаил
        if (!file_dir.isEmpty())
        {//фаил выбран
            no_change = false;
            ui->textEdit->clear();
            QFile file;
            file.setFileName(file_dir);
            file.open(QIODevice::ReadOnly);
            QByteArray ba;//массив для передачи данных
            ba.clear();
            long long int size;//размер файла
            size=file.size();
            ba=file.read(size);//чтение из файла
            ui->textEdit->append(QString::fromUtf8(ba));//копирование в поле редактора
            file.close();//закрываем
            path_file = file_dir;
            QFileInfo fileInfo(file.fileName());
            QString file_short_name = fileInfo.fileName();
            MainWindow::setWindowTitle(file_short_name);
            no_change = true;
        }
    }

}




void MainWindow::on_action_zoomin_triggered() //увеличение масштаба текста
{
    // получаем текущий шрифт у textEdit
    QFont font = ui->textEdit->font();
    // получаем текущий размер шрифта у textEdit
    int size = font.pointSize();
    // устанавливаем новый размер шрифта у textEdit
    font.setPointSize(size + 15);
    // присваиваем textEdit новый измененный шрифт
    ui->textEdit->setFont(font);
}


void MainWindow::on_action_zoomout_triggered()//уменьшение масштаба текста
{
    // получаем текущий шрифт у textEdit
    QFont font = ui->textEdit->font();
    // получаем текущий размер шрифта у textEdit
    int size = font.pointSize();
    // устанавливаем новый размер шрифта у textEdit
    font.setPointSize(size - 15);
    // присваиваем textEdit новый измененный шрифт
    ui->textEdit->setFont(font);
}


void MainWindow::on_action_reset_triggered() // сброс шрифта
{
    // получаем текущий шрифт у textEdit
    QFont font = ui->textEdit->font();
    // устанавливаем новый размер шрифта (размер по умолчанию) у textEdit
    font.setPointSize(9);
    // присваиваем textEdit новый измененный шрифт
    ui->textEdit->setFont(font);
}


void MainWindow::on_action_newwindow_triggered()// создаем еще одно окно
{
    MainWindow *w2 = new MainWindow(); //создаем окно
    w2->show(); // показываем окно
}




//при изменении  textEdit
void MainWindow::on_textEdit_textChanged()
{
    if (no_change){
        if (path_file == "") {
            if (ui->textEdit->toPlainText().isEmpty()) {
                this->setWindowTitle("Безымянный - Блокнот");
                path_file = "";
            }
            else {

                QString modtitle = this->windowTitle();
                if (!modtitle.contains("*")){
                    this->setWindowTitle("*" + modtitle);
                }
            }
        }
        else {
            QFile file;
            file.setFileName(path_file);
            file.open(QIODevice::ReadOnly);
            QByteArray ba;//массив для передачи данных
            ba.clear();
            long long int size;//размер файла
            size=file.size();
            ba=file.read(size);//чтение из файла
            file.close();
            //проверка на то, что файл сохранить не нужно(то есть он не изменен)
            if (QString::fromUtf8(ba) != ui->textEdit->toPlainText()) {

                QString modtitle = this->windowTitle();
                if (!modtitle.contains("*")){
                    this->setWindowTitle("*" + modtitle);
                }
            }
            else {
                QFileInfo fileInfo(file.fileName());
                QString file_short_name = fileInfo.fileName();
                MainWindow::setWindowTitle(file_short_name);

            }

        }


    }


}

