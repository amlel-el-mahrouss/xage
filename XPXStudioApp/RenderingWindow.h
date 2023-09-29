#pragma once

namespace XPXStudioApp {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for RenderingWindow
	/// </summary>
	public ref class RenderingWindow : public System::Windows::Forms::Form
	{
	public:
		RenderingWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RenderingWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::StatusStrip^ ProjectStatus;
	private: System::Windows::Forms::ToolStripStatusLabel^ IsReady;
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::Panel^ panel2;

	protected:

	protected:

	protected:

	protected:








	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(RenderingWindow::typeid));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->ProjectStatus = (gcnew System::Windows::Forms::StatusStrip());
			this->IsReady = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->ProjectStatus->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.BackgroundImage")));
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pictureBox1->Location = System::Drawing::Point(12, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(58, 50);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// ProjectStatus
			// 
			this->ProjectStatus->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->IsReady });
			this->ProjectStatus->Location = System::Drawing::Point(0, 1019);
			this->ProjectStatus->Name = L"ProjectStatus";
			this->ProjectStatus->Size = System::Drawing::Size(1904, 22);
			this->ProjectStatus->TabIndex = 1;
			this->ProjectStatus->Text = L"statusStrip1";
			// 
			// IsReady
			// 
			this->IsReady->Font = (gcnew System::Drawing::Font(L"Urbanist ExtraBold", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->IsReady->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->IsReady->Name = L"IsReady";
			this->IsReady->Size = System::Drawing::Size(59, 17);
			this->IsReady->Text = L"Waiting...";
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(20)), static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(20)));
			this->panel1->Location = System::Drawing::Point(12, 78);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(58, 925);
			this->panel1->TabIndex = 2;
			// 
			// panel2
			// 
			this->panel2->Location = System::Drawing::Point(76, 12);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(1816, 991);
			this->panel2->TabIndex = 3;
			// 
			// RenderingWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(39)), static_cast<System::Int32>(static_cast<System::Byte>(39)),
				static_cast<System::Int32>(static_cast<System::Byte>(39)));
			this->ClientSize = System::Drawing::Size(1904, 1041);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->ProjectStatus);
			this->Controls->Add(this->pictureBox1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"RenderingWindow";
			this->Text = L"RRMan - Model Editor";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ProjectStatus->ResumeLayout(false);
			this->ProjectStatus->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
