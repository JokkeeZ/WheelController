using System;
using SharpDX.DirectInput;

namespace RBRWC
{
	class WheelInputEventArgs : EventArgs
	{
		public JoystickOffset Offset { get; }

		public int Value { get; }

		public WheelInputEventArgs(JoystickOffset offset, int value)
		{
			Offset = offset;
			Value = value;
		}
	}
}
