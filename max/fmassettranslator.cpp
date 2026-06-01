//**************************************************************************/
// Copyright (c) 1998-2024 Autodesk, Inc.
// All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.
//**************************************************************************/
// DESCRIPTION: Plugin Wizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "fmassettranslator.h"
#include "DCCManager.hpp"
#include "maxutils.h"

#include <QApplication>
#include <QFile>
#include <QPointer>
#include <QSizePolicy>

#include "fmmainwindow.h"

#define FMAssetTranslator_CLASS_ID Class_ID(0x87d995e6, 0xa0b2225f)

class FMAssetTranslator : public SceneImport
{
public:
	// Constructor/Destructor
	FMAssetTranslator();
	virtual ~FMAssetTranslator();

	int ExtCount() override; // Number of extensions supported
	const TCHAR* Ext(int n) override; // Extension #n (i.e. "3DS")
	const TCHAR* LongDesc() override; // Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR* ShortDesc() override; // Short ASCII description (i.e. "3D Studio")
	const TCHAR* AuthorName() override; // ASCII Author name
	const TCHAR* CopyrightMessage() override; // ASCII Copyright message
	const TCHAR* OtherMessage1() override; // Other message #1
	const TCHAR* OtherMessage2() override; // Other message #2
	unsigned int Version() override; // Version number * 100 (i.e. v3.01 = 301)
	void ShowAbout(HWND hWnd) override; // Show DLL's "About..." box
	int DoImport(const TCHAR* name, ImpInterface* i, Interface* gi, BOOL suppressPrompts = FALSE) override; // Import file
	int ZoomExtents() override;
};


class FMAssetTranslatorClassDesc : public ClassDesc2
{
public:
	int           IsPublic() override { return TRUE; }
	void* Create(BOOL /*loading = FALSE*/) override { return new FMAssetTranslator(); }
	const TCHAR* ClassName() override { return GetString(IDS_CLASS_NAME); }
	const TCHAR* NonLocalizedClassName() override { return _T("FMAssetTranslator"); }
	SClass_ID     SuperClassID() override { return SCENE_IMPORT_CLASS_ID; }
	Class_ID      ClassID() override { return FMAssetTranslator_CLASS_ID; }
	const TCHAR* Category() override { return GetString(IDS_CATEGORY); }

	const TCHAR* InternalName() override { return _T("FMAssetTranslator"); } // Returns fixed parsable name (scripter-visible name)
	HINSTANCE     HInstance() override { return hInstance; } // Returns owning module handle

};

ClassDesc2* GetFMAssetTranslatorDesc()
{
	static FMAssetTranslatorClassDesc FMAssetTranslatorDesc;
	return &FMAssetTranslatorDesc;
}

INT_PTR CALLBACK FMAssetTranslatorOptionsDlgProc(HWND hWnd, UINT message, WPARAM, LPARAM lParam)
{
	static FMAssetTranslator* imp = nullptr;

	switch (message)
	{
	case WM_INITDIALOG:
		imp = (FMAssetTranslator*)lParam;
		CenterWindow(hWnd, GetParent(hWnd));
		return TRUE;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return 1;
	}

	return 0;
}

//--- FMAssetTranslator -------------------------------------------------------
FMAssetTranslator::FMAssetTranslator()
{

}

FMAssetTranslator::~FMAssetTranslator()
{

}

int FMAssetTranslator::ExtCount()
{
//#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR* FMAssetTranslator::Ext(int /*n*/)
{
//#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("zip");
}

const TCHAR* FMAssetTranslator::LongDesc()
{
//#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("ForzaTech Asset Importer");
}

const TCHAR* FMAssetTranslator::ShortDesc()
{
//#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("ForzaTech Asset Importer");
}

const TCHAR* FMAssetTranslator::AuthorName()
{
//#pragma message(TODO("Return ASCII Author name"))
	return _T("Autodesk");
}

const TCHAR* FMAssetTranslator::CopyrightMessage()
{
//#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR* FMAssetTranslator::OtherMessage1()
{
	// TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR* FMAssetTranslator::OtherMessage2()
{
	// TODO: Return other message #2 in any
	return _T("");
}

unsigned int FMAssetTranslator::Version()
{
//#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return FT_MAX_VERSION_NUMBER;
}

void FMAssetTranslator::ShowAbout(HWND /*hWnd*/)
{
	// Optional
}


int FMAssetTranslator::DoImport(const TCHAR* filename, ImpInterface* /*importerInt*/, Interface* /*ip*/, BOOL suppressPrompts)
{
	std::string file_name = fmnext::convertWideToStdString(filename);

	QPointer<FMMainWindow> qPtrMainWindow;
	if (qPtrMainWindow.isNull())
	{
		qPtrMainWindow = new FMMainWindow(FMQtWindow::mainWindow());

#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 2)
		qPtrMainWindow->setWindowTitle("Forza Enhanced Qt5");
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		qPtrMainWindow->setWindowTitle("Forza Enhanced Qt6");
#endif

		//DCCManager::SetImporterFrameRate();

		QFile f(":/assets/custom.qss");
		if (f.open(QFile::ReadOnly))
		{
			qPtrMainWindow->setStyleSheet(f.readAll());
		}

		qPtrMainWindow->setModel(file_name);
		qPtrMainWindow->show();
	}

	return TRUE;
}

int FMAssetTranslator::ZoomExtents()
{
	return ZOOMEXT_NO;
}
