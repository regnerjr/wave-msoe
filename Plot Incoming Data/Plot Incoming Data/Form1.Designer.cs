namespace Plot_Incoming_Data
{
	partial class Form1
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.OpenCloseComPort_button = new System.Windows.Forms.Button();
			this.ComPort_StatusBar = new System.Windows.Forms.ProgressBar();
			this.receivedData = new System.Windows.Forms.Button();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.sendText = new System.Windows.Forms.Button();
			this.textBox2 = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// OpenCloseComPort_button
			// 
			this.OpenCloseComPort_button.Location = new System.Drawing.Point(47, 38);
			this.OpenCloseComPort_button.Name = "OpenCloseComPort_button";
			this.OpenCloseComPort_button.Size = new System.Drawing.Size(201, 55);
			this.OpenCloseComPort_button.TabIndex = 0;
			this.OpenCloseComPort_button.Text = "&OpenPort";
			this.OpenCloseComPort_button.UseVisualStyleBackColor = true;
			this.OpenCloseComPort_button.Click += new System.EventHandler(this.OpenCloseComPort_button_Click);
			// 
			// ComPort_StatusBar
			// 
			this.ComPort_StatusBar.Location = new System.Drawing.Point(47, 99);
			this.ComPort_StatusBar.Name = "ComPort_StatusBar";
			this.ComPort_StatusBar.Size = new System.Drawing.Size(201, 23);
			this.ComPort_StatusBar.TabIndex = 1;
			// 
			// receivedData
			// 
			this.receivedData.Location = new System.Drawing.Point(31, 131);
			this.receivedData.Name = "receivedData";
			this.receivedData.Size = new System.Drawing.Size(229, 118);
			this.receivedData.TabIndex = 2;
			this.receivedData.Text = "Received Data!!";
			this.receivedData.UseVisualStyleBackColor = true;
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(32, 287);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(228, 20);
			this.textBox1.TabIndex = 3;
			// 
			// sendText
			// 
			this.sendText.Location = new System.Drawing.Point(49, 313);
			this.sendText.Name = "sendText";
			this.sendText.Size = new System.Drawing.Size(198, 29);
			this.sendText.TabIndex = 4;
			this.sendText.Text = "Send What is in Textbox";
			this.sendText.UseVisualStyleBackColor = true;
			this.sendText.Click += new System.EventHandler(this.sendText_Click);
			// 
			// textBox2
			// 
			this.textBox2.Location = new System.Drawing.Point(82, 10);
			this.textBox2.Name = "textBox2";
			this.textBox2.Size = new System.Drawing.Size(170, 20);
			this.textBox2.TabIndex = 5;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(28, 13);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(53, 13);
			this.label1.TabIndex = 6;
			this.label1.Text = "COM Port";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 366);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.textBox2);
			this.Controls.Add(this.sendText);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.receivedData);
			this.Controls.Add(this.ComPort_StatusBar);
			this.Controls.Add(this.OpenCloseComPort_button);
			this.Name = "Form1";
			this.Text = "Form1";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button OpenCloseComPort_button;
		private System.Windows.Forms.ProgressBar ComPort_StatusBar;
		private System.Windows.Forms.Button receivedData;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.Button sendText;
		private System.Windows.Forms.TextBox textBox2;
		private System.Windows.Forms.Label label1;

	}
}

