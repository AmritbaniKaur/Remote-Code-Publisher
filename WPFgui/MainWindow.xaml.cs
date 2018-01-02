
////////////////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs   - WPF gui written in C#               					  //
//                      - makes calls into C++\CLI shim dll                       //
//                      - Shim is bound to a MockChannel static library           //
// Source:			    Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017 //
// ver 1.1																		  //
//--------------------------------------------------------------------------------//
//	Language:		Visual C++ 2015, Visual Studio 2015 Community Edition		  //
//	Platform:		HP Envy x360, Core i7, Windows 10 Home						  //
//	Application:	CSE 687- OOD Project #4, Spring 2017						  //
//	Author:			Amritbani Sondhi,											  //
//					Graduate Student, Syracuse University						  //
//					asondhi@syr.edu												  //
////////////////////////////////////////////////////////////////////////////////////
/*
 * WPF GUI is written in C#, whereas MockChannel is written in C++
 * CLI Shim DLL is used as a bridge between both which is written in managed C++
 * The Mock Channel is further connected to the Message Client and Message Server
 * which provides the further functionality of Uploading, Publishing and
 * Downloading
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;

namespace WPFgui
{
  public partial class MainWindow : Window
  {
    Shim shim = new Shim();
    Thread tRcv = null;

    public MainWindow()
    {
      InitializeComponent();
    }

        // when upload event is triggered sends POST as a message to the MockChannel
        private void Upload_Event(object sender, RoutedEventArgs e)
        {
            String msg = "POST";
            SendList.Items.Insert(0, msg);
            shim.PostMessage(msg);
        }

        // when publish event is triggered sends PUBLISH as a message to the MockChannel
        private void Publish_Event(object sender, RoutedEventArgs e)
        {
            String msg = "PUBLISH";
            SendList.Items.Insert(0, msg);
            shim.PostMessage(msg);
        }

        // when download event is triggered sends DOWNLOAD as a message to the MockChannel
        private void Download_Event(object sender, RoutedEventArgs e)
        {
            String msg = "DOWNLOAD";
            SendList.Items.Insert(0, msg);
            shim.PostMessage(msg);
        }

    void Add(String msg)
    {
        RecvList.Items.Insert(0, msg);
    }

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
            this.Title = "Remote Code Publisher";
            this.WindowStartupLocation = WindowStartupLocation.CenterScreen;

            tRcv = new Thread(
        () =>
        {
          while(true)
          {
            String msg = shim.GetMessage();
            Action<String> act = new Action<string>(Add);
            Object[] args = { msg };
            Dispatcher.Invoke(act, args);
          }
        }
      );
      tRcv.Start();
    }
  }
}
