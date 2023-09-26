#pragma once

namespace XPXStudioApp {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for RRRecentProjects
	/// </summary>
	public ref class RRRecentProjects : public System::Windows::Forms::Form
	{
	public:
		RRRecentProjects(void)
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
		~RRRecentProjects()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button1;

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
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::DeepSkyBlue;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->Font = (gcnew System::Drawing::Font(L"Urbanist", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->ForeColor = System::Drawing::SystemColors::InactiveBorder;
			this->button2->Location = System::Drawing::Point(12, 307);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(260, 42);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Create XSD scene...";
			this->button2->UseVisualStyleBackColor = false;
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::DeepSkyBlue;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"Urbanist", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->ForeColor = System::Drawing::SystemColors::InactiveBorder;
			this->button1->Location = System::Drawing::Point(12, 259);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(260, 42);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Open from path...";
			this->button1->UseVisualStyleBackColor = false;
			// 
			// RRRecentProjects
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 361);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button2);
			this->ForeColor = System::Drawing::Color::Black;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(300, 400);
			this->MinimumSize = System::Drawing::Size(300, 400);
			this->Name = L"RRRecentProjects";
			this->ShowIcon = false;
			this->Text = L"XAGE RRman - Recents";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
