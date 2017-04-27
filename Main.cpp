#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <string.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	std::string fileName = QFileDialog::getOpenFileName(NULL, "Open File", NULL, "Gameboy ROMs (*.gb)").toStdString();

	return 0;
}