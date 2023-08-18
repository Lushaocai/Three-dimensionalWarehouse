using S7.Net;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using DataLib;
namespace 智能立库上位机_新_
{

    public partial class Form1 : Form
    {
         Plc mYplc =new Plc(CpuType.S71500,"192.168.0.1",0,1);
         PlcData myPlcDATA = new PlcData();


        public Form1()
        {
            InitializeComponent();
        }




        private void button1_Click(object sender, EventArgs e)
        {

            if (!mYplc.IsConnected) {
                try
                {
                 mYplc.Open();
                 MessageBox.Show("连接成功");
                }
                catch (Exception) { 
                 MessageBox.Show("连接失败");               
                }
            }

        }
        private void button3_Click(object sender, EventArgs e)
        {
            if(mYplc.IsConnected)
            {
                MessageBoxButtons button = MessageBoxButtons.YesNo;
                DialogResult result = MessageBox.Show("是否断开PLC", "警告", button, MessageBoxIcon.Warning);
                if (result == DialogResult.Yes)
                { 
                mYplc.Close();
                MessageBox.Show("PLC已断开");                
                }      
                
            }
        }


        private void Form1_Load_1(object sender, EventArgs e)
        {     



        }


        private void timer1_Tick(object sender, EventArgs e)
        {
            if (mYplc.IsConnected)
            {
                //plc 写数据
                mYplc.Write("M200.0", myPlcDATA.确认数量);
                mYplc.Write("M200.1", myPlcDATA.出库模式);
                mYplc.Write("M200.3", myPlcDATA.启动);
                mYplc.Write("M200.4", myPlcDATA.停止);
                mYplc.Write("M200.5", myPlcDATA.取消);

                //plc 读数据
                myPlcDATA.出库启动 = (bool)mYplc.Read("M200.6");
                myPlcDATA.入库启动 = (bool)mYplc.Read("M200.7");
                myPlcDATA.运行标志位 = (bool)mYplc.Read("M208.0");
                myPlcDATA.停止中 = (bool)mYplc.Read("M208.1");
                myPlcDATA.仓库1数据 = (ushort)mYplc.Read("MW300");
                myPlcDATA.仓库2数据 = (ushort)mYplc.Read("MW302");
                myPlcDATA.仓库3数据 = (ushort)mYplc.Read("MW302");
                myPlcDATA.仓库1货物进入传感器 = (bool)mYplc.Read("I0.6");
                myPlcDATA.仓库1货物离开传感器 = (bool)mYplc.Read("I0.7");


                //PLC状态显示
                label1.BackColor = Color.Lime;
                label1.Text = "PLC已连接";
            }
            else
            {

                label1.BackColor = Color.Red;
                label1.Text = "PLC未连接";
            }


            //灰色图表设置
            chart1.Series[0].Points[0].SetValueY(myPlcDATA.仓库1数据); //已入库
            chart1.Series[0].Points[1].SetValueY(54 - myPlcDATA.仓库1数据);//空仓数量
            chart1.Series[0].Points[0].Label = "已入库数量:" + myPlcDATA.仓库1数据.ToString();
            chart1.Series[0].Points[1].Label = "空仓库数量:" + (54 - myPlcDATA.仓库1数据).ToString();
            //灰色图表设置
            chart2.Series[0].Points[0].SetValueY(myPlcDATA.仓库2数据); //已入库
            chart2.Series[0].Points[1].SetValueY(54 - myPlcDATA.仓库2数据);//空仓数量
            chart2.Series[0].Points[0].Label = "已入库数量:" + myPlcDATA.仓库2数据.ToString();
            chart2.Series[0].Points[1].Label = "空仓库数量:" + (54 - myPlcDATA.仓库2数据).ToString();
            //灰色图表设置
            chart3.Series[0].Points[0].SetValueY(myPlcDATA.仓库3数据); //已入库
            chart3.Series[0].Points[1].SetValueY(54 - myPlcDATA.仓库3数据);//空仓数量
            chart3.Series[0].Points[0].Label = "已入库数量:" + myPlcDATA.仓库3数据.ToString();
            chart3.Series[0].Points[1].Label = "空仓库数量:" + (54 - myPlcDATA.仓库3数据).ToString();

            //运行状态
            if (myPlcDATA.运行标志位 && !myPlcDATA.停止中)
            {
                label2.BackColor = Color.Lime;
                label2.Text = "设备运行";
            }
            if (!myPlcDATA.运行标志位 && !myPlcDATA.停止中)
            {
                label2.BackColor = Color.Red;
                label2.Text = "设备停止";
                //groupBox8.Enabled = false;

            }
            if ( myPlcDATA.停止中)
            {

                label2.BackColor = Color.Yellow;
                label2.Text = "停止中..";

            }

            //当前模式
            if (myPlcDATA.出库启动 == true)
            {
                label3.Text = "出库模式已启动";
                label3.BackColor = Color.Lime;
                label4.Text = "入库模式已停止";
                label4.BackColor = Color.Red;
                //groupBox8.Enabled = true;
            }
            if (myPlcDATA.入库启动 == true)
            {
                label3.Text = "出库模式已停止";
                label3.BackColor = Color.Red;
                label4.Text = "入库模式已启动";
                label4.BackColor = Color.Lime;
                //groupBox8.Enabled = false;
                myPlcDATA.取消 = false;
            }

            //进出库模式
            //1号仓出库数值限制
            if (numericUpDown1.Value > myPlcDATA.仓库1数据)
            {
                numericUpDown1.Value = myPlcDATA.仓库1数据;
            }
            //1号仓出库数值限制
            if (numericUpDown2.Value > myPlcDATA.仓库2数据)
            {
                numericUpDown2.Value = myPlcDATA.仓库2数据;
            }
            //3号仓出库数值限制
            if (numericUpDown3.Value > myPlcDATA.仓库3数据)
            {
                numericUpDown3.Value = myPlcDATA.仓库3数据;
            }

            myPlcDATA.灰色出库数量 = (int)numericUpDown1.Value;
            myPlcDATA.绿色出库数量 = (int)numericUpDown2.Value;
            myPlcDATA.蓝色出库数量 = (int)numericUpDown3.Value;




        }

   


