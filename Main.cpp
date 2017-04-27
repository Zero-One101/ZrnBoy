#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <string.h>

#include "Z80.h"

Z80 z80;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QString fileName = QFileDialog::getOpenFileName(NULL, "Open File", NULL, "Gameboy ROMs (*.gb)");

	if (fileName.isEmpty())
	{
		QMessageBox::critical(NULL, "Error", "Must select a valid .gb file");
		return 1;
	}

	z80.LoadGame(fileName.toStdString());

	return 0;
}