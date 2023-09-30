/*
 * =====================================================================
 *
 *			XPXStudioApp
 *
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: MainForm.cpp
 *			Purpose: Update form.
 *
 * =====================================================================
 */

#include "RRRecentProjects.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	XPXStudioApp::RRRecentProjects form;
	Application::Run(% form);

}