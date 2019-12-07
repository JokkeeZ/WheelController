using System;
using System.Threading;
using System.Threading.Tasks;
using SharpDX.DirectInput;

namespace RBRWC
{
	class WheelController : IDisposable
	{
		private readonly DirectInput input;
		private readonly Joystick wheel;

		public event EventHandler<WheelInputEventArgs> OnInput;

		private CancellationTokenSource cts;

		public WheelController()
		{
			input = new DirectInput();
			wheel = new Joystick(input, GetWheelGuid(input));

			wheel.Properties.BufferSize = 128;
		}

		public async Task ListenForInput(IntPtr windowHandle)
		{
			// Obtains access to the wheel.
			wheel.Acquire();

			cts = new CancellationTokenSource();

			await Task.Run(async () =>
			{
				while (!cts.IsCancellationRequested)
				{
					// Handle wheel input only when the game is foreground window.
					var currentForegroundWindow = NativeMethods.GetForegroundWindow();
					if (currentForegroundWindow != windowHandle)
					{
						continue;
					}

					var data = wheel.GetBufferedData();
					await HandleWheelInput(data);
				}
			});
		}

		public void RequestDispose()
		{
			cts.Cancel();
		}

		private async Task HandleWheelInput(JoystickUpdate[] updates)
		{
			foreach (var update in updates)
			{
				OnInput?.Invoke(this, new WheelInputEventArgs(update.Offset, update.Value));
				await Task.Delay(25);
			}
		}

		public Guid GetWheelGuid(DirectInput directInput)
		{
			var devices = directInput.GetDevices(DeviceType.Driving,
				DeviceEnumerationFlags.ForceFeedback);

			if (devices.Count > 0)
				return devices[0].InstanceGuid;

			return Guid.Empty;
		}

		public void Dispose()
		{
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		protected virtual void Dispose(bool disposing)
		{
			if (disposing)
			{
				cts?.Dispose();

				input.Dispose();
				wheel.Dispose();
			}
		}
	}
}
