/////////////////////////////////////////////////////////////////////////
// CodePopupWindow.xaml.cs - Window to display source code             //
// ver 1.0                                                             //
// Yuxuan Xing                                                         //
// Source:Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018    //
/////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF Window for Project4Demo.  It's 
 * responsibility is to display source code on demand.
 *   
 * Required Files:
 * ---------------
 * CodePopupWindow.xaml, CodePopupWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 30 Mar 2018
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
using System.Windows.Shapes;

namespace WpfApp1
{
  /// <summary>
  /// Interaction logic for CodePopupWindow.xaml
  /// </summary>
  public partial class CodePopupWindow : Window
  {
    public CodePopupWindow()
    {
      InitializeComponent();
    }
    private void exitButton_Click(object sender, RoutedEventArgs e)
    {
      this.Close();
    }
  }
}
