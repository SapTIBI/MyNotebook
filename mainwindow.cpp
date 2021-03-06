#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractButton>
#include <QPushButton>
#include <QFontDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit); //растягиваем textEdit по всей  ширине главного окна
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
    // случай если файл существует в системе(такой путь есть)
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
        if ( QString::fromUtf8(ba) == ui->textEdit->toPlainText()) {
            event->accept(); // закрываем приложение
        }
        //если файл был изменен, то предлагаем пользователю его сохранить перед дальнейшим действием
        else {
            QMessageBox msgBox;
            msgBox.setText(QString("Вы хотите сохранить изменения в файле '%1' ?").arg(path_file));
            QPushButton *yesbtn = msgBox.addButton(tr("Сохранить"),QMessageBox::YesRole);
            QPushButton *nobtn =  msgBox.addButton(tr("Не сохранять"),QMessageBox::NoRole);
            QPushButton *cancelbtn = msgBox.addButton(tr("Отмена"),QMessageBox::RejectRole);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            //пользователь выбрал сохранить данные
            if (msgBox.clickedButton() == yesbtn) {
                //проверка на то, что операция сохранения прошла и вернула true
                if (on_action_savefile_triggered()){
                    msgBox.close();
                    event->accept();
                }
                delete yesbtn;
                return;
            }
            //пользователь выбрал не сохранять данные
            else if (msgBox.clickedButton() == nobtn) {
                msgBox.close();
                event->accept();
                delete  nobtn;
                return;
            }
            //пользователь отменяет операцию
            else if (msgBox.clickedButton() == cancelbtn) {
                msgBox.close();
                event->ignore();
                delete cancelbtn;
                return;
            }

        }

    }
    // случай если файл не существует в системе(таково пути нет)
    else if (!ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText(QString("Вы хотите сохранить изменения в файле 'Безымянный' ?"));
        QPushButton *yesbtn = msgBox.addButton(tr("Сохранить"),QMessageBox::YesRole);
        QPushButton *nobtn =  msgBox.addButton(tr("Не сохранять"),QMessageBox::NoRole);
        QPushButton *cancelbtn = msgBox.addButton(tr("Отмена"),QMessageBox::RejectRole);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        //пользователь выбрал сохранить данные
        if (msgBox.clickedButton() == yesbtn) {
            delete yesbtn;
            //проверка на то, что операция сохранения прошла и вернула true
            if (on_action_savefile_triggered()) {
                event->accept();
                msgBox.close();
                return;
            }
            event->ignore();
            msgBox.close();
            return;
        }
        //пользователь выбрал не сохранять данные
        else if (msgBox.clickedButton() == nobtn) {
            msgBox.close();
            event->accept();
            delete nobtn;
            return;
        }
        //пользователь отменяет операцию
        else if (msgBox.clickedButton() == cancelbtn) {
            msgBox.close();
            event->ignore();
            delete cancelbtn;
            return;
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
            no_change = false;// textEdit перестает обрабатывать изменения - изменения происходят программно
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
            msgBox.setText(QString("Вы хотите сохранить изменения в файле '%1' ?").arg(path_file));
            QPushButton *yesbtn = msgBox.addButton(tr("Сохранить"),QMessageBox::YesRole);
            QPushButton *nobtn =  msgBox.addButton(tr("Не сохранять"),QMessageBox::NoRole);
            QPushButton *cancelbtn = msgBox.addButton(tr("Отмена"),QMessageBox::RejectRole);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            //пользователь выбрал сохранить данные
            if (msgBox.clickedButton() == yesbtn) {
                no_change = false;// textEdit перестает обрабатывать изменения - изменения происходят программно
                //проверка на то, что операция сохранения прошла и вернула true
                if (on_action_savefile_triggered()){
                    path_file = "";
                    MainWindow::setWindowTitle("Безымянный - Блокнот");
                    ui->textEdit->clear();
                }
                no_change = true;
                msgBox.close();
                delete yesbtn;
                return;
            }
            //пользователь выбрал не сохранять данные
            else if (msgBox.clickedButton() == nobtn) {
                no_change = false;// textEdit перестает обрабатывать изменения - изменения происходят программно
                path_file = "";
                msgBox.close();
                MainWindow::setWindowTitle("Безымянный - Блокнот");
                ui->textEdit->clear();
                no_change = true;
                delete nobtn;
                return;
            }
            //пользователь отменяет операцию
            else if (msgBox.clickedButton() == cancelbtn) {
                msgBox.close();
                delete cancelbtn;
                return;
            }

        }

    }
    else if (!ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText(QString("Вы хотите сохранить изменения в файле 'Безымянный' ?"));
        QPushButton *yesbtn = msgBox.addButton(tr("Сохранить"),QMessageBox::YesRole);
        QPushButton *nobtn =  msgBox.addButton(tr("Не сохранять"),QMessageBox::NoRole);
        QPushButton *cancelbtn = msgBox.addButton(tr("Отмена"),QMessageBox::RejectRole);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        //пользователь выбрал сохранить данные
        if (msgBox.clickedButton() == yesbtn) {
            no_change = false; // textEdit перестает обрабатывать изменения - изменения происходят программно
            //проверка на то, что операция сохранения прошла и вернула true
            if (on_action_savefile_triggered()){
                MainWindow::setWindowTitle("Безымянный - Блокнот");
                path_file = "";
                ui->textEdit->clear();
            }
            no_change = true;// textEdit начинает обрабатывать изменения - изменения происходят вручную(пользователем)
            msgBox.close();
            delete yesbtn;
            return;
        }
        //пользователь выбрал не сохранять данные
        else if (msgBox.clickedButton() == nobtn) {
            no_change = false;// textEdit перестает обрабатывать изменения - изменения происходят программно
            msgBox.close();
            MainWindow::setWindowTitle("Безымянный - Блокнот");
            path_file = "";
            ui->textEdit->clear();
            no_change = true;// textEdit начинает обрабатывать изменения - изменения происходят вручную(пользователем)
            delete nobtn;
            return;
        }
        //пользователь отменяет операцию
        else if (msgBox.clickedButton() == cancelbtn) {
            msgBox.close();
            delete cancelbtn;
            return;
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
        return true; // сохранение прошло успешно
    }
    // случай если файл не существует в системе(таково пути нет), сохраняем его при помощи окна(впервые)
    else {
        return  on_action_savefileas_triggered();
    }
}

