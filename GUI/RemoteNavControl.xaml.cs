///////////////////////////////////////////////////////////////////////
// RemoteNavControl.xaml.cs - Control GUI for remote Navigation      //
// ver 1.0                                                           //
// Yuxuan Xing                                                       //
// Source:Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018  //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based control GUI for Project4Demo.  It's 
 * responsibilities are to:
 * - Provide a display of directory contents of the remote server.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 *   
 * Required Files:
 * ---------------
 * RemoteNavControl.xaml, RemoteNavControl.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 22 Apr 2018
 * - first release
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
using MsgPassingCommunication;

namespace WpfApp1
{
  public partial class RemoteNavControl : UserControl
  {
    internal CsEndPoint navEndPoint_;
    internal Stack<string> pathStack_ = new Stack<string>();

    public RemoteNavControl()
    {
      InitializeComponent();
    }

    private void Refresh_Click(object sender, RoutedEventArgs e)
    {
      MainWindow win = (MainWindow)Window.GetWindow(this);
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(navEndPoint_));
      msg.add("command", "getDirs");
      msg.add("path", pathStack_.Peek());
      win.translater.postMessage(msg);
      msg.remove("command");
      msg.add("command", "getFiles");
      win.translater.postMessage(msg);
    }

    internal void refreshDisplay()
    {
      Refresh_Click(this, null);
    }
    internal void clearDirs()
    {
      DirList.Items.Clear();
    }
    //----< function dispatched by child thread to main thread >-------

    internal void addDir(string dir)
    {
      DirList.Items.Add(dir);
    }
    //----< function dispatched by child thread to main thread >-------

    internal void insertParent()
    {
      DirList.Items.Insert(0, "..");
    }
    //----< function dispatched by child thread to main thread >-------

    internal void clearFiles()
    {
      FileList.Items.Clear();
    }
    //----< function dispatched by child thread to main thread >-------

    internal void addFile(string file)
    {
      FileList.Items.Add(file);
    }
    //----< show file text on MouseDoubleClick >-----------------------

    private void FileList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      MainWindow win = (MainWindow)Window.GetWindow(this);
      string fileName = (string)FileList.SelectedItem;
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(navEndPoint_));
      msg.add("command", "sendFile");
      msg.add("path", pathStack_.Peek());
      msg.add("fileName", fileName);
      win.translater.postMessage(msg);
    }
    //----< respond to mouse double-click on dir name >----------------

    private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      MainWindow win = (MainWindow)Window.GetWindow(this);

      // build path for selected dir
      string selectedDir = (string)DirList.SelectedItem;
      string path;
      if (selectedDir == "..")
      {
        if (pathStack_.Count > 1)  // don't pop off "Storage"
          pathStack_.Pop();
        else
          return;
      }
      else
      {
        path = pathStack_.Peek() + "/" + selectedDir;
        pathStack_.Push(path);
      }
      // display path in Dir TextBlcok
      PathTextBlock.Text = win.removeFirstDir(pathStack_.Peek());

      // build message to get dirs and post it
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(navEndPoint_));
      msg.add("command", "getDirs");
      msg.add("path", pathStack_.Peek());
      win.translater.postMessage(msg);

      // build message to get files and post it
      msg.remove("command");
      msg.add("command", "getFiles");
      win.translater.postMessage(msg);
    }
    private void Analyze_Click(object sender, RoutedEventArgs e)
    {
      MainWindow win = (MainWindow)Window.GetWindow(this);
      // build message to analyze code on current server path
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(navEndPoint_));
      msg.add("command", "codeAnalyze");
      msg.add("path", pathStack_.Peek());
      win.translater.postMessage(msg);
      //pathStack_.Pop();
    }
  }
}
