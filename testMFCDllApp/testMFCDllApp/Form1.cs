using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace testMFCDllApp
{
    public partial class Form1 : Form
    {
        [DllImport("MIES.dll", CallingConvention = CallingConvention.Cdecl)] //D:\\Project\\Lab_Project\\MIES\\MIES\\testMFCDllApp\\testMFCDllApp\\bin\\Debug\\
        // 接口函数调用申明
        // 护士工作站提示患者农合是否直补接口函数
        static extern void DisplayPatientZBInfo(StringBuilder result, StringBuilder patient_id, StringBuilder visit_id);
        // 预结算查询接口函数
        [DllImport("MIES.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void DisplayPatientPreSettlementInfo(StringBuilder result, StringBuilder patient_id, StringBuilder visit_id);
        // 提示类查询接口函数
        [DllImport("MIES.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void DisplayPatientInfo(StringBuilder result, StringBuilder patient_id, StringBuilder visit_id);
        // 医生提交时提示需要审批的项目接口函数
        [DllImport("MIES.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void DisplayProjectCheckInfo(StringBuilder result, StringBuilder patient_id, StringBuilder visit_id, StringBuilder item_class, StringBuilder item_code, StringBuilder charge_type, StringBuilder item_spec);
        // 医保目录对应相关操作接口函数
        [DllImport("MIES.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void DisplayMedicalInsuranceInfo(StringBuilder result, StringBuilder patient_id, StringBuilder visit_id,  StringBuilder item_class, StringBuilder item_code, StringBuilder charge_type, StringBuilder item_spec);
        // 打印自费知情同意书相关操作接口函数
        [DllImport("MIES.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void PrintSelfChargeConsentLetter(StringBuilder result, StringBuilder patient_id);
        // 打印每日清单相关操作接口函数
        [DllImport("MIES.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void PrintDailyList(StringBuilder patient_id);
        
        // 门诊用药医保目录对应相关操作接口函数
        [DllImport("MIES.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void DisplayOutpatientServiceInfo(StringBuilder result, StringBuilder patient_id, StringBuilder item_code, StringBuilder zz_code);
        

        public Form1()
        {
            InitializeComponent();
        }

        // 直补
        private void button1_Click(object sender, EventArgs e)
        {
            StringBuilder patient_id = new StringBuilder(Controls["patient_id"].Text);
            StringBuilder visit_id = new StringBuilder(Controls["visit_id"].Text);

            // 调用接口函数
            StringBuilder result = new StringBuilder(4096);
            DisplayPatientZBInfo(result, patient_id, visit_id);
            INFOTEXTBOX.Text = result.ToString();
        }

        // 提示类
        private void button2_Click(object sender, EventArgs e)
        {
            StringBuilder patient_id = new StringBuilder(Controls["patient_id"].Text);
            StringBuilder visit_id = new StringBuilder(Controls["visit_id"].Text);

            // 调用接口函数
            StringBuilder result = new StringBuilder(4096);
            DisplayPatientInfo(result, patient_id, visit_id);
            INFOTEXTBOX.Text = result.ToString();
        }

        // 费用预查询
        private void button3_Click(object sender, EventArgs e)
        {
            StringBuilder patient_id = new StringBuilder(Controls["patient_id"].Text);
            StringBuilder visit_id = new StringBuilder(Controls["visit_id"].Text);

            // 调用接口函数
            StringBuilder result = new StringBuilder(4096);
            DisplayPatientPreSettlementInfo(result, patient_id, visit_id);
            INFOTEXTBOX.Text = result.ToString();
        }

        // 审批提示
        private void button4_Click(object sender, EventArgs e)
        {
            StringBuilder patient_id = new StringBuilder(Controls["patient_id"].Text);
            StringBuilder visit_id = new StringBuilder(Controls["visit_id"].Text);
            StringBuilder item_class = new StringBuilder(Controls["item_class"].Text);
            StringBuilder item_code = new StringBuilder(Controls["item_code"].Text);
            StringBuilder charge_type = new StringBuilder(Controls["charge_type"].Text);
            StringBuilder item_spec = new StringBuilder(Controls["item_spec"].Text);

            // 调用接口函数
            StringBuilder result = new StringBuilder(4096);
            DisplayProjectCheckInfo(result, patient_id, visit_id, item_class, item_code, charge_type, item_spec);
            INFOTEXTBOX.Text = result.ToString();
        }

        // 医保目录对应
        private void button5_Click(object sender, EventArgs e)
        {
            StringBuilder patient_id = new StringBuilder(Controls["patient_id"].Text);
            StringBuilder visit_id = new StringBuilder(Controls["visit_id"].Text);
            StringBuilder item_class = new StringBuilder(Controls["item_class"].Text);
            StringBuilder item_code = new StringBuilder(Controls["item_code"].Text);
            StringBuilder charge_type = new StringBuilder(Controls["charge_type"].Text);
            StringBuilder item_spec = new StringBuilder(Controls["item_spec"].Text);

            // 调用接口函数
            StringBuilder result = new StringBuilder(4096);
            DisplayMedicalInsuranceInfo(result, patient_id, visit_id, item_class, item_code, charge_type, item_spec);
            INFOTEXTBOX.Text = result.ToString();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            StringBuilder patient_id = new StringBuilder(Controls["patient_id"].Text);
            StringBuilder result = new StringBuilder(4096);
            PrintSelfChargeConsentLetter(result, patient_id);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            StringBuilder patient_id = new StringBuilder(Controls["patient_id"].Text);
            PrintDailyList(patient_id);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            StringBuilder patient_id = new StringBuilder(Controls["patient_id"].Text);
            StringBuilder item_code = new StringBuilder(Controls["item_code"].Text);
            StringBuilder item_spec = new StringBuilder(Controls["item_spec"].Text);
            StringBuilder zz_code = new StringBuilder(Controls["zz_code"].Text);

            // 调用接口函数
            StringBuilder result = new StringBuilder(4096);
            DisplayOutpatientServiceInfo(result, patient_id, item_code, zz_code);
            INFOTEXTBOX.Text = result.ToString();
        }
    }
}
