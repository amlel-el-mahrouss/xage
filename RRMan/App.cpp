/*
 * =====================================================================
 *
 *			RRMan
 *
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: App.cpp
 *			Purpose: main form.
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

	RRMan::RRRecentProjects form;
	Application::Run(% form);

}