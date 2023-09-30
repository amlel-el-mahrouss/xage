/*
 * =====================================================================
 *
 *			XPXUpdateApp
 * 
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: MainForm.cpp
 *			Purpose: Main form, summons a new HTTP instance to download any new binaries.
 *
 * =====================================================================
 */

#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	XPXUpdate::MainForm form;
	Application::Run(% form);
}