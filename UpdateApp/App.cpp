/*
 * =====================================================================
 *
 *			Update App
 * 
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: UpdateForm.cpp
 *			Purpose: Main form, summons a new HTTP instance to download any new binaries.
 *
 * =====================================================================
 */

#include "UpdateForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	UpdateApp::UpdateForm form;
	Application::Run(% form);
}