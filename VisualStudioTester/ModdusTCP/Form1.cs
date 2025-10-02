using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO.Ports;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using Modbus.Data;
using Modbus.Device;
using Modbus.Utility;
using System.Windows.Forms;

namespace ModdusTCP
{
    public partial class Form1 : Form
    {
        TcpClient client;
        ModbusIpMaster master;
        bool value = true;



        public Form1()
        {
            InitializeComponent();
            panel1.Enabled = false;


        }
        

        // bind the listening socket to the port


        private void Form1_Load(object sender, EventArgs e)
        {
            Reg_type.SelectedIndex = 0;

        }

        private void button1_Click(object sender, EventArgs e)


        {
            try
            {
                // 1️⃣ Validate register type selection
                if (Reg_type.SelectedIndex < 0)
                    throw new Exception("Please select a register type.");

                string selectedItem = Reg_type.Items[Reg_type.SelectedIndex].ToString();

                // 2️⃣ Validate register number
                if (!ushort.TryParse(Reg_Nr.Text.Trim(), out ushort registerNumber))
                    throw new Exception("Please enter a valid register number (0-65535).");

                // 3️⃣ Validate value to write
                if (string.IsNullOrWhiteSpace(textBox2.Text))
                    throw new Exception("Please enter a value to write.");

                // 4️⃣ Handle writable register types
                switch (selectedItem)
                {
                    case "Coil":
                        if (!bool.TryParse(textBox2.Text.Trim(), out bool coilValue))
                            throw new Exception("Please enter true/false for Coil.");
                        master.WriteSingleCoil(registerNumber, coilValue);
                        break;

                    case "Holding Register":
                        if (!ushort.TryParse(textBox2.Text.Trim(), out ushort holdValue))
                            throw new Exception("Please enter a valid number (0-65535) for Holding Register.");
                        master.WriteSingleRegister(registerNumber, holdValue);
                        break;

                    case "Discrete Input":
                    case "Input Register":
                        throw new Exception($"{selectedItem} is read-only. Cannot write.");

                    default:
                        throw new Exception("Unknown register type selected.");
                }

                // Optional: update textbox with current value
                if (selectedItem == "Coil")
                {
                    bool[] coilRead = master.ReadCoils(registerNumber, 1);
                    textBox2.Text = coilRead[0].ToString();
                }
                else if (selectedItem == "Holding Register")
                {
                    ushort[] holdRegRead = master.ReadHoldingRegisters(registerNumber, 1);
                    textBox2.Text = holdRegRead[0].ToString();
                }
            }

            catch (Exception)
            {
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            try {
                if (Reg_type.SelectedIndex < 0)
                {
                    throw new Exception("Please select a register type.");
                }

                // Check if register number textbox is empty
                if (string.IsNullOrWhiteSpace(Reg_Nr.Text))
                {
                    throw new Exception("Please enter a register number.");
                }

                // Get selected register type and number
                string selectedItem = Reg_type.Items[Reg_type.SelectedIndex].ToString();
                string reg_id = Reg_Nr.Text.Trim();

                // If you need reg_id as ushort
                ushort registerNumber;
                if (!ushort.TryParse(reg_id, out registerNumber))
                {
                    throw new Exception("Register number must be a valid number (0-65535).");
                }

                switch (selectedItem)
                {
                    case "Coil":
                        bool[] coilValue = master.ReadCoils(registerNumber, 1);
                        Read_Val.Text = coilValue[0].ToString();
                        break;

                    case "Discrete Input":
                        bool[] inputValue = master.ReadInputs(registerNumber, 1);
                        Read_Val.Text = inputValue[0].ToString();
                        break;

                    case "Input Register":
                        ushort[] inputRegValue = master.ReadInputRegisters(registerNumber, 1);
                        Read_Val.Text = inputRegValue[0].ToString();
                        break;

                    case "Holding Register":
                        ushort[] holdingRegValue = master.ReadHoldingRegisters(registerNumber, 1);
                        Read_Val.Text = holdingRegValue[0].ToString();
                        break;

                    default:
                        throw new Exception("Unknown register type selected.");
                }



            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message, "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                client = new TcpClient(Ip_box.Text, Convert.ToInt32(port_box.Text));
                master = ModbusIpMaster.CreateIp(client);
                status_label.ForeColor = Color.Green;
                status_label.Text = "Conected";
                button3.Enabled = false;
                panel1.Enabled = true;


            }
            catch
            {
                status_label.ForeColor = Color.Red;
                status_label.Text = "Not Conected";
                _disconnect.Enabled = false;
                button3.Enabled = true;
                panel1.Enabled = false;
            }
            
           
        }

        private void disconnect_Click(object sender, EventArgs e)
        {
            try {
                master.Dispose();
                client.Dispose();
                _disconnect.Enabled = false;
                button3.Enabled = true;
                status_label.Text = "";
                panel1.Enabled = false;
            } catch (Exception) { 
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //bool[] Coil_val = master.ReadCoils(800,1);
            //textBox1.Text = Coil_val[0].ToString();
            master.WriteSingleCoil(20, true);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            master.WriteSingleCoil(20, false);
        }

        private void Ip_box_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void button6_Click(object sender, EventArgs e)
        {
            bool[] Coil_val = master.ReadCoils(0, 1);
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }
    }
    }