        //启动系统
        private void button2_MouseDown(object sender, MouseEventArgs e)
        {
            myPlcDATA.启动 = true;
        }

        private void button2_MouseUp(object sender, MouseEventArgs e)
        {
            myPlcDATA.启动 = false;
        }

        //停止系统
        private void button4_MouseDown(object sender, MouseEventArgs e)
        {
            myPlcDATA.停止 = true;
        }

        private void button4_MouseUp(object sender, MouseEventArgs e)
        {
            myPlcDATA.停止 = false;
        }

        //启动出库模式
        private void button5_MouseDown(object sender, MouseEventArgs e)
        {
            myPlcDATA.出库模式 = true;
        }

        private void button5_MouseUp(object sender, MouseEventArgs e)
        {
            myPlcDATA.出库模式 = false;
        }

        private void button7_MouseDown(object sender, MouseEventArgs e)
        {
            mYplc.Write("MW202", (ushort)myPlcDATA.灰色出库数量);
            mYplc.Write("MW204", (ushort)myPlcDATA.绿色出库数量);
            mYplc.Write("MW206", (ushort)myPlcDATA.蓝色出库数量);
            myPlcDATA.确认数量 = true;
        }

        private void button7_MouseUp(object sender, MouseEventArgs e)
        {
            myPlcDATA.确认数量 = false;
        }

        private void button6_MouseDown(object sender, MouseEventArgs e)
        {
            myPlcDATA.取消 = true;
        }

        private void button6_MouseUp(object sender, MouseEventArgs e)
        {

        }
    }


}
    

