using System;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using SharpDX.DirectInput;

namespace RBRWC
{
	class Program
	{
		static async Task Main()
		{
			Console.WriteLine("If this window is on top of RBR,");
			Console.WriteLine("You can just minimize this window to background!");

			var proc = Process.GetProcessesByName("RichardBurnsRally_SSE").FirstOrDefault();
			if (proc == null)
			{
				return;
			}

			NativeMethods.BringProcessToFront(proc.MainWindowHandle);

			using var wheel = new WheelController();
			wheel.OnInput += OnWheelInput;

			proc.EnableRaisingEvents = true;
			proc.Exited += (s, e) => wheel.RequestDispose();

			await wheel.ListenForInput(proc.MainWindowHandle);
		}

		static void OnWheelInput(object sender, WheelInputEventArgs e)
		{
			switch (e.Offset)
			{
				// 'X' button
				case JoystickOffset.Buttons5:
					if (e.Value == 128) // Pressed.
						SendKeys.SendWait("{ENTER}");
					break;

				// Triangle button
				case JoystickOffset.Buttons2:
					if (e.Value == 128) // Pressed.
						SendKeys.SendWait("{ESC}");
					break;

				case JoystickOffset.PointOfViewControllers0:
					{
						// Arrow up button pressed
						if (e.Value == 0)
						{
							SendKeys.SendWait("{UP}");
							break;
						}

						// Arrow right pressed
						if (e.Value == 9000)
						{
							SendKeys.SendWait("{RIGHT}");
							break;
						}

						// Arrow down pressed
						if (e.Value == 18000)
						{
							SendKeys.SendWait("{DOWN}");
							break;
						}

						// Arrow left pressed
						if (e.Value == 27000)
						{
							SendKeys.SendWait("{LEFT}");
							break;
						}
					}
					break;
			}
		}
	}
}
