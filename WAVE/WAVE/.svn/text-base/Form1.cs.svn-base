using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Diagnostics;
using System.Threading;

namespace WAVE
{
	public partial class Form1 : Form
	{
		SerialPort myPort = new SerialPort();	//sets up the serial port
		Xbee myXbee = new Xbee();				//sets up the Xbee.cs class
		String priceFromWeb;			//the actual price in string form "12611" or similar
		byte[] price_packet_byte = new byte[6];
		string RxString;				//sets up all data that is received
		bool OnOff;
		bool receivePacketIsHandledAndComplete;
		static Random _r = new Random();

		/// Initializes the UI, Opens the COM Port using Default COM5
		/// Adds Event handler for Serial Data Received
		public Form1()
		{
			InitializeComponent();  //Windows Forms Microsoft wrote this
			this.readAvailablePorts();
			//prepares for incoming data event handler
			myPort.ReceivedBytesThreshold = 9;		//trigger SerialDataReceivedEventHandler on 9 bytes received
			myPort.DataReceived += new SerialDataReceivedEventHandler(SerialDataReceivedEventHandler);
		}//Form1

		public void readAvailablePorts()
		{
			string[] portsArray = SerialPort.GetPortNames();
			try
			{
				AvailablePorts_listbox.Items.AddRange(portsArray);
			}//try
			catch (System.ArgumentException)
			{
				MessageBox.Show("Argument Error");
			}//catch
			catch (System.InvalidOperationException)
			{
				MessageBox.Show("Invalid Operation");
			}
		}

		
		/// If port is closed, opens COM5 and switches button
		/// If port is open, closes port
		private void OpenCloseComPortButton_Click(object sender, EventArgs e)
		{
			string selectedPort = (string)AvailablePorts_listbox.SelectedItem;
			if (!myPort.IsOpen)
			{
				try //handles the cases when you try to connect but the xbee is not plugged in 
				{
					myPort.PortName = selectedPort;
					myPort.Open();
					//CurrentComPort.Text = myPort.PortName;
					for(int i = 0; i<101; i++)
						ComPort_StatusBar.Value = i;
					OpenCloseComPort_button.Text = "ClosePort";

					//requestDataOutlet1.Enabled = true;
					readRelay_button.Enabled = true;
					readRelay_ToolStripMenuItem.Enabled = true;
					commitRelay_button.Enabled = true;
					commitRelay_ToolStripMenuItem.Enabled = true;
					updatePrice_button.Enabled = true;
					updatePrice_ToolStripMenuItem.Enabled = true;
					checkPrice_Button.Enabled = true;
					checkPrice_ToolStripMenuItem.Enabled = true;
					turnOnOffAllOutlets_Button.Enabled = true;

					relayTimerOutlet1.Start();
					
				}//try
				catch (IOException)
				{
					MessageBox.Show("Sorry Your Port is not available.\nPlease Check your Connections.");
				}//catch
				catch (System.ArgumentException)
				{
					MessageBox.Show("Please Select an Avalable Port.");
				}//catch
			}//if
			else
			{
				myPort.Close();
				for(int i=100; i>=0; i--)
					ComPort_StatusBar.Value = i;
				//CurrentComPort.Enabled = true;
				OpenCloseComPort_button.Text = "OpenPort";

				//requestDataOutlet1.Enabled = false;
				readRelay_button.Enabled = false;
				readRelay_ToolStripMenuItem.Enabled = false;
				commitRelay_button.Enabled = false;
				commitRelay_ToolStripMenuItem.Enabled = false;
				updatePrice_button.Enabled = false;
				updatePrice_ToolStripMenuItem.Enabled = false;
				checkPrice_Button.Enabled = false;
				checkPrice_ToolStripMenuItem.Enabled = false;
				turnOnOffAllOutlets_Button.Enabled = false;
				requestSetOfData_timer.Stop();
			}//else
		}//OpenCloseComPortButton_Click

		
		/// SerialDataReceivedEventHandler
		/// Triggers on incomming data.  Info is saved from the input buffer
		public void SerialDataReceivedEventHandler(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
		{
			RxString = "";
			for (int i = 0; i < 9; i++)
			{
				int fromBuffer = myPort.ReadChar(); //saves data to 'fromBuffer'
				RxString += Convert.ToChar(fromBuffer);     //'fromBuffer' converted to receivd string
			}
			this.Invoke(new EventHandler(Data_Relay_Price));
		}//SerialDataReceivedEventHandler

		
		/// Separates data from string received into array
		/// Calls myXbee.discoverData to sort through data
		///     Received data is sent through 'incomingData'
		///     Returns actual power-data, the status of the relay, or the price_TextBox on the UC
		///     If data, saves into separate text files
		/// For price_TextBox returned: verify's its current.
		/// For status returned: changes application status bars
		public void Data_Relay_Price(object sender, EventArgs e)
		{
			//MessageBox.Show(RxString);  //shows received data in popup window
			PortMonitor_textbox.ForeColor = Color.Green;
			PortMonitor_textbox.Text = RxString;

			//stores each byte into an array
			switch (RxString.Substring(0, 1))
			{
				case "D":
					myXbee.Data(RxString);

					string usage = RxString.Substring(2, 7);
					if (usage.Substring(0, 1) == "0")
					{
						usage = RxString.Substring(3, 6);
						if (usage.Substring(0, 1) == "0")
						{
							usage = RxString.Substring(4, 5);
							if (usage.Substring(0, 1) == "0")
							{
								usage = RxString.Substring(5, 4);
								if (usage.Substring(0, 1) == "0")
									usage = RxString.Substring(6, 3);
							}
						}
					}

					if (RxString.Substring(1, 1) == "1")
					{
						updateTextbox1(usage+" Watts");
						updateTextbox1Price("$"+Convert.ToString(Convert.ToDouble(usage)*720/1000*Convert.ToInt16(priceFromWeb.Substring(1,5))/100000));
					}
					else
					{
						updateTextbox2(usage+" Watts");
						updateTextbox2Price("$"+Convert.ToString(Convert.ToDouble(usage) * 720 / 1000 * Convert.ToInt16(priceFromWeb.Substring(1,5)) / 100000));
					}
					//MessageBox.Show("Received Data");
					break;

				case "R":
					OnOff = myXbee.Relay(RxString);
					if (myXbee.RelayWho == "1")
						cb1.Checked = OnOff;

					if (myXbee.RelayWho == "2")
						cb2.Checked = OnOff;

					//MessageBox.Show(Convert.ToString(OnOff));
					UpdateOutletStatus();
					//MessageBox.Show("Outlet Status Switched");
					break;

				case "P":	//checks the return of price from UC; if not what we sent, resend
					try
					{
						int CompareString = string.Compare(priceFromWeb.Substring(1, 5), (myXbee.Price(RxString)));
						if (CompareString != 0)
						{
							updatePrice_button.PerformClick(); //updates price by sending it to UC
							MessageBox.Show("Price Not Current\nTry Updating Again");
						}
						else
						{
							MessageBox.Show("Oultet " + myXbee.PriceWho + " Price Up To Date");
						}
						break;
					}
					catch
					{
						RxString = "";
						myPort.DiscardInBuffer();
						MessageBox.Show("Error: Data Transfered Incorrectly\nPlease Try Again");
						break;
					}

				default:
					//MessageBox.Show("Receive string didn't start with 1,2, or 3");
					break;
			}//switch
			myPort.DiscardInBuffer();
			receivePacketIsHandledAndComplete = true;
		}//Data_Relay_Price

		private void UpdateOutletStatus()
		{
			if (myXbee.RelayWho == "1")
			{
				if (cb1.Checked)
					for(int i = 0; i<101;i++)
						progressBar1.Value = i;
				else
					for (int i = 100; i >= 0; i--)
						progressBar1.Value = i;
			}//if outlet 1
			if (myXbee.RelayWho == "2")
			{
				if (cb2.Checked)
					for (int i = 0; i < 101; i++)
						progressBar2.Value = i;
				else
					for (int i = 100; i >= 0; i--)
						progressBar2.Value = i;
			}//if outlet 2
			return;
		}//UpdateOutletStatus

		/// Plots data with gnuplot using either separate graphs or one overlapping graph
		private void display_graph_Click(object sender, EventArgs e)
		{
			if (this.overlap_radio_button.Checked == true)
				Process.Start(@"D:\WAVE\newGnuplotOverlap.DEM");
			else if (this.sep_graphs_radio_button.Checked == true)
				Process.Start(@"D:\WAVE\newGnuplotSeperate.DEM");
			return;
		}//display_graph_Click


		/// getPriceStringFromWeb
		/// Goes to internet, grabs webpage,
		/// extracts 5 digit price, converts to $0.string and prints on UI
		/// creates a byte array containting 312611, prints the price to a text file and returns the array
		public string getPriceStringFromWeb()
		{
			Internet myInternet = new Internet();
			string weEnergiesWebsiteHTML = "";
			try
			{
				weEnergiesWebsiteHTML = myInternet.DownloadHTMLPage("http://www.we-energies.com/residential/acctoptions/wi_pricetariff.htm");
			}//try
			catch (Exception _Exception)
			{
				MessageBox.Show("Exception caught in process: {0}", _Exception.ToString());
				return null;
			}//catch

			//finds Energy charge (right before the price)
			int index = weEnergiesWebsiteHTML.IndexOf("Energy charge");
			priceFromWeb = weEnergiesWebsiteHTML.Substring(index + 27, 5);
			price_TextBox.Text = "$ 0." + priceFromWeb + " per kWh";    //prints the price_TextBox to display
			return priceFromWeb;         //returns the price '12611' as string
		}//getPriceStringFromWeb

		private void WritePriceTextFile()
		{
			//writes price to text file
			TextWriter priceWrite_StreamWriter = new StreamWriter(@"D:\WAVE\ThePrice.txt");
			priceWrite_StreamWriter.Write(price_TextBox.Text);
			priceWrite_StreamWriter.Close();//closes the text document so it can be modified by other processes
			return;
		}//WritePriceTextFile

		// Gets updated Price from Web, Writes price to Text File, creates bytePricePacket
		private void updatePrice_Button_Click(object sender, EventArgs e)
		{
			try
			{
				getPriceStringFromWeb();    //string pr = "312611";
			}
			catch
			{
				MessageBox.Show("No Internet Connection Present");
			}

			try
			{
				WritePriceTextFile();
			}
			catch { }
			
			priceFromWeb = "P" + priceFromWeb + "000";
			myPort.Write(priceFromWeb);
			checkPriceOutlet1_timer.Start();
			return;
		}//updatePrice_Button_Click

		private void turnOnOffAllOutlets_Button_Click(object sender, EventArgs e)
		{
			if (turnOnOffAllOutlets_Button.Text == "Turn All Outlets Off")
			{
				turnOnOffAllOutlets_Button.Text = "Turn All Outlets On";
				myPort.Write("R10000000");
				Thread.Sleep(500);
				myPort.Write("R20000000");
			}
			else
			{
				turnOnOffAllOutlets_Button.Text = "Turn All Outlets Off";
				myPort.Write("R11000000");
				Thread.Sleep(500);
				myPort.Write("R21000000");
			}
		}

		private void commitRelay_Button_Click(object sender, EventArgs e)
		{
			if (cb1.Checked == true)	//outlet 1
				myPort.Write("R11000000");
			else
				myPort.Write("R10000000");
			UpdateOutletStatus();
			Thread.Sleep(500);

			if (cb2.Checked == true)	//outlet 2
				myPort.Write("R21000000");
			else
				myPort.Write("R20000000");

			UpdateOutletStatus();
		}//commitRelay_Button_Click

		private void AvailablePorts_listbox_SelectedIndexChanged(object sender, EventArgs e)
		{
				OpenCloseComPort_button.Enabled = true;
		}

		
		
		//**********************************************************************************
		//******************************* TIMERS AND REQUESTING DATA ***********************
		//**********************************************************************************

		private void dataTimerOutlet1_Tick(object sender, EventArgs e)
		{
			dataTimerOutlet1.Stop();
			if (receivePacketIsHandledAndComplete == true)
			{
				receivePacketIsHandledAndComplete = false;
				dataTimerOutlet2.Start();		//commented out for troubleshooting with 1 outlet
				//requestSetOfData_timer.Start();
			}
			else
			{
				if (myPort.IsOpen)
				{
					myPort.DiscardInBuffer();
					dataTimerOutlet1.Start();
					requestDataOutlet1();
				}
			}
		}

		private void requestDataOutlet1()
		{
			receivePacketIsHandledAndComplete = false;	//initialize to wait for received data
			myPort.Write("D1Q000000");
		}

		private void sendSecondDataRequest()
		{
			receivePacketIsHandledAndComplete = false;	//initialize to wait for received data
			myPort.Write("D2Q000000");
		}

		private void dataTimerOutlet2_Tick(object sender, EventArgs e)
		{
			dataTimerOutlet2.Stop();
			if (receivePacketIsHandledAndComplete == true)
			{
				receivePacketIsHandledAndComplete = false;
				requestSetOfData_timer.Start();
			}
			else
			{
				if (myPort.IsOpen)
				{
					myPort.DiscardInBuffer();
					dataTimerOutlet2.Start();
					sendSecondDataRequest();
				}
			}
		}

		private void requestSetOfData_timer_Tick(object sender, EventArgs e)
		{
			requestSetOfData_timer.Stop();
			PortMonitor_textbox.Text = "";
			dataTimerOutlet1.Start();
		}



		/// PRICE
		/// </summary>
		private void refreshPrice_timer_Tick(object sender, EventArgs e)
		{
			//timer runs for a day = 24hr*60min*60sec*1000ms = 86400000ms
			requestSetOfData_timer.Stop();
			refreshPrice_timer.Stop();
			updatePrice_button.PerformClick();
		}

		private void CheckPrice_Button_Click(object sender, EventArgs e)
		{
			receivePacketIsHandledAndComplete = false;	//initialize to wait for received data
			myPort.Write("PQ1000000");		//outlet 1 request price acknowledge
		}

		private void checkPriceOutlet1_timer_Tick(object sender, EventArgs e)
		{
			checkPriceOutlet1_timer.Stop();
			if (receivePacketIsHandledAndComplete == true)
			{
				receivePacketIsHandledAndComplete = false;
				checkPriceOutlet2_timer.Start();		//commented out for troubleshooting with 1 outlet
				//refreshPrice_timer.Start();
				//requestSetOfData_timer.Start();
			}
			else
			{
				if (myPort.IsOpen)
				{
					myPort.DiscardInBuffer();
					checkPriceOutlet1_timer.Start();
					checkPrice_Button.PerformClick();
				}
			}
		}

		private void checkPriceOutlet2_timer_Tick(object sender, EventArgs e)
		{
			checkPriceOutlet2_timer.Stop();
			if (receivePacketIsHandledAndComplete == true)
			{
				receivePacketIsHandledAndComplete = false;
				refreshPrice_timer.Start();
				requestSetOfData_timer.Start();
			}
			else
			{
				if (myPort.IsOpen)
				{
					myPort.DiscardInBuffer();
					checkPriceOutlet2_timer.Start();
					sendSecondPriceRequest();
				}
			}
		}

		private void sendSecondPriceRequest()
		{
			receivePacketIsHandledAndComplete = false;	//initialize to wait for received data
			myPort.Write("PQ2000000");
		}



		/// RELAY
		/// </summary>
		private void ReadRelay_Button_Click(object sender, EventArgs e)
		{
			receivePacketIsHandledAndComplete = false;	//initialize to wait for received data
			myPort.Write("R1Q000000");		//read '2'=relay; '1'=outlet 1
		}//ReadRelay_Button_Click

		private void relayTimerOutlet1_Tick(object sender, EventArgs e)
		{
			relayTimerOutlet1.Stop();
			if (receivePacketIsHandledAndComplete == true)
			{
				receivePacketIsHandledAndComplete = false;
				relayTimerOutlet2.Start();		//commented out for troubleshooting with 1 outlet
				//updatePrice_button.PerformClick();
			}
			else
			{
				if (myPort.IsOpen)
				{
					myPort.DiscardInBuffer();
					relayTimerOutlet1.Start();
					readRelay_button.PerformClick();
				}
			}
		}

		private void relayTimerOutlet2_Tick(object sender, EventArgs e)
		{
			relayTimerOutlet2.Stop();
			if (receivePacketIsHandledAndComplete == true)
			{
				receivePacketIsHandledAndComplete = false;
				UpdateOutletStatus();
				updatePrice_button.PerformClick();
			}
			else
			{
				if (myPort.IsOpen)
				{
					myPort.DiscardInBuffer();
					relayTimerOutlet2.Start();
					sendSecondRelayRequest();
				}
			}
		}

		private void sendSecondRelayRequest()
		{
			receivePacketIsHandledAndComplete = false;	//initialize to wait for received data
			myPort.Write("R2Q000000");
		}



//*********************************************************************************************
//***********************	Menu Bar Functions	***********************************************
//*********************************************************************************************
		private void readRelay_ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			readRelay_button.PerformClick();
		}