bool MainWindow::on_action_savefileas_triggered() // сохранение файла впервые с выбором имени (окно сохранения)
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
        return true; // сохранение прошло успешно
    }
    return false; // файла не найдено, ошибка сохранения


}



void MainWindow::on_action_openfile_triggered() // открытие файла
{
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
        if (QString::fromUtf8(ba) == ui->textEdit->toPlainText()) {
            QString file_dir;//имя файла
            file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.txt)"));
            //проверяем выбран ли фаил
            if (!file_dir.isEmpty())
            {//фаил выбран
                no_change = false;// textEdit перестает обрабатывать изменения - изменения происходят программно
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
                no_change = true;// textEdit начинает обрабатывать изменения - изменения происходят вручную(пользователем)
            }
            return;
        }
        //если файл был изменен, то предлагаем пользователю его сохранить перед дальнейшим дейсвием
        //показываем окно выбора
        else{
            QMessageBox msgBox;
            msgBox.setText(QString("Вы хотите сохранить изменения в файле '%1' ?").arg(path_file));
            QPushButton *yesbtn = msgBox.addButton(tr("Сохранить"),QMessageBox::YesRole);
            QPushButton *nobtn =  msgBox.addButton(tr("Не сохранять"),QMessageBox::NoRole);
            QPushButton *cancelbtn = msgBox.addButton(tr("Отмена"),QMessageBox::RejectRole);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            //пользователь выбрал сохранить данные
            if (msgBox.clickedButton() == yesbtn) {
                on_action_savefile_triggered();//сохраняем текущие данные
                on_action_openfile_triggered();//открываем файл
                msgBox.close(); // скрываем окно с предупреждением(выбором)
                delete yesbtn;
                return;
            }
            //пользователь выбрал не сохранять данные
            else if (msgBox.clickedButton() == nobtn) {
                file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.txt)"));
                //проверяем выбран ли фаил
                if (!file_dir.isEmpty())
                {//фаил выбран
                    no_change = false;// textEdit перестает обрабатывать изменения - изменения происходят программно
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
                    no_change = true;// textEdit начинает обрабатывать изменения - изменения происходят вручную(пользователем)
                }
                msgBox.close();// скрываем окно с предупреждением(выбором)
                delete nobtn;
                return;
            }
            //пользователь отменяет операцию
            else if (msgBox.clickedButton() == cancelbtn) {
                msgBox.close();// скрываем окно с предупреждением(выбором)
                delete cancelbtn;
                return;
            }
        }
    }
    // случай, когда файла еще нет в системе, пользователю предлагают сохранить изменнные данные(окно выбора)
    else if (!ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText(QString("Вы хотите сохранить изменения в файле 'Безымянный' ?"));
        QPushButton *yesbtn = msgBox.addButton(tr("Сохранить"),QMessageBox::YesRole);
        QPushButton *nobtn =  msgBox.addButton(tr("Не сохранять"),QMessageBox::NoRole);
        QPushButton *cancelbtn = msgBox.addButton(tr("Отмена"),QMessageBox::RejectRole);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        //пользователь выбрал сохранить данные
        if (msgBox.clickedButton() == yesbtn) {
            //проверка на то, что операция сохранения прошла и вернула true
            if (on_action_savefile_triggered()){
                //получаем имя файла( путь к файлу)
                file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.txt)"));
                //проверяем выбран ли фаил
                if (!file_dir.isEmpty())
                {//фаил выбран
                    no_change = false;// textEdit перестает обрабатывать изменения - изменения происходят программно
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
                    no_change = true;// textEdit начинает обрабатывать изменения - изменения происходят вручную(пользователем)
                }
            }
            msgBox.close();
            delete yesbtn;
            return;
        }
        //пользователь выбрал не сохранять данные
        else if (msgBox.clickedButton() == nobtn) {
            file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.txt)"));
            //проверяем выбран ли фаил
            if (!file_dir.isEmpty())
            {//фаил выбран
                no_change = false;// textEdit перестает обрабатывать изменения - изменения происходят программно
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
                no_change = true;// textEdit начинает обрабатывать изменения - изменения происходят вручную(пользователем)
            }
            msgBox.close();// скрываем окно с предупреждением(выбором)
            delete nobtn;
            return;
        }
        //пользователь отменяет операцию
        else if (msgBox.clickedButton() == cancelbtn) {
            msgBox.close();// скрываем окно с предупреждением(выбором)
            delete cancelbtn;
            return;
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
            no_change = false;// textEdit перестает обрабатывать изменения - изменения происходят программно
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
            no_change = true;// textEdit начинает обрабатывать изменения - изменения происходят вручную(пользователем)
        }
    }

}

