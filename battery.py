#!/usr/bin/env python3

import os
import sys
import signal
import serial
import subprocess

process = None
last_osd = None

def start_osd(filename, text):
	global process
	global last_osd

	if process:
		if last_osd != (filename, text):
			os.killpg(process.pid, signal.SIGTERM)

	last_osd = (filename, text)
	args = [
		"/root/raspi-gameboy-battery/raspidmx/pngview/pngview",
		"-l", "999999",
		"-x", "0",
		"-y", "0",
		"-b", "0",
		"-n", 
	]

	if text:
		args.extend(["-s", text])

	args.append(filename)

	process = subprocess.Popen(
		args,
		stdout=sys.stdout,
		stderr=sys.stderr,
		preexec_fn=os.setsid
	) 

def file_from_voltage(voltage):
	if voltage <= 3.1:
		return "battery-0.png"
	elif voltage <= 3.3:
		return "battery-1.png"
	elif voltage <= 3.5:
		return "battery-2.png"
	elif voltage <= 3.7:
		return "battery-3.png"
	elif voltage <= 3.8:
		return "battery-4.png"
	else:
		return "battery-5.png"

def main():
	if len(sys.argv) > 2:
		sys.stderr.write("Usage: %s <tty>\n" % sys.argv[0])
		sys.exit(1)

	start_osd("/root/raspi-gameboy-battery/icons/battery-unknown.png", None)

	with serial.Serial(sys.argv[1] if len(sys.argv) == 2 else "/dev/ttyACM0", 115200, timeout=10) as ser:
		while True:
			line = float(ser.readline().strip())
			if not line:
				return
			voltage = (line / 1023) * 2.54 * (162/12) / 2
			start_osd("/root/raspi-gameboy-battery/icons/" + file_from_voltage(voltage), "%.1fV" % voltage)

if __name__ == '__main__':
	try:
		main()
	except KeyboardInterrupt:
		pass
	finally:
		if process:
			os.killpg(process.pid, signal.SIGTERM)
