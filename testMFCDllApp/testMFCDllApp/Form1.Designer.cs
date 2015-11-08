namespace testMFCDllApp
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.INFOTEXTBOX = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.patient_id = new System.Windows.Forms.TextBox();
            this.visit_id = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.item_class = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.charge_type = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.item_spec = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.item_code = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.button6 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.zz_code = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(34, 319);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "医保手续";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // INFOTEXTBOX
            // 
            this.INFOTEXTBOX.Location = new System.Drawing.Point(39, 209);
            this.INFOTEXTBOX.Multiline = true;
            this.INFOTEXTBOX.Name = "INFOTEXTBOX";
            this.INFOTEXTBOX.Size = new System.Drawing.Size(402, 97);
            this.INFOTEXTBOX.TabIndex = 1;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(134, 319);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "提示类";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(232, 319);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 3;
            this.button3.Text = "费用预查询";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(33, 357);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 4;
            this.button4.Text = "审批提示";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(133, 357);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(89, 23);
            this.button5.TabIndex = 5;
            this.button5.Text = "医保目录对应";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(35, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 6;
            this.label1.Text = "病人id";
            // 
            // patient_id
            // 
            this.patient_id.Location = new System.Drawing.Point(82, 26);
            this.patient_id.Name = "patient_id";
            this.patient_id.Size = new System.Drawing.Size(100, 21);
            this.patient_id.TabIndex = 7;
            // 
            // visit_id
            // 
            this.visit_id.Location = new System.Drawing.Point(253, 26);
            this.visit_id.Name = "visit_id";
            this.visit_id.Size = new System.Drawing.Size(100, 21);
            this.visit_id.TabIndex = 9;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(206, 29);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "住院号";
            // 
            // item_class
            // 
            this.item_class.Location = new System.Drawing.Point(118, 62);
            this.item_class.Name = "item_class";
            this.item_class.Size = new System.Drawing.Size(100, 21);
            this.item_class.TabIndex = 11;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(35, 65);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 12);
            this.label3.TabIndex = 10;
            this.label3.Text = "医院项目类别";
            // 
            // charge_type
            // 
            this.charge_type.Location = new System.Drawing.Point(118, 129);
            this.charge_type.Name = "charge_type";
            this.charge_type.Size = new System.Drawing.Size(100, 21);
            this.charge_type.TabIndex = 13;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(35, 132);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(53, 12);
            this.label4.TabIndex = 12;
            this.label4.Text = "医保费别";
            // 
            // item_spec
            // 
            this.item_spec.Location = new System.Drawing.Point(118, 162);
            this.item_spec.Name = "item_spec";
            this.item_spec.Size = new System.Drawing.Size(100, 21);
            this.item_spec.TabIndex = 15;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(35, 165);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(77, 12);
            this.label5.TabIndex = 14;
            this.label5.Text = "医院项目规格";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(37, 194);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(53, 12);
            this.label6.TabIndex = 18;
            this.label6.Text = "返回结果";
            // 
            // item_code
            // 
            this.item_code.Location = new System.Drawing.Point(118, 95);
            this.item_code.Name = "item_code";
            this.item_code.Size = new System.Drawing.Size(100, 21);
            this.item_code.TabIndex = 20;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(35, 98);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(77, 12);
            this.label7.TabIndex = 19;
            this.label7.Text = "医院项目编码";
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(34, 402);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(148, 23);
            this.button6.TabIndex = 21;
            this.button6.Text = "打印自费知情同意书";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(208, 402);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(148, 23);
            this.button7.TabIndex = 22;
            this.button7.Text = "打印每日清单";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.button7_Click);
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(232, 356);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(75, 23);
            this.button8.TabIndex = 23;
            this.button8.Text = "门诊用药";
            this.button8.UseVisualStyleBackColor = true;
            this.button8.Click += new System.EventHandler(this.button8_Click);
            // 
            // zz_code
            // 
            this.zz_code.Location = new System.Drawing.Point(324, 165);
            this.zz_code.Name = "zz_code";
            this.zz_code.Size = new System.Drawing.Size(100, 21);
            this.zz_code.TabIndex = 25;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(241, 168);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(53, 12);
            this.label8.TabIndex = 24;
            this.label8.Text = "重症编码";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(481, 440);
            this.Controls.Add(this.zz_code);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.button8);
            this.Controls.Add(this.button7);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.item_code);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.item_spec);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.charge_type);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.item_class);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.visit_id);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.patient_id);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.INFOTEXTBOX);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox INFOTEXTBOX;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox patient_id;
        private System.Windows.Forms.TextBox visit_id;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox item_class;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox charge_type;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox item_spec;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox item_code;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.TextBox zz_code;
        private System.Windows.Forms.Label label8;
    }
}

