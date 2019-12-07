using System;
using System.Runtime.InteropServices;

namespace RBRWC
{
	class NativeMethods
	{
		[DllImport("user32.dll")]
		static extern bool SetForegroundWindow(IntPtr hWnd);

		[DllImport("user32.dll")]
		public static extern IntPtr GetForegroundWindow();

		[DllImport("user32.dll")]
		static extern bool ShowWindow(IntPtr handle, int nCmdShow);

		[DllImport("user32.dll")]
		static extern bool IsIconic(IntPtr handle);

		public static void BringProcessToFront(IntPtr processHandle)
		{
			if (IsIconic(processHandle))
			{
				const int SW_RESTORE = 9;
				ShowWindow(processHandle, SW_RESTORE);
			}

			SetForegroundWindow(processHandle);
		}
	}
}
