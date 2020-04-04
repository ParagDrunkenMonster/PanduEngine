#include "PANDUColadaToPanduExporter.h"
#include <conio.h>

int main(int argc, char *argv[])
{
	Pandu::LogManager::CreateInstance();

	Pandu::String mediaDir = "../../../Media/";
	Pandu::String exportPath = mediaDir + "bin";
	//Pandu::GeometryBinary geometryBinary("pSphereShape1.pgb");
	//geometryBinary.LoadFromFile();


	if(argc >= 2)
	{
		Pandu::ColadaToPanduExporter exporter(exportPath,argv[1]);
		exporter.ExportToPandu((unsigned int)Pandu::eCP_None);
	}
	else
	{
		Pandu::ColadaToPanduExporter exporter( exportPath,(mediaDir + "Goblin.dae").CString() );
		exporter.ExportToPandu((unsigned int)Pandu::eCP_None);
		//Pandu::gLogManager->WriteLog("Display help");
	}

	Pandu::LogManager::DestroyInstance();

	getch();

	return 0;
}