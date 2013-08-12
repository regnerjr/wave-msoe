using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WAVE
{
	class Internet
	{
		public string DownloadHTMLPage( string _URL )
		{
			string _PageContent = null;
			System.Net.HttpWebRequest _HttpWebRequest = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(_URL);
			// Request response:
			System.Net.WebResponse _WebResponse = _HttpWebRequest.GetResponse();
			// Open data stream:
			System.IO.Stream _WebStream = _WebResponse.GetResponseStream();
			// Create reader object:
			System.IO.StreamReader _StreamReader = new System.IO.StreamReader(_WebStream);
			// Read the entire stream content:
			_PageContent = _StreamReader.ReadToEnd();
			// Cleanup
			_StreamReader.Close();
			_WebStream.Close();
			_WebResponse.Close();
			return _PageContent;
		}
	}
}