		private void quitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Application.Exit();
		}

		private void checkPrice_ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			checkPrice_Button.PerformClick();
		}

		private void commitRelayToolStripMenuItem_Click(object sender, EventArgs e)
		{
			commitRelay_button.PerformClick();
		}

		private void overlap_ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			overlapToolStripMenuItem.Checked = true;
			overlap_radio_button.Checked = true;
			sep_graphs_radio_button.Checked = false;
			separateToolStripMenuItem.Checked = false;
		}

		private void separate_ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			overlapToolStripMenuItem.Checked = false;
			overlap_radio_button.Checked = false;
			sep_graphs_radio_button.Checked = true;
			separateToolStripMenuItem.Checked = true;
		}

		private void graph_ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			display_graph.PerformClick();
		}

		private void currentPrice_ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			TextReader priceRead_StreamReader = new StreamReader(@"D:\WAVE\ThePrice.txt");
			string msgbox = priceRead_StreamReader.ReadLine();
			priceRead_StreamReader.Close();//closes the text document so it can be modified by other processes
			MessageBox.Show(msgbox);
		}

		private void updatePrice_ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			updatePrice_button.PerformClick();
		}



	//******** Virtual Return Buttons **************
		private void virtualReturn_Click(object sender, EventArgs e)
		{
			myPort.Write(packet_textbox.Text);
		}

		public void updateTextbox1(string sampleWattage)
		{
			Outlet1_Last_sample_textbox.Text = sampleWattage;
		}

		public void updateTextbox1Price(string PricePerTime)
		{
			Outlet1_Last_sample_price_textbox.Text = PricePerTime;
		}

		public void updateTextbox2(string sampleWattage)
		{
			Outlet2_Last_sample_textbox.Text = sampleWattage;
		}

		public void updateTextbox2Price(string PricePerTime)
		{
			Outlet2_Last_sample_price_textbox.Text = PricePerTime;
		}

	}
}