void MainWindow::on_action_fontedit_triggered()//настройка шрифта
{
    bool check;
    QFont font = QFontDialog::getFont(&check,this); //получаем параметры шрифта
    if (check) {
        QTextCursor cursor(ui->textEdit->textCursor());
        QTextCharFormat Format = cursor.charFormat();
        Format.setFont(font);
        cursor.setCharFormat(Format);
        ui->textEdit->setTextCursor(cursor);
    }
    return;



}

void MainWindow::on_action_reset_triggered() // сброс шрифта
{
    QFont font;
    if (ui->textEdit->textCursor().selectedText().isEmpty()) {
        ui->textEdit->selectAll();
    }
    //присваиваем textEdit новый defaultный шрифт
    ui->textEdit->setCurrentFont(font);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.clearSelection();
    ui->textEdit->setTextCursor(cursor);
}


void MainWindow::on_action_newwindow_triggered()// создаем еще одно окно
{
    MainWindow *w2 = new MainWindow(); //создаем окно
    w2->show(); // показываем окно
}




//при изменении  textEdit
void MainWindow::on_textEdit_textChanged()
{   //no_change = true - изменения в textEdit были сделаны вручную
    if (no_change){
        //если мы работаем с файлом "Безымянный"
        if (path_file.isEmpty()) {
            //проверка на отсутствие изменений в файле "Безымянный"
            if (ui->textEdit->toPlainText().isEmpty())
                this->setWindowTitle("Безымянный - Блокнот");
            else
                //файл "Безымянный" был изменен - к тайтлу файла приписываем "*"
                this->setWindowTitle("*Безымянный - Блокнот");
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
            //проверка на то, что файл изменен - нужно добавить "*" в тайтле пути
            if (QString::fromUtf8(ba) != ui->textEdit->toPlainText()) {
                QString modtitle = this->windowTitle();
                if (!modtitle.contains("*")) this->setWindowTitle("*" + modtitle);
            }
            // в ином случае тайтл файла без "*"
            else {
                QFileInfo fileInfo(file.fileName());
                QString file_short_name = fileInfo.fileName();
                MainWindow::setWindowTitle(file_short_name);

            }

        }


    }


}


