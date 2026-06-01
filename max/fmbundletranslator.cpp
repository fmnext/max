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

#include <QPointer>

#include "fmbundletranslator.h"
#include "fmoptionsdialog.h"
#include "maxutils.h"

#define FMBundleTranslator_CLASS_ID Class_ID(0x87d995e6, 0xa0b2114f)

class FMBundleTranslator : public SceneImport
{
public:
	// Constructor/Destructor
	FMBundleTranslator();
	virtual ~FMBundleTranslator();

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


class FMBundleTranslatorClassDesc : public ClassDesc2 
{
public:
	int           IsPublic() override                               { return TRUE; }
	void*         Create(BOOL /*loading = FALSE*/) override         { return new FMBundleTranslator(); }
	const TCHAR*  ClassName() override                              { return GetString(IDS_CLASS_NAME); }
	const TCHAR*  NonLocalizedClassName() override                  { return _T("FMBundleTranslator"); }
	SClass_ID     SuperClassID() override                           { return SCENE_IMPORT_CLASS_ID; }
	Class_ID      ClassID() override                                { return FMBundleTranslator_CLASS_ID; }
	const TCHAR*  Category() override                               { return GetString(IDS_CATEGORY); }

	const TCHAR*  InternalName() override                           { return _T("FMBundleTranslator"); } // Returns fixed parsable name (scripter-visible name)
	HINSTANCE     HInstance() override                              { return hInstance; } // Returns owning module handle


};

ClassDesc2* GetFMBundleTranslatorDesc()
{
	static FMBundleTranslatorClassDesc FMBundleTranslatorDesc;
	return &FMBundleTranslatorDesc; 
}


//--- FMBundleTranslator -------------------------------------------------------
FMBundleTranslator::FMBundleTranslator()
{

}

FMBundleTranslator::~FMBundleTranslator()
{

}

int FMBundleTranslator::ExtCount()
{
//#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR* FMBundleTranslator::Ext(int /*n*/)
{
//#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("modelbin");
}

const TCHAR* FMBundleTranslator::LongDesc()
{
//#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("ForzaTech Bundle Importer");
}

const TCHAR* FMBundleTranslator::ShortDesc()
{
//#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("ForzaTech Bundle Importer");
}

const TCHAR* FMBundleTranslator::AuthorName()
{
//#pragma message(TODO("Return ASCII Author name"))
	return _T("Autodesk");
}

const TCHAR* FMBundleTranslator::CopyrightMessage()
{
//#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR* FMBundleTranslator::OtherMessage1()
{
	// TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR* FMBundleTranslator::OtherMessage2()
{
	// TODO: Return other message #2 in any
	return _T("");
}

unsigned int FMBundleTranslator::Version()
{
//#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return FT_MAX_VERSION_NUMBER;
}

void FMBundleTranslator::ShowAbout(HWND /*hWnd*/)
{
	// Optional
}

int FMBundleTranslator::DoImport(const TCHAR* filename, ImpInterface* /*importerInt*/, Interface* /*ip*/, BOOL suppressPrompts)
{
	std::string file_name = fmnext::convertWideToStdString(filename);

	QPointer<FMOptionsDialog> dialog;
	if (dialog.isNull())
	{
		dialog = new FMOptionsDialog(FMQtWindow::mainWindow());
		dialog->setFixedSize(QSize(FMQtWindow::dpiScale(400), FMQtWindow::dpiScale(200)));
		dialog->setModelData(file_name);
		dialog->show();
	}

	return TRUE;
}

int FMBundleTranslator::ZoomExtents()
{
	return ZOOMEXT_NO;
}
