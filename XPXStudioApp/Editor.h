#pragma once

namespace XPXStudioApp {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace Irrlicht;

	/// <summary>
	/// Summary for EditForm
	/// </summary>
	public ref class EditForm : public System::Windows::Forms::Form
	{
	private:
		Scene::SceneNode^ mSelected;
	private: System::Windows::Forms::ContextMenuStrip^ contextMenuStrip2;
	private: System::Windows::Forms::TabControl^ tabControl1;

	private: System::Windows::Forms::TabPage^ tabPage4;
	private: System::Windows::Forms::TabPage^ tabPage5;

	private: System::Windows::Forms::TabPage^ tabPage3;











		   IrrlichtDevice^ mDevice;

	public:
		EditForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			auto params = gcnew IrrlichtCreationParameters();
			params->DriverMultithreaded = true;
			params->DriverType = Video::DriverType::OpenGL;
			params->WindowID = this->panel2->Handle;

			mDevice = IrrlichtDevice::CreateDevice(params);
			mDevice->SceneManager->ActiveCamera = mDevice->SceneManager->AddCameraSceneNodeMaya();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EditForm()
		{
			if (components)
			{
				delete components;
			}
		}





	private: System::Windows::Forms::StatusStrip^ statusStrip1;
	private: System::Windows::Forms::Panel^ panel1;

	private: System::Windows::Forms::TabControl^ mCtxSwitch;
	private: System::Windows::Forms::TabPage^ tabPage1;
	private: System::Windows::Forms::WebBrowser^ PXPWindow;
	private: System::Windows::Forms::TabPage^ tabPage2;

	private: System::Windows::Forms::Panel^ panel2;
	private: System::Windows::Forms::ColorDialog^ colorDialog1;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::ContextMenuStrip^ contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ createPartToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ createScriptToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ createScriptAsXassetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ importMeshAsXassetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^ selectNodeToolStripMenuItem;
	private: System::ComponentModel::IContainer^ components;


	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->mCtxSwitch = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->PXPWindow = (gcnew System::Windows::Forms::WebBrowser());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->createPartToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->createScriptToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->createScriptAsXassetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->importMeshAsXassetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->selectNodeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->contextMenuStrip2 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->panel1->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->mCtxSwitch->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// statusStrip1
			// 
			this->statusStrip1->Location = System::Drawing::Point(0, 659);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(1264, 22);
			this->statusStrip1->TabIndex = 3;
			this->statusStrip1->Text = L"mPlaceStat";
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(21)), static_cast<System::Int32>(static_cast<System::Byte>(21)),
				static_cast<System::Int32>(static_cast<System::Byte>(21)));
			this->panel1->Controls->Add(this->tabControl1);
			this->panel1->Controls->Add(this->mCtxSwitch);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(1264, 659);
			this->panel1->TabIndex = 4;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Controls->Add(this->tabPage4);
			this->tabControl1->Location = System::Drawing::Point(12, 62);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(200, 594);
			this->tabControl1->TabIndex = 12;
			// 
			// tabPage4
			// 
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(192, 618);
			this->tabPage4->TabIndex = 1;
			this->tabPage4->Text = L"Script";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// mCtxSwitch
			// 
			this->mCtxSwitch->AllowDrop = true;
			this->mCtxSwitch->Controls->Add(this->tabPage2);
			this->mCtxSwitch->Controls->Add(this->tabPage5);
			this->mCtxSwitch->Controls->Add(this->tabPage1);
			this->mCtxSwitch->Location = System::Drawing::Point(218, 62);
			this->mCtxSwitch->Multiline = true;
			this->mCtxSwitch->Name = L"mCtxSwitch";
			this->mCtxSwitch->SelectedIndex = 0;
			this->mCtxSwitch->Size = System::Drawing::Size(1034, 590);
			this->mCtxSwitch->TabIndex = 11;
			this->mCtxSwitch->Selected += gcnew System::Windows::Forms::TabControlEventHandler(this, &EditForm::RaiseNewRender);
			this->mCtxSwitch->Click += gcnew System::EventHandler(this, &EditForm::mCtxSwitch_Click);
			// 
			// tabPage1
			// 
			this->tabPage1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(215)), static_cast<System::Int32>(static_cast<System::Byte>(191)),
				static_cast<System::Int32>(static_cast<System::Byte>(81)));
			this->tabPage1->Controls->Add(this->PXPWindow);
			this->tabPage1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(62)), static_cast<System::Int32>(static_cast<System::Byte>(68)),
				static_cast<System::Int32>(static_cast<System::Byte>(125)));
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(1026, 564);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"ToolBox";
			// 
			// PXPWindow
			// 
			this->PXPWindow->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PXPWindow->Location = System::Drawing::Point(3, 3);
			this->PXPWindow->MinimumSize = System::Drawing::Size(20, 20);
			this->PXPWindow->Name = L"PXPWindow";
			this->PXPWindow->ScriptErrorsSuppressed = true;
			this->PXPWindow->Size = System::Drawing::Size(1020, 558);
			this->PXPWindow->TabIndex = 7;
			this->PXPWindow->Url = (gcnew System::Uri(L"https://play-xplicit.com/index.html", System::UriKind::Absolute));
			this->PXPWindow->DocumentCompleted += gcnew System::Windows::Forms::WebBrowserDocumentCompletedEventHandler(this, &EditForm::PXPWindow_DocumentCompleted);
			// 
			// tabPage2
			// 
			this->tabPage2->BackColor = System::Drawing::Color::Black;
			this->tabPage2->ContextMenuStrip = this->contextMenuStrip1;
			this->tabPage2->Controls->Add(this->panel2);
			this->tabPage2->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(1026, 564);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Place1";
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->createPartToolStripMenuItem,
					this->createScriptToolStripMenuItem, this->createScriptAsXassetToolStripMenuItem, this->importMeshAsXassetToolStripMenuItem,
					this->toolStripSeparator1, this->selectNodeToolStripMenuItem
			});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(229, 120);
			// 
			// createPartToolStripMenuItem
			// 
			this->createPartToolStripMenuItem->Name = L"createPartToolStripMenuItem";
			this->createPartToolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->createPartToolStripMenuItem->Text = L"Create Part";
			this->createPartToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::createPartToolStripMenuItem_Click);
			// 
			// createScriptToolStripMenuItem
			// 
			this->createScriptToolStripMenuItem->Name = L"createScriptToolStripMenuItem";
			this->createScriptToolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->createScriptToolStripMenuItem->Text = L"Create Script";
			// 
			// createScriptAsXassetToolStripMenuItem
			// 
			this->createScriptAsXassetToolStripMenuItem->Name = L"createScriptAsXassetToolStripMenuItem";
			this->createScriptAsXassetToolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->createScriptAsXassetToolStripMenuItem->Text = L"Import/Select Script as xasset";
			// 
			// importMeshAsXassetToolStripMenuItem
			// 
			this->importMeshAsXassetToolStripMenuItem->Name = L"importMeshAsXassetToolStripMenuItem";
			this->importMeshAsXassetToolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->importMeshAsXassetToolStripMenuItem->Text = L"Import Mesh as xasset";
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(225, 6);
			this->toolStripSeparator1->Click += gcnew System::EventHandler(this, &EditForm::toolStripSeparator1_Click);
			// 
			// selectNodeToolStripMenuItem
			// 
			this->selectNodeToolStripMenuItem->Name = L"selectNodeToolStripMenuItem";
			this->selectNodeToolStripMenuItem->Size = System::Drawing::Size(228, 22);
			this->selectNodeToolStripMenuItem->Text = L"Select Node";
			this->selectNodeToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::selectNodeToolStripMenuItem_Click);
			// 
			// panel2
			// 
			this->panel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel2->Location = System::Drawing::Point(3, 3);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(1020, 558);
			this->panel2->TabIndex = 0;
			this->panel2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &EditForm::panel2_Paint);
			// 
			// tabPage5
			// 
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Size = System::Drawing::Size(1026, 564);
			this->tabPage5->TabIndex = 2;
			this->tabPage5->Text = L"Script1";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// contextMenuStrip2
			// 
			this->contextMenuStrip2->Name = L"contextMenuStrip2";
			this->contextMenuStrip2->Size = System::Drawing::Size(61, 4);
			// 
			// tabPage3
			// 
			this->tabPage3->BackColor = System::Drawing::Color::White;
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(192, 568);
			this->tabPage3->TabIndex = 0;
			this->tabPage3->Text = L"Rendering";
			// 
			// EditForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->ClientSize = System::Drawing::Size(1264, 681);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->statusStrip1);
			this->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MaximumSize = System::Drawing::Size(1280, 720);
			this->MinimumSize = System::Drawing::Size(1280, 720);
			this->Name = L"EditForm";
			this->Text = L"RRMan - Place1";
			this->panel1->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->mCtxSwitch->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void panel2_createContext(System::Object^ sender) {
}
private: System::Void PXPWindow_DocumentCompleted(System::Object^ sender, System::Windows::Forms::WebBrowserDocumentCompletedEventArgs^ e) {
	if (e->Url->ToString() != "https://play-xplicit.com/index.html")
	{
		this->PXPWindow->Navigate("https://play-xplicit.com/index.html");
	}
}
private: System::Void mCtxSwitch_Click(System::Object^ sender, System::EventArgs^ e) {
}

private: System::Void panel2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	mDevice->VideoDriver->BeginScene(Video::ClearBufferFlag::Color | Video::ClearBufferFlag::Depth, gcnew Video::Color(0, 0, 0, 255));

	mDevice->SceneManager->DrawAll();

	mDevice->VideoDriver->EndScene();
}
private: System::Void createPartToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	auto node = mDevice->SceneManager->AddCubeSceneNode();

	node->SetMaterialFlag(Video::MaterialFlag::Lighting, false);

	node->Scale->X = 10;
	node->Scale->Y = 1;
	node->Scale->Z = 20;

	node->Position->X = 0;
	node->Position->Y = -2;
	node->Position->Z = 0;

	mSelected = node;
}
private: System::Void toolStripSeparator1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void selectNodeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	mDevice->SceneManager->ActiveCamera->Target = mSelected->Position;
}

private: System::Void RaiseNewRender(System::Object^ sender, System::Windows::Forms::TabControlEventArgs^ e) {

}
};
}
