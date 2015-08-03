#include "edlogger.h"

#include "GLFW/glfw3.h"
#include "FreeImage/FreeImage.h"

EDLogger::EDLogger(void)
{
	ltime = time(NULL);
	Tm = localtime(&ltime);
}

EDLogger::EDLogger(char* filename)
{
	printf("Log: %s\n", filename);
	ltime = time(NULL);
	Tm = localtime(&ltime);
	writer = new EDFileWriter(filename);
}

EDLogger::~EDLogger()
{
	delete writer;
}

void EDLogger::logLine(char* line)
{
	writer->writeLnStr(line);
}

void EDLogger::logLineTimestamp(char* line)
{
	char tsLine[256];
	sprintf(tsLine, "{%2.d:%2.d} - %2.d:%2.d:%2.d: %s", Tm->tm_mday, Tm->tm_mon + 1, Tm->tm_hour, Tm->tm_min, Tm->tm_sec, line);
	writer->writeLnStr(tsLine);
}

void EDLogger::closeLog(void)
{
	writer->close();
}

//EDPrinter

EDPrinter::EDPrinter(void)
{ }

EDPrinter::~EDPrinter(void)
{ }

void EDPrinter::printScreen(GLConfig* config, char* printName)
{
	int size = config->height *config->width * 3;
	if (size == 0)
	{
		return;
	}
	GLubyte *pixels = new GLubyte[size];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, config->width, config->height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char temp;

	int i = 0;
	while (i < size)
	{
		temp = pixels[i];       //grab blue
		pixels[i] = pixels[i + 2];//assign red to blue
		pixels[i + 2] = temp;     //assign blue to red
		i += 3;     //skip to next blue byte
	}

	FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, config->width, config->height, 3 * config->width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
	FreeImage_Save(FIF_BMP, image, printName, 0);
	FreeImage_Unload(image);

	delete[] pixels;
}

//void EDPrinter::printScreen(EDConfiguration config)
//{
//	printScreen(config, config.getPrintFilename());
//}
//
//void EDPrinter::printScreen(EDConfiguration config, char* printName)
//{
//	int size = config.height *config.width * 3;
//	if(size == 0)
//	{
//		return;
//	}
//	GLubyte *pixels = new GLubyte [size];
//	glPixelStorei(GL_PACK_ALIGNMENT, 1);
//	glReadPixels(0, 0, config.width, config.height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//	unsigned char temp;
//
//    int i = 0;
//    while (i < size)
//    {
//        temp = pixels[i];       //grab blue
//        pixels[i] = pixels[i+2];//assign red to blue
//        pixels[i+2] = temp;     //assign blue to red
//        i += 3;     //skip to next blue byte
//    }
//
//	FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, config.width, config.height, 3 * config.width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
//	FreeImage_Save(FIF_BMP, image, printName, 0);
//	FreeImage_Unload(image);
//
//	delete [] pixels;
//}