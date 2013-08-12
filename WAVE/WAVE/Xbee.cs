using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;

namespace WAVE
{
	class Xbee
	{
		public string RelayWho;
		public string PriceWho;

		public void Data(string RxString)
		{
			StreamReader read = new StreamReader("d:\\WAVE\\ThePrice.txt");
			String PricefromText = read.ReadLine();
			PricefromText = PricefromText.Substring(2, 7);

			String tempStringWattage = "";
			int time = 24;	//one day
			double tempDoubleWattage = 0.0;

			foreach (char Character in RxString.Substring(2, 7))
			{
				if (Character > 57 | Character < 46 | Character == 47)
				{	//if not a number or decimal point
					MessageBox.Show("Data Received Contained something other than a number", "Data Received ERROR");
					tempStringWattage += "0";
				}
				else
					tempStringWattage += Character;		//save the number if it is a number
			}
			tempDoubleWattage = Convert.ToDouble(tempStringWattage);
			double PricePerTime = tempDoubleWattage * time / 1000 * Convert.ToDouble(PricefromText);

			string format = "M/d/yyyy HH:mm:ss";
			DateTime rightNow = DateTime.Now;
			string rightNowString = rightNow.ToString(format);

			switch (RxString.Substring(1, 1))
			{
				case "1":
					//outlet 1
					StreamWriter sw = new StreamWriter("d:\\WAVE\\Outlet1.txt", true);
					sw.Write(rightNowString);
					sw.Write("	");
					sw.WriteLine(Convert.ToString(PricePerTime));
					sw.Close();
					break;

				case "2":
					//outlet 2
					StreamWriter sw2 = new StreamWriter("d:\\WAVE\\Outlet2.txt", true);
					sw2.Write(rightNowString);
					sw2.Write("	");
					sw2.WriteLine(Convert.ToString(PricePerTime));
					sw2.Close();
					break;

				//************************
				//can include more outlets
				//************************
				default:
					MessageBox.Show("Invalid Address or Outlet");
					break;
			}//switch
		}

		public bool Relay(string RxString)
		{
			RelayWho = RxString.Substring(1, 1);
			string onoff;
			onoff = RxString.Substring(2, 1);
			if (onoff == "1")
				return true;
			else
				return false;
		}

		public string Price(string RxString)
		{
			PriceWho = RxString.Substring(1, 1);
			RxString = RxString.Substring(2, 5);
			return RxString;
		}
	}
}
