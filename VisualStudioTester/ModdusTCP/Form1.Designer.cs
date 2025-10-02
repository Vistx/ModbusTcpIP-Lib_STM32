
namespace ModdusTCP
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
            this.button1 = new System.Windows.Forms.Button();
            this.Read_Val = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.Ip_box = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.port_box = new System.Windows.Forms.TextBox();
            this.button3 = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.status_label = new System.Windows.Forms.Label();
            this._disconnect = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.Reg_Nr = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.Reg_type = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(30, 168);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Write";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Read_Val
            // 
            this.Read_Val.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.Read_Val.Location = new System.Drawing.Point(129, 129);
            this.Read_Val.Name = "Read_Val";
            this.Read_Val.ReadOnly = true;
            this.Read_Val.Size = new System.Drawing.Size(121, 22);
            this.Read_Val.TabIndex = 1;
            this.Read_Val.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Read_Val.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(30, 128);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Read";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // Ip_box
            // 
            this.Ip_box.Location = new System.Drawing.Point(34, 12);
            this.Ip_box.Name = "Ip_box";
            this.Ip_box.Size = new System.Drawing.Size(118, 22);
            this.Ip_box.TabIndex = 3;
            this.Ip_box.Text = "192.168.0.129";
            this.Ip_box.TextChanged += new System.EventHandler(this.Ip_box_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.MediumBlue;
            this.label2.Location = new System.Drawing.Point(4, 12);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(24, 17);
            this.label2.TabIndex = 5;
            this.label2.Text = "IP:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.Crimson;
            this.label1.Location = new System.Drawing.Point(161, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 17);
            this.label1.TabIndex = 7;
            this.label1.Text = "Port:";
            // 
            // port_box
            // 
            this.port_box.Location = new System.Drawing.Point(200, 12);
            this.port_box.Name = "port_box";
            this.port_box.Size = new System.Drawing.Size(68, 22);
            this.port_box.TabIndex = 6;
            this.port_box.Text = "502";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(271, 11);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(85, 23);
            this.button3.TabIndex = 8;
            this.button3.Text = "Connect";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 300);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(52, 17);
            this.label3.TabIndex = 9;
            this.label3.Text = "Status:";
            // 
            // status_label
            // 
            this.status_label.AutoSize = true;
            this.status_label.Location = new System.Drawing.Point(70, 300);
            this.status_label.Name = "status_label";
            this.status_label.Size = new System.Drawing.Size(0, 17);
            this.status_label.TabIndex = 10;
            // 
            // _disconnect
            // 
            this._disconnect.Location = new System.Drawing.Point(254, 16);
            this._disconnect.Name = "_disconnect";
            this._disconnect.Size = new System.Drawing.Size(95, 23);
            this._disconnect.TabIndex = 11;
            this._disconnect.Text = "Disconnect";
            this._disconnect.UseVisualStyleBackColor = true;
            this._disconnect.Click += new System.EventHandler(this.disconnect_Click);
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(129, 169);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(121, 22);
            this.textBox2.TabIndex = 13;
            this.textBox2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // Reg_Nr
            // 
            this.Reg_Nr.Location = new System.Drawing.Point(129, 17);
            this.Reg_Nr.Name = "Reg_Nr";
            this.Reg_Nr.Size = new System.Drawing.Size(100, 22);
            this.Reg_Nr.TabIndex = 15;
            this.Reg_Nr.Text = "1";
            this.Reg_Nr.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Reg_Nr.TextChanged += new System.EventHandler(this.textBox3_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(56, 22);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(49, 17);
            this.label4.TabIndex = 16;
            this.label4.Text = "RegNr";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // Reg_type
            // 
            this.Reg_type.FormattingEnabled = true;
            this.Reg_type.Items.AddRange(new object[] {
            "Holding Register",
            "Input Registers",
            "Discrete Input",
            "Coil"});
            this.Reg_type.Location = new System.Drawing.Point(129, 75);
            this.Reg_type.Name = "Reg_type";
            this.Reg_type.Size = new System.Drawing.Size(121, 24);
            this.Reg_type.TabIndex = 17;
            this.Reg_type.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(35, 75);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(70, 17);
            this.label5.TabIndex = 18;
            this.label5.Text = "Reg Type";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.Reg_type);
            this.panel1.Controls.Add(this._disconnect);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.Read_Val);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.button2);
            this.panel1.Controls.Add(this.Reg_Nr);
            this.panel1.Controls.Add(this.textBox2);
            this.panel1.Location = new System.Drawing.Point(7, 59);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(359, 223);
            this.panel1.TabIndex = 19;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(374, 325);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.status_label);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.port_box);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.Ip_box);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox Read_Val;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox Ip_box;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox port_box;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label status_label;
        private System.Windows.Forms.Button _disconnect;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox Reg_Nr;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox Reg_type;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Panel panel1;
    }
}

