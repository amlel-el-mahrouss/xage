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

#include "RenderingWindow.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	XPXStudioApp::RenderingWindow form;
	Application::Run(% form);

}