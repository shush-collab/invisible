Invisible
A software + hardware solution to your leetcode nightmare.

Quick summary: flash http_request_sender_typer.ino to an ESP32-S3 development board, run the included server (server.py) on your computer, then connect the board to the computer while pressing the reset button. The board will present itself as a USB keyboard and automatically type content pushed from the server.

This README explains how to get started, what the repository contains, safety notes, and troubleshooting tips.

Contents

http_request_sender_typer.ino — the Arduino/ESP sketch that turns an ESP32-S3 into a USB HID keyboard client.

server.py — a simple server that sends text/typing commands to the device.

(These files live in the repository.) citeturn1view1turn1view2

Requirements

Hardware

ESP32-S3 development board (the board must support native USB / USB HID).

USB-C or USB cable (data + power) to connect the ESP32-S3 to your computer.

Software

Arduino IDE or PlatformIO (or another tool that can compile and flash .ino sketches for ESP32-S3).

Python 3.x to run the included server (if you use server.py).

Quickstart

Flash the sketch

Open http_request_sender_typer.ino in the Arduino IDE or PlatformIO. Select the appropriate ESP32-S3 board (for example: ESP32S3 Dev Module), the correct port, and upload the sketch to the board. If you prefer PlatformIO, add the project to your platformio.ini and platformio run --target upload.

Run the server

On your computer, run the Python server included in this repo: python3 server.py.

If the server has configuration (host/port or API keys), edit server.py accordingly before launching.

Put the board into keyboard mode

While connecting the board to your laptop (plugging it in), press (or hold and release) the reset button on the ESP32-S3 as instructed. This sequence causes the board to enumerate as a USB HID keyboard and accept typing commands from the server.

Use

With the server running and the device connected/enumerated as a keyboard, actions triggered by the server will be typed automatically on the host machine.

These three steps are the standard workflow for this project: flash .ino → run server → connect while resetting the board. (Project files: http_request_sender_typer.ino, server.py). citeturn1view1turn1view2

Example (minimal)

# on the host machine
python3 server.py

# on the ESP32-S3
# flash the .ino from Arduino IDE or PlatformIO

Troubleshooting

Board doesn't enumerate as a keyboard:

Make sure you have the correct ESP32-S3 board selected in the IDE and that the board supports native USB/HID.

Try different USB cables (some cables are power-only).

Try pressing reset while plugging in (some boards require a specific timing to enter USB HID mode).

Server can't reach the device:

Confirm the server is running and listening on the expected port.

Check logs printed by server.py (if present).

Sketch fails to compile:

Install the latest Espressif/ESP32 board support for Arduino or make sure PlatformIO has the esp32-s3 platform installed.

Ensure any referenced libraries (USB HID wrappers) are installed — check the top of the .ino for #include lines and add those libraries to your Arduino IDE.

Security & Safety

This project turns a microcontroller into a USB keyboard. A device that behaves like a keyboard can send arbitrary keystrokes to whatever host it’s plugged into. Use responsibly and never connect it to machines where you don't have permission.

Don't run untrusted server code that can instruct the device to type sensitive commands.

Contributing

Contributions are welcome! If you'd like help with features, bug fixes, or improved documentation:

Fork the repo.

Create a branch with a descriptive name.

Open a pull request describing your change.

License

Include your chosen license here (MIT, Apache-2.0, etc.) or add a LICENSE file to the repo.

Acknowledgements

Thanks for using Invisible! If you want the README tailored (add example server commands, dependencies, or a requirements.txt), tell me what environment you plan to run the server in and I’ll add exact instructions.

