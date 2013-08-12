using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Diagnostics;

namespace Plot_Incoming_Data
{
	public partial class Form1 : Form
	{
		SerialPort myPort = new SerialPort();	//sets up the serial port
		char RxString;				//sets up all data that is received
		//int numberOfSamples = 1;

		public Form1()
		{
			InitializeComponent();
			//myPort.ReceivedBytesThreshold = numberOfSamples;		//trigger SerialDataReceivedEventHandler on 1 bytes received
			//myPort.ReadTimeout = 1000;				//one second timeout for incoming data not reaching 9 bytes
			myPort.DataReceived += new SerialDataReceivedEventHandler(SerialDataReceivedEventHandler);
		}

		private void OpenCloseComPort_button_Click(object sender, EventArgs e)
		{
			if (!myPort.IsOpen)
			{
				try //handles the cases when you try to connect but the xbee is not plugged in 
				{
					myPort.PortName = textBox2.Text;
					myPort.Open();
					myPort.DiscardOutBuffer();
					//CurrentComPort.Text = myPort.PortName;
					for(int i = 0; i<=100; i++)
						ComPort_StatusBar.Value = i;
					OpenCloseComPort_button.Text = "ClosePort";
				}//try
				catch (IOException)
				{
					MessageBox.Show("Sorry Your Port is not available,\nPlease Check your Connections");
				}//catch
				catch (System.ArgumentException)
				{
					MessageBox.Show("Please Select an Available Port");
				}//catch
			}//if
			else
			{
				myPort.Close();
				for(int i=100; i>=0; i--)
					ComPort_StatusBar.Value = i;
				//CurrentComPort.Enabled = true;
				OpenCloseComPort_button.Text = "OpenPort";
			}//else
		}//OpenCloseComPortButton_Click

		public void SerialDataReceivedEventHandler(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
		{
			receivedData.BackColor = Color.LawnGreen;
			//RxString = ' ';
			//for (int i = 0; i < numberOfSamples; i++)
			//{
				int fromBuffer = myPort.ReadChar(); //saves data to 'fromBuffer'
				RxString = Convert.ToChar(fromBuffer);     //'fromBuffer' converted to receivd string
			//}
			this.Invoke(new EventHandler(SaveToTextFile));
			receivedData.BackColor = Color.Red;
		}//SerialDataReceivedEventHandler

		private void SaveToTextFile(object sender, EventArgs e)
		{
			//writes price to text file
			TextWriter plotDataWrite_StreamWriter = new StreamWriter(@"D:\WAVE\PlottingData.txt",true);
			if (RxString == ',')
				plotDataWrite_StreamWriter.WriteLine("");
			else if (RxString == ';')
				plotDataWrite_StreamWriter.WriteLine(";");
			else
				if (RxString == '\0')
					;
				else
					plotDataWrite_StreamWriter.Write(RxString);
				
			plotDataWrite_StreamWriter.Close();//closes the text document so it can be modified by other processes
			return;
		}

		private void sendText_Click(object sender, EventArgs e)
		{
			myPort.Write(textBox1.Text);
		}//WritePriceTextFile
	}
}